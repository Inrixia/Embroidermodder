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

#include "toolbars.h"

ToolbarData toolbar_data[MAX_TOOLBARS] = {
    {
        .id = TOOLBAR_FILE,
        .key = "File",
        .entries = {
            ACTION_NEW,
            ACTION_OPEN,
            ACTION_SAVE,
            ACTION_SAVEAS,
            ACTION_PRINT,
            ACTION_DESIGN_DETAILS,
            TOOLBAR_SEPERATOR,
            ACTION_UNDO,
            ACTION_REDO,
            TOOLBAR_SEPERATOR,
            ACTION_HELP,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_EDIT,
        .key = "Edit",
        .entries = {
            ACTION_CUT,
            ACTION_COPY,
            ACTION_PASTE,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PAN,
        .key = "Pan",
        .entries = {
            ACTION_PAN_REAL_TIME,
            ACTION_PAN_POINT,
            TOOLBAR_SEPERATOR,
            ACTION_PAN_LEFT,
            ACTION_PAN_RIGHT,
            ACTION_PAN_UP,
            ACTION_PAN_DOWN,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_ZOOM,
        .key = "Zoom",
        .entries = {
            ACTION_ZOOM_WINDOW,
            ACTION_ZOOM_DYNAMIC,
            ACTION_ZOOM_SCALE,
            TOOLBAR_SEPERATOR,
            ACTION_ZOOM_CENTER,
            ACTION_ZOOM_IN,
            ACTION_ZOOM_OUT,
            TOOLBAR_SEPERATOR,
            ACTION_ZOOM_SELECTED,
            ACTION_ZOOM_ALL,
            ACTION_ZOOM_EXTENTS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_VIEW,
        .key = "View",
        .entries = {
            ACTION_DAY,
            ACTION_NIGHT,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_SETTINGS,
        .key = "Settings",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_WINDOW,
        .key = "Window",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_HELP,
        .key = "Help",
        .entries = {
            ACTION_HELP,
            TOOLBAR_SEPERATOR,
            ACTION_CHANGELOG,
            TOOLBAR_SEPERATOR,
            ACTION_ABOUT,
            TOOLBAR_SEPERATOR,
            ACTION_WHATS_THIS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_DRAW,
        .key = "Draw",
        .entries = {
            ACTION_ADD_CIRCLE,
            ACTION_ADD_PATH,
            ACTION_ADD_POLYGON,
            ACTION_ADD_POLYLINE,
            ACTION_ADD_POINT,
            ACTION_ADD_HEART,
            ACTION_ADD_SINGLE_LINE_TEXT,
            TOOLBAR_END
        },
        .horizontal = 0
    },
    {
        .id = TOOLBAR_ICON,
        .key = "Icon",
        .entries = {
            ACTION_ICON16,
            ACTION_ICON24,
            ACTION_ICON32,
            ACTION_ICON48,
            ACTION_ICON64,
            ACTION_ICON128,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_LAYER,
        .key = "Layer",
        .entries = {
            ACTION_MAKE_LAYER_CURRENT,
            ACTION_LAYER_EDITOR,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PROPERTIES,
        .key = "Properties",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_TEXT,
        .key = "Text",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PROMPT,
        .key = "Command Prompt",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    }
};

/*  . */
int32_t top_toolbar_layout[] = {
    TOOLBAR_SEPERATOR,
    TOOLBAR_FILE,
    TOOLBAR_EDIT,
    TOOLBAR_HELP,
    TOOLBAR_ICON,
    TOOLBAR_SEPERATOR,
    TOOLBAR_ZOOM,
    TOOLBAR_PAN,
    TOOLBAR_VIEW,
    TOOLBAR_SEPERATOR,
    TOOLBAR_LAYER,
    TOOLBAR_PROPERTIES,
    TOOLBAR_SEPERATOR,
    TOOLBAR_TEXT,
    TOOLBAR_END
};

/*  . */
int32_t bottom_toolbar_layout[] = {
    TOOLBAR_PROMPT,
    TOOLBAR_END
};

/*  . */
int32_t side_toolbar_layout[] = {
    TOOLBAR_DRAW,
    TOOLBAR_END
};
