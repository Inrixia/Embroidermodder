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
 */

/* Used for debugging, this script should not crash the program. If it manages
 * not to then you can check gcov/lcov for what lines we managed to cover with
 * this test.
 */ 
char *coverage_test_script[] = {
    "new",
    "icon 16",
    "icon 32",
    "icon 64",
    "icon 128",
    "icon 24",
    "zoom in",
    "zoom extents",
    "pan up",
    "pan down",
    "pan right",
    "pan left",
    "quit"
};
