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
 */

#include "sizes.h"
#include "actions.h"
#include "menus.h"

const char *menu_labels[] = {
	"&File",
	"&Edit",
	"&View",
	"&Settings",
	"&Window",
	"&Help",
	"&Draw",
	"Open &Recent",
	"&Zoom",
	"&Pan"
};

/*  . */
int32_t menubar_order[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_VIEW,
    MENU_DRAW,
    MENU_SETTINGS,
    MENU_WINDOW,
    MENU_HELP,
    -1
};

MenuData menu_data[MAX_MENUS];

int32_t file_menu[] = {
    ACTION_NEW,
    MENU_SEPERATOR,
    ACTION_OPEN,
    MENU_SUBMENU, MENU_RECENT,
    MENU_SEPERATOR,
    ACTION_SAVE,
    ACTION_SAVEAS,
    MENU_SEPERATOR,
    ACTION_PRINT,
    MENU_SEPERATOR,
    ACTION_WINDOW_CLOSE,
    MENU_SEPERATOR,
    ACTION_DESIGN_DETAILS,
    MENU_SEPERATOR,
    ACTION_EXIT,
    MENU_END
};

int32_t edit_menu[] = {
    ACTION_UNDO,
    ACTION_REDO,
    MENU_SEPERATOR,
    ACTION_CUT,
    ACTION_COPY,
    ACTION_PASTE,
    MENU_SEPERATOR,
    MENU_END
};

int32_t pan_menu[] = {
    MENU_ICON,
    ACTION_PAN,
    ACTION_PAN_REAL_TIME,
    ACTION_PAN_POINT,
    MENU_SEPERATOR,
    ACTION_PAN_LEFT,
    ACTION_PAN_RIGHT,
    ACTION_PAN_UP,
    ACTION_PAN_DOWN,
    MENU_END
};

int32_t zoom_menu[] = {
    MENU_ICON,
    ACTION_ZOOM,
    ACTION_ZOOM_REAL_TIME,
    ACTION_ZOOM_PREVIOUS,
    MENU_SEPERATOR,
    ACTION_ZOOM_WINDOW,
    ACTION_ZOOM_DYNAMIC,
    ACTION_ZOOM_SCALE,
    ACTION_ZOOM_CENTER,
    MENU_SEPERATOR,
    ACTION_ZOOM_IN,
    ACTION_ZOOM_OUT,
    MENU_SEPERATOR,
    ACTION_ZOOM_SELECTED,
    ACTION_ZOOM_ALL,
    ACTION_ZOOM_EXTENTS,
    MENU_END
};

int32_t view_menu[] = {
    MENU_SEPERATOR,
    MENU_SUBMENU, MENU_ZOOM,
    MENU_SUBMENU, MENU_PAN,
    MENU_SEPERATOR,
    ACTION_DAY,
    ACTION_NIGHT,
    MENU_SEPERATOR,
    MENU_END
};

int32_t settings_menu[] = {
    ACTION_SETTINGS_DIALOG,
    MENU_SEPERATOR,
    ACTION_TEST,
    MENU_END
};

int32_t window_menu[] = {
    MENU_SEPERATOR,
    MENU_END
};

int32_t help_menu[] = {
    ACTION_HELP,
    MENU_SEPERATOR,
    ACTION_CHANGELOG,
    MENU_SEPERATOR,
    ACTION_TIP_OF_THE_DAY,
    MENU_SEPERATOR,
    ACTION_ABOUT,
    MENU_SEPERATOR,
    ACTION_WHATS_THIS,
    MENU_END
};

int32_t draw_menu[] = {
    ACTION_ADD_CIRCLE,
    ACTION_ADD_PATH,
    ACTION_ADD_POLYGON,
    ACTION_ADD_POLYLINE,
    ACTION_ADD_POINT,
    ACTION_ADD_HEART,
    ACTION_ADD_SINGLE_LINE_TEXT,
    MENU_END
};
