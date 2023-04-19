/**
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * This file contains all the read and write functions for the
 * library.
 *
 *******************************************************************
 *
 *  Thanks to Jason Weiler for describing the binary formats of the HUS and
 *  VIP formats at:
 *
 *  http://www.jasonweiler.com/HUSandVIPFileFormatInfo.html
 *
 *  Further thanks to github user tatarize for solving the mystery of the
 *  compression in:
 *
 *  https://github.com/EmbroidePy/pyembroidery
 *
 *  with a description of that work here:
 *
 *  https://stackoverflow.com/questions/7852670/greenleaf-archive-library
 *
 *  This is based on their work.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "embroidery_internal.h"

/* This is a work in progress.
 * ---------------------------
 */

/**
 * @brief 
 * 
 * @param data 
 * @param length 
 * @param output 
 * @param output_length 
 * @return int
 *
 * This avoids the now unnecessary compression by placing a
 * minimal header of 6 bytes and using only literals in the
 * huffman compressed part (see the sources above).
 *
 */
int hus_compress(char *data, int length, char *output, int *output_length)
{
    unsigned char *a = (unsigned char*)output;
    a[0] = length%256;
    a[1] = (length/256)%256;
    a[2] = 0x02;
    a[3] = 0xA0;
    a[4] = 0x01;
    a[5] = 0xFE;
    memcpy(output+6, data, length);
    *output_length = length+6;
    return 0;
}

/**
 * @brief 
 * These next 2 functions represent the Huffman class in tartarize's code.
 * @param h 
 */
void
huffman_build_table(huffman *h)
{
    int bit_length, i, max_length, size;
    max_length = 0;
    size = 1 << h->table_width;
    for (i = 0; i < h->table_width; i++) {
        if (h->lengths[i] > max_length) {
            max_length = h->lengths[i];
        }
    }
    for (bit_length=1; bit_length<=h->table_width; bit_length++) {
        int j;
        size /= 2;
        for (j=0; j < h->nlengths; j++) {
            if (h->lengths[j] == bit_length) {
                int k;
                for (k=0; k<size; k++) {
                    h->table[h->ntable+k] = j;
                    h->ntable++;
                }
            }
        }
    }
}

/**
 * @brief 
 * 
 */
int huffman_lookup_data[2];

/**
 * @brief 
 * 
 * @param h 
 * @param byte_lookup 
 * @return int* 
 */
int *huffman_lookup(huffman h, int byte_lookup)
{
    int *out = huffman_lookup_data;
    if (h.table_width == 0) {
        out[0] = h.default_value;
        out[1] = 0;
        return out;
    }
    out[0] = h.table[byte_lookup >> (16-h.table_width)];
    out[1] = h.lengths[out[0]];
    return out;
}

/* These functions represent the EmbCompress class. */

/**
 * @brief 
 * 
 */
void compress_init()
{

}

/**
 * @brief 
 * 
 * @param c 
 * @param length 
 * @return int 
 */
int
compress_get_bits(compress *c, int length)
{
    int i, end_pos_in_bits, start_pos_in_bytes,
        end_pos_in_bytes, value, mask_sample_bits,
        unused_bits, original;

    end_pos_in_bits = c->bit_position + length - 1;
    start_pos_in_bytes = c->bit_position / 8;
    end_pos_in_bytes = end_pos_in_bits / 8;
    value = 0;

    for (i=start_pos_in_bytes; i < end_pos_in_bytes+1; i++) {
        value <<= 8;
        if (i > c->input_length) {
            break;
        }
        value |= (c->input_data[i]) & 0xFF;
    }

    unused_bits = (7 - end_pos_in_bits) % 8;
    mask_sample_bits = (1<<length) - 1;
    original = (value >> unused_bits) & mask_sample_bits;
    return original;
}

/**
 * @brief 
 * 
 * @param c 
 * @param bit_count 
 * @return int 
 */
int
compress_pop(compress *c, int bit_count)
{
    int value = compress_get_bits(c, bit_count);
    c->bit_position += bit_count;
    return value;
}

/**
 * @brief 
 * 
 * @param c 
 * @param bit_count 
 * @return int 
 */
int
compress_peek(compress *c, int bit_count)
{
    return compress_get_bits(c, bit_count);
}

/**
 * @brief 
 * 
 * @param c 
 * @return int 
 */
