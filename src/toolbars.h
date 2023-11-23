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

#ifndef _EMBROIDERMODDER_TOOLBARS_
#define _EMBROIDERMODDER_TOOLBARS_

#include "sizes.h"
#include "actions.h"

/* Toolbars */
#define TOOLBAR_SEPERATOR                       -2
#define TOOLBAR_END                             -1
#define TOOLBAR_FILE                             0
#define TOOLBAR_EDIT                             1
#define TOOLBAR_PAN                              2
#define TOOLBAR_ZOOM                             3
#define TOOLBAR_VIEW                             4
#define TOOLBAR_SETTINGS                         5
#define TOOLBAR_WINDOW                           6
#define TOOLBAR_HELP                             7
#define TOOLBAR_DRAW                             8
#define TOOLBAR_ICON                             9
#define TOOLBAR_LAYER                           10
#define TOOLBAR_PROPERTIES                      11
#define TOOLBAR_TEXT                            12
#define TOOLBAR_PROMPT                          13
#define TOTAL_TOOLBARS                          14

#ifdef __cplusplus
extern "C" {
#endif

/*
 */
typedef struct ToolbarData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
    char horizontal;
} ToolbarData;

/* Toolbar data */
extern ToolbarData toolbar_data[MAX_TOOLBARS];
extern int32_t top_toolbar_layout[];
extern int32_t bottom_toolbar_layout[];
extern int32_t side_toolbar_layout[];

#ifdef __cplusplus
}
#endif

#endif
