/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2023 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *     https://peps.python.org/pep-0007/
 *
 * ------------------------------------------------------------
 */

#ifndef __EMBROIDERMODDER_MENUS__
#define __EMBROIDERMODDER_MENUS__

#include "sizes.h"
#include "actions.h"

#define MENU_ICON                               -4
#define MENU_SUBMENU                            -3
#define MENU_SEPERATOR                          -2
#define MENU_END                                -1
#define MENU_FILE                                0
#define MENU_EDIT                                1
#define MENU_VIEW                                2
#define MENU_SETTINGS                            3
#define MENU_WINDOW                              4
#define MENU_HELP                                5
#define MENU_DRAW                                6
#define MENU_RECENT                              7
#define MENU_ZOOM                                8
#define MENU_PAN                                 9
#define TOTAL_MENUS                             10

#ifdef __cplusplus
extern "C" {
#endif

/*
 */
typedef struct MenuData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
} MenuData;

/* Menus data */
extern MenuData menu_data[MAX_MENUS];
extern int32_t menubar_order[];
extern int32_t file_menu[];
extern int32_t edit_menu[];
extern int32_t pan_menu[];
extern int32_t zoom_menu[];
extern int32_t view_menu[];
extern int32_t settings_menu[];
extern int32_t window_menu[];
extern int32_t help_menu[];
extern int32_t draw_menu[];
extern const char *menu_labels[];

#ifdef __cplusplus
}
#endif

#endif
