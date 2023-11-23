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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

#include "core.h"

/* See the "Programming principles for the C core" in the reference manual
 * before changing this file.
 */

const char *version = "2.0.0-alpha4";

/* The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

/* . */
const char *object_names[] = {
    "Base",
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arclength Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Multiline Text",
    "Text",
    "Unknown",
    "END"
};

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

/* . */
const char rectangle_main_script[][MAX_STRING_LENGTH] = {
    "init",
    "clear-selection",
    "newRect = true",
    "real x1 = 0.0f",
    "real y1 = 0.0f",
    "real x2 = 0.0f",
    "real y2 = 0.0f",
    "set-prompt-prefix-tr Specify first corner point or [Chamfer/Fillet]: "
};

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

/* . */
const char *alias_table[] = {
    "exit", "quit",
    "u", "undo",
    "close", "windowclose",
    "closeall", "windowcloseall",
    "cascade", "windowcascade"
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

const char *group_box_data[] = {
    "general", "General",
    "geometry_arc", "Geometry",
    "misc_arc", "Misc",
    "geometry_block", "Geometry",
    "geometry_circle", "Geometry",
    "geometry_dim_aligned", "Geometry",
    "geometry_dim_angular", "Geometry",
    "geometry_dim_arc_length", "Geometry",
    "geometry_dim_diameter", "Geometry",
    "geometry_dim_leader", "Geometry",
    "geometry_dim_linear", "Geometry",
    "geometry_dim_ordinate", "Geometry",
    "geometry_dim_radius", "Geometry",
    "geometry_ellipse", "Geometry",
    "geometry_image", "Geometry",
    "misc_image", "Misc",
    "geometry_infinite_line", "Geometry",
    "geometry_line", "Geometry",
    "geometry_path", "Geometry",
    "misc_path", "Misc",
    "geometry_point", "Geometry",
    "geometry_polygon", "Geometry",
    "geometry_polyline", "Geometry",
    "misc_polyline", "Misc",
    "geometry_ray", "Geometry",
    "geometry_rectangle", "Geometry",
    "geometry_text_multi", "Geometry",
    "text_text_single", "Text",
    "geometry_text_single", "Geometry",
    "misc_text_single", "Misc",
    "END", "END"
};

const int32_t group_box_ids[] = {
    OBJ_TYPE_NULL,
    OBJ_TYPE_ARC,
    OBJ_TYPE_ARC,
    OBJ_TYPE_BLOCK,
    OBJ_TYPE_CIRCLE,
    OBJ_TYPE_DIMALIGNED,
    OBJ_TYPE_DIMANGULAR,
    OBJ_TYPE_DIMARCLENGTH,
    OBJ_TYPE_DIMDIAMETER,
    OBJ_TYPE_DIMLEADER,
    OBJ_TYPE_DIMLINEAR,
    OBJ_TYPE_DIMORDINATE,
    OBJ_TYPE_DIMRADIUS,
    OBJ_TYPE_ELLIPSE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_INFINITELINE,
    OBJ_TYPE_LINE,
    OBJ_TYPE_PATH,
    OBJ_TYPE_PATH,
    OBJ_TYPE_POINT,
    OBJ_TYPE_POLYGON,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_RAY,
    OBJ_TYPE_RECTANGLE,
    OBJ_TYPE_TEXTMULTI,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE
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

char rubber_modes[N_RUBBER_MODES][MAX_STRING_LENGTH] = {
    "CIRCLE_1P_RAD",
    "CIRCLE_1P_DIA",
    "CIRCLE_2P",
    "CIRCLE_3P",
    "CIRCLE_TTR",
    "CIRCLE_TTT",
    "DIMLEADER_LINE",
    "ELLIPSE_LINE",
    "ELLIPSE_MAJORDIAMETER_MINORRADIUS",
    "ELLIPSE_MAJORRADIUS_MINORRADIUS",
    "ELLIPSE_ROTATION",
    "LINE",
    "POLYGON",
    "POLYGON_INSCRIBE",
    "POLYGON_CIRCUMSCRIBE",
    "POLYLINE",
    "RECTANGLE",
    "TEXTSINGLE",
    "END"
};

const char *tips[] = {
    "we need more tips?",
    "you can change the color of the display through settings?",
    "you can hide the scrollbars to increase the viewable area through settings?",
    "you can change the icon size for increased visibility?",
    "you can toggle the grid on and off by pressing the button in the statusbar?",
    "the grid size can be changed to match your hoop size through settings?",
    "the crosshair size is based on a percentage of your screen size? Setting it to 100 may help you visually line things up better.",
    "you can pan by pressing the middle mouse button and dragging your mouse across the screen?",
    "you can open and edit multiple designs simultaneously?",
    "you that many embroidery machines support the .dst format?",
    "that you can zoom in and out using your mouse wheel?",
    "that you can use circular and isometric grids?",
    "about our command line format converter?",
    "that you can use the 'DAY' and 'NIGHT' commands to quickly switch the view colors to commonly used white or black?",
    "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?",
    "END"
};

const char types[][MAX_STRING_LENGTH] = {
    "NULL",
    "STRING",
    "STRING_LIST",
    "REAL",
    "INT",
    "BOOL",
    "FUNCTION",
    "VECTOR",
    "UNKNOWN"
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

int general_props[] = {
    ST_ICON_THEME,
    ST_ICON_SIZE,
    ST_MDI_USE_COLOR,
    ST_MDI_USE_LOGO,
    ST_MDI_USE_TEXTURE,
    ST_MDI_COLOR,
    ST_MDI_LOGO,
    ST_MDI_TEXTURE,
    ST_TIP_OF_THE_DAY,
    -1
};

int display_props[] = {
    ST_USE_OPENGL,
    ST_ANTI_ALIAS,
    ST_TEXT_ANTI_ALIAS,
    ST_SMOOTH_PIXMAP,
    ST_HQ_ANTI_ALIAS,
    ST_NON_COSMETIC,
    ST_SHOW_SCROLLBARS,
    ST_SCROLLBAR_WIDGET_NUM,
    ST_CROSSHAIR_COLOR,
    ST_BG_COLOR,
    ST_SELECTBOX_LEFT_COLOR,
    ST_SELECTBOX_LEFT_FILL,
    ST_SELECTBOX_RIGHT_COLOR,
    ST_SELECTBOX_RIGHT_FILL,
    ST_SELECTBOX_ALPHA,
    ST_ZOOMSCALE_IN,
    ST_ZOOMSCALE_OUT,
    -1
};

int prompt_props[] = {
    ST_PROMPT_TEXT_COLOR,
    ST_PROMPT_BG_COLOR,
    ST_PROMPT_FONT_FAMILY,
    ST_PROMPT_FONT_SIZE,
    ST_SAVE_HISTORY,
    ST_HTML_OUTPUT,
    -1
};

int quick_snap_props[] = {
    ST_QSNAP_ENABLED,
    ST_QSNAP_LOCATOR_COLOR,
    ST_QSNAP_LOCATOR_SIZE,
    ST_QSNAP_APERTURE_SIZE,
    ST_QSNAP_ENDPOINT,
    ST_QSNAP_MIDPOINT,
    ST_QSNAP_CENTER,
    ST_QSNAP_NODE,
    ST_QSNAP_QUADRANT,
    ST_QSNAP_INTERSECTION,
    ST_QSNAP_EXTENSION,
    ST_QSNAP_INSERTION,
    ST_QSNAP_PERPENDICULAR,
    ST_QSNAP_TANGENT,
    ST_QSNAP_NEAREST,
    ST_QSNAP_APPARENT,
    ST_QSNAP_PARALLEL,
    -1
};

int opensave_props[] = {
    ST_OPENSAVE_FILTER,
    -1
};


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
