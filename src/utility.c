/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 *
 *  See the "Programming principles for the C core" in the reference manual
 *  before changing this file.
 *
 *  These functions use only libembroidery and standard types and therefore requires
 *  no embroidermodder headers. As such it doesn't need to recompile when the
 *  rest of the source changes and vice-versa.
 *
 *  Also they don't call each other, so we don't need to include the
 *  function declarations.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>

#include "../extern/libembroidery/src/embroidery.h"

const char *version = "2.0.0-alpha4";

const char *details_labels[] = {
    "Total Stitches:",
    "Real Stitches:",
    "Jump Stitches:",
    "Trim Stitches:",
    "Total Colors:",
    "Color Changes:",
    "Left:",
    "Top:",
    "Right:",
    "Bottom:",
    "Width:",
    "Height:",
    "END"
};

const char *default_prompt_style[] = {
    "color",                      "#000000", // Match -------|
    "background-color",           "#FFFFFF", //              |
    "selection-color",            "#FFFFFF", //              |
    "selection-background-color", "#000000", // Match -------|
    "font-family",              "Monospace",
    "font-style",                  "normal",
    "font-size",                     "12px"
    "END"
};

const char *usage_msg = ""
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ \n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|\n"
    " _____________________________________________________________________________ \n"
    "|                                                                             |\n"
    "|                     https://www.libembroidery.org                           |\n"
    "|_____________________________________________________________________________|\n"
    "\n"
    "Usage: embroidermodder [options] files ...\n"
    "\n"
    "Options:\n"
    "  -d, --debug      Print lots of debugging information.\n"
    "  -h, --help       Print this message and exit.\n"
    "  -v, --version    Print the version number of embroidermodder and exit.\n"
    "\n";

/* TODO: "Aligned"
 * TODO: "Fit"
 */
const char *justify_options[] = {
    "Left",
    "Center",
    "Right",
    "Aligned",
    "Middle",
    "Fit",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    "END"
};

/* . */
const char *button_list[] = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT",
    "END"
};


/* File name extensions for embroidery formats with any level of support. */
const char *extensions[] = {
    "100",
    "10o",
    "ART",
    "BMC",
    "BRO",
    "CND",
    "COL",
    "CSD",
    "CSV",
    "DAT",
    "DEM",
    "DSB",
    "DST",
    "DSZ",
    "DXF",
    "EDR",
    "EMD",
    "EXP",
    "EXY",
    "EYS",
    "FXY",
    "GNC",
    "GT",
    "HUS",
    "INB",
    "JEF",
    "KSM",
    "PCD",
    "PCM",
    "PCQ",
    "PCS",
    "PEC",
    "PEL",
    "PEM",
    "PES",
    "PHB",
    "PHC",
    "RGB",
    "SEW",
    "SHV",
    "SST",
    "STX",
    "SVG",
    "T09",
    "TAP",
    "THR",
    "TXT",
    "U00",
    "U01",
    "VIP",
    "VP3",
    "XXX",
    "ZSK",
    "END"
};

/* Tokenize our command using a 1 character deliminator. */
int
tokenize(char **argv, char *str, const char delim)
{
    int argc = 0;
    argv[argc] = 0;
    if (strlen(str) == 0) {
        return 0;
    }
    for (int i=0; str[i]; i++) {
        if (str[i] == delim) {
            str[i] = 0;
            argc++;
            argv[argc] = str+i;
        }
    }
    return argc;
}


/* Check that RBG values are in the range (0,255) inclusive. */
unsigned char
validRGB(int r, int g, int b)
{
    unsigned char result = (r>=0);
    result &= (r<256);
    result &= (g>=0);
    result &= (g<256);
    result &= (b>=0);
    result &= (b<256);
    return result;
}

/*
 * Returns the location of the first occurence of c in s if present. Otherwise
 * return -1.
 */
int
str_contains(char *s, char c)
{
    int i;
    for (i=0; s[i]; i++) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}

/* . */
int
string_array_length(const char *list[])
{
    int i = 0;
    for (i=0; strcmp(list[i], "END"); i++) {}
    return i;
}

/* Fourier series for parametric plotting. */
EmbReal
fourier_series(EmbReal arg, EmbReal *terms, int n_terms)
{
    EmbReal x = 0.0f;
    for (int i=0; i<(n_terms/3); i++) {
        x += terms[3*i+0] * sin(terms[3*i+1] + terms[3*i+2] * arg);
    }
    return x;
}

/* . */
uint8_t
willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return (c < INT_MIN || c > INT_MAX);
}

/* . */
uint8_t
willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return (c < INT_MIN || c > INT_MAX);
}

/* Round the number "numToRound" to a multple of the number "multiple",
 * rounding up if "roundUp" is true.
 *
 * First, "multiple" is 0 then we have an invalid input so just return the
 * argument, then if the number is already a multiple of \a multiple then
 * return the argument.
 *
 * Then take the remainder off the argument and determine which way to round
 * the result.
 */
int
roundToMultiple(uint8_t roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    int result = numToRound - remainder;
    if (roundUp) {
        if (numToRound < 0) {
            return result;
        }
        return result + multiple;
    }
    /* else round down */
    if (numToRound < 0) {
        return result - multiple;
    }
    return result;
}
