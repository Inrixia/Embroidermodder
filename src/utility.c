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
 *  These functions use only standard types and therefore requires
 *  no non-standard headers. As such it doesn't need to recompile when the
 *  rest of the source changes and vice-versa.
 *
 *  Also they don't call each other, so we don't need to include the
 *  function declarations.
 */

#include <string.h>

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