int
compress_read_variable_length(compress *c)
{
    int q, m, s;
    m = compress_pop(c, 3);
    if (m!=7) {
        return m;
    }
    for (q=0; q<13; q++) {
        s = compress_pop(c, 1);
        if (s) {
            m++;
        }
        else {
            break;
        }
    }
    return m;
}

/**
 * @brief 
 * 
 * @param c 
 */
void
compress_load_character_length_huffman(compress *c)
{
    int count;
    count = compress_pop(c, 5);
    if (count == 0) {
        c->character_length_huffman.default_value = compress_pop(c, 5);
    }
    else {
        int i;
        for (i = 0; i < count; i++) {
            c->character_length_huffman.lengths[i] = 0;
        }
        for (i = 0; i < count; i++) {
            if (i==3) {
                i += compress_pop(c, 2);
            }
            c->character_length_huffman.lengths[i] = compress_read_variable_length(c);
        }
    }
    huffman_build_table(&(c->character_length_huffman));
}

/**
 * @brief 
 * 
 * @param c 
 */
void
compress_load_character_huffman(compress *c)
{
    int count;
    count = compress_pop(c, 9);
    if (count == 0) {
        c->character_huffman.default_value = compress_pop(c, 9);
    }
    else {
        int i = 0;
        while (i < count) {
            int *h = huffman_lookup(c->character_length_huffman,
                compress_peek(c, 16));
            c->bit_position += h[1];
            if (h[0]==0) {
                i += h[0];
            }
            else if (h[0]==1) {
                i += 3 + compress_pop(c, 4);
            }
            else if (h[0]==2) {
                i += 20 + compress_pop(c, 9);
            }
            else {
                c->character_huffman.lengths[i] = h[0] - 2;
                i++;
            }
        }
    }
    huffman_build_table(&(c->character_huffman));
}

/**
 * @brief 
 * 
 * @param c 
 */
void
compress_load_distance_huffman(compress *c)
{
    int count;
    count = compress_pop(c, 5);
    if (count == 0) {
        c->distance_huffman.default_value = compress_pop(c, 5);
    }
    else {
        int i;
        for (i = 0; i < count; i++) {
            c->distance_huffman.lengths[i] = compress_read_variable_length(c);
        }
    }
    huffman_build_table(&(c->distance_huffman));
}

/**
 * @brief 
 * 
 * @param c 
 */
void
compress_load_block(compress *c)
{
    c->block_elements = compress_pop(c, 16);
    compress_load_character_length_huffman(c);
    compress_load_character_huffman(c);
    compress_load_distance_huffman(c);
}

/**
 * @brief 
 * 
 * @param c 
 * @return int 
 */
int
compress_get_token(compress *c)
{
    int *h;
    if (c->block_elements <= 0) {
        compress_load_block(c);
    }
    c->block_elements--;
    h = huffman_lookup(c->character_huffman, compress_peek(c, 16));
    c->bit_position += h[1];
    return h[0];
}

/**
 * @brief 
 * 
 * @param c 
 * @return int 
 */
int
compress_get_position(compress *c)
{
    int *h, v;
    h = huffman_lookup(c->distance_huffman, compress_peek(c, 16));
    c->bit_position += h[1];
    if (h[0] == 0) {
        return 0;
    }
    v = h[0] - 1;
    v = (1<<v) + compress_pop(c, v);
    return v;
}

/**
 * @brief 
 * 
 * @param data 
 * @param length 
 * @param output 
 * @param output_length 
 * @return int 
 */
int
hus_decompress(char *data, int length, char *output, int *output_length)
{
    int character, i, j;
    compress *c = (compress*)malloc(sizeof(compress));
    c->bit_position = 0;
    c->input_data = data;
    c->input_length = length;
    c->bits_total = length*8;
    i = 0;
    while (c->bits_total > c->bit_position && i < *output_length) {
        /* process token */
        character = 0; /* fix this */
        if (character < 0x100) {
            output[i] = (char)character;
            i++;
        }
        else if (character == 510) {
            break;
        }
        else {
            length = character - 253;
            /* not sure about i here */
            c->bit_position = i - compress_get_position(c) - 1;
            for (j=c->bit_position; j < c->bit_position+length; j++) {
                output[i] = output[j];
                i++;
            }
        }
    }
    free(c);
    return 0;
}