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

#include "core.h"

/* The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

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

int selection_props[] = {
	ST_SELECTION_PICK_FIRST,
	ST_SELECTION_PICK_ADD,
	ST_SELECTION_PICK_DRAG,
	ST_SELECTION_COOLGRIP_COLOR,
	ST_SELECTION_HOTGRIP_COLOR,
	ST_SELECTION_GRIP_SIZE,
	ST_SELECTION_PICKBOX_SIZE,
	-1
};

int grid_ruler_props[] = {
	ST_GRID_ON_LOAD,
	ST_SHOW_ORIGIN,
	ST_MATCH_GRID_CROSSHAIR,
	ST_GRID_LOAD_FROM_FILE,
	ST_GRID_TYPE,
	ST_GRID_CENTER_ORIGIN,
	ST_GRID_CENTER_X,
	ST_GRID_CENTER_Y,
	ST_GRID_SIZE_X,
	ST_GRID_SIZE_Y,
	ST_GRID_SPACING_X,
	ST_GRID_SPACING_Y,
	-1
};

int accept_preview_props[] = {
    ST_MDI_USE_LOGO,
    ST_MDI_USE_TEXTURE,
    ST_MDI_USE_COLOR,
    ST_SHOW_SCROLLBARS,
    ST_SELECTBOX_ALPHA,
    ST_PROMPT_FONT_FAMILY,
    ST_PROMPT_FONT_STYLE,
    -1
};

int accept_accept_props[] = {
    ST_MDI_LOGO,
    ST_MDI_TEXTURE,
    ST_MDI_COLOR,
    ST_CROSSHAIR_COLOR,
    ST_BG_COLOR,
    ST_SELECTBOX_LEFT_COLOR,
    ST_SELECTBOX_LEFT_FILL,
    ST_SELECTBOX_RIGHT_COLOR,
    ST_SELECTBOX_RIGHT_FILL,
    ST_PROMPT_TEXT_COLOR,
    ST_PROMPT_BG_COLOR,
    ST_RULER_COLOR,
    ST_LWT_SHOW,
    ST_LWT_REAL,
    -1
};

const char *grid_type_visibility_lattice[] = {
	"labelGridSizeX",
	"spinBoxGridSizeX",
	"labelGridSizeY",
	"spinBoxGridSizeY",
	"labelGridSpacingX",
	"spinBoxGridSpacingX",
	"labelGridSpacingY",
	"spinBoxGridSpacingY",
	"END"
};

const char *grid_type_visibility_circular[] = {
	"labelGridSizeRadius",
	"spinBoxGridSizeRadius",
	"labelGridSpacingRadius",
	"spinBoxGridSpacingRadius",
	"labelGridSpacingAngle",
	"spinBoxGridSpacingAngle",
	"END"
};

const char *load_from_file_enabled[] = {
    "labelGridType",
    "comboBoxGridType",
    "checkBoxGridCenterOnOrigin",
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "labelGridSpacingX",
    "spinBoxGridSpacingX",
    "labelGridSpacingY",
    "spinBoxGridSpacingY",
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

WidgetData snap_point_data[] = {
    {
        .label = "Endpoint",
        .icon = "locator-snaptoendpoint",
        .setting = ST_QSNAP_ENDPOINT,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Midpoint",
        .icon = "locator-snaptomidpoint",
        .setting = ST_QSNAP_MIDPOINT,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Center",
        .icon = "locator-snaptocenter",
        .setting = ST_QSNAP_CENTER,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Node",
        .icon = "locator-snaptonode",
        .setting = ST_QSNAP_NODE,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Quadrant",
        .icon = "locator-snaptoquadrant",
        .setting = ST_QSNAP_QUADRANT,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Intersection",
        .icon = "locator-snaptointersection",
        .setting = ST_QSNAP_INTERSECTION,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Extension",
        .icon = "locator-snaptoextension",
        .setting = ST_QSNAP_EXTENSION,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Insertion",
        .icon = "locator-snaptoinsert",
        .setting = ST_QSNAP_INSERTION,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Perpendicular",
        .icon = "locator-snaptoperpendicular",
        .setting = ST_QSNAP_PERPENDICULAR,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Tangent",
        .icon = "locator-snaptotangent",
        .setting = ST_QSNAP_TANGENT,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Nearest",
        .icon = "locator-snaptonearest",
        .setting = ST_QSNAP_NEAREST,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Apparent Intersection",
        .icon = "locator-snaptoapparentintersection",
        .setting = ST_QSNAP_APPARENT,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Parallel",
        .icon = "locator-snaptoparallel",
        .setting = ST_QSNAP_PARALLEL,
        .type = EDITOR_CHECKBOX
    },
    {
        .type = -1
    }
};

WidgetData render_data[] = {
    {
        .label = "Use OpenGL",
        .icon = "blank",
        .setting = ST_USE_OPENGL,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Antialias",
        .icon = "blank",
        .setting = ST_ANTI_ALIAS,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Antialias Text",
        .icon = "blank",
        .setting = ST_TEXT_ANTI_ALIAS,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Smooth Pixmap",
        .icon = "blank",
        .setting = ST_SMOOTH_PIXMAP,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "High Quality Antialiasing (OpenGL)",
        .icon = "blank",
        .setting = ST_HQ_ANTI_ALIAS,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Non Cosmetic",
        .icon = "blank",
        .setting = ST_NON_COSMETIC,
        .type = EDITOR_CHECKBOX
    },
    {
        .type = -1
    }
};

WidgetData save_history_data[] = {
    {
        .label = "Save History",
        .icon = "blank",
        .setting = ST_SAVE_HISTORY,
        .type = EDITOR_CHECKBOX
    },
    {
        .label = "Save As HTML",
        .icon = "blank",
        .setting = ST_HTML_OUTPUT,
        .type = EDITOR_CHECKBOX
    },
    {
        .type = -1
    }
};

const char *tab_names[] = {
    "General",
    "Files/Paths",
    "Display",
    "Prompt",
    "Open/Save",
    "Printing",
    "Snap",
    "Grid/Ruler",
    "Ortho/Polar",
    "QuickSnap",
    "QuickTrack",
    "LineWeight",
    "Selection",
    "END"
};

/* This imitates an initiation file, and if none is present it is loaded
 * using the same parser.
 *
 * Our ini syntax is minimal: all lines with an equals sign '=' are parsed
 * comments are lines without the sign, everything to the left is the key,
 * everything to the right is the value.
 *
 * There are 3 types: int, float and string (char array). Boolean values are
 * treated as ints and arrays are strings with comma seperation.
 *
 * 1. If the first char of the value string is a number but not period is present
 *    then it is an int.
 * 2. If a period is present and it starts with a number then it is a float.
 * 3. Otherwise it is a string.
 * 4. Colors start with an open parenthesis, so we know that the string passed
 *    should then be converted into a color in the next loop.
 *
 *------------------------------------------------------------------------------
 *
 * Add a settings page entry to each row, allowing copying every entry on a tab
 * to dialog/config/accept_ as a loop.
 */
Setting settings_data[SETTINGS_TOTAL] = {
    {
        .id = ST_LANGUAGE,
        .key = "general_language",
        .value = "default",
        .type = 's'
    },
    {
        .id = ST_ICON_THEME,
        .key = "general_icon_theme",
        .value = "default",
        .type = 's'
    },
    {
        .id = ST_ICON_SIZE,
        .key = "general_icon_size",
        .value = "16",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_LOGO,
        .key = "general_mdi_bg_use_logo",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_TEXTURE,
        .key = "general_mdi_bg_use_texture",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_COLOR,
        .key = "general_mdi_bg_use_color",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_LOGO,
        .key = "general_mdi_bg_logo",
        .value = "images/logo-spirals.png",
        .type = 's'
    },
    {
        .id = ST_MDI_TEXTURE,
        .key = "general_mdi_bg_texture",
        .value = "images/texture-spirals.png",
        .type = 's'
    },
    {
        .id = ST_MDI_COLOR,
        .key = "general_mdi_bg_color",
        .value = "255",
        .type = 'i'
    },
    {
        .id = ST_TIP_OF_THE_DAY,
        .key = "general_tip_of_the_day",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_CURRENT_TIP,
        .key = "general_current_tip",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SYSTEM_HELP_BROWSER,
        .key = "general_system_help_browser",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_POS_X,
        .key = "window_position_x",
        .value = "100",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_POS_Y,
        .key = "window_position_y",
        .value = "100",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_SIZE_X,
        .key = "window_size_x",
        .value = "800",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_SIZE_Y,
        .key = "window_size_y",
        .value = "600",
        .type = 'i'
    },
    {
        .id = ST_USE_OPENGL,
        .key = "display_use_open_gl",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_ANTI_ALIAS,
        .key = "display_render_hint_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_ANTI_ALIAS,
        .key = "display_render_hint_text_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SMOOTH_PIXMAP,
        .key = "display_render_hint_smooth_pixmap",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_HQ_ANTI_ALIAS,
        .key = "display_render_hint_high_quality_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_NON_COSMETIC,
        .key = "display_render_hint_non_cosmetic",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SHOW_SCROLLBARS,
        .key = "display_show_scrollbars",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SCROLLBAR_WIDGET_NUM,
        .key = "display_scrollbar_widget_num",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_CROSSHAIR_COLOR,
        .key = "display_crosshair_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_BG_COLOR,
        .key = "display_background_color",
        .value = "16777215",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_LEFT_COLOR,
        .key = "display_selectbox_left_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_LEFT_FILL,
        .key = "display_selectbox_left_fill",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_RIGHT_COLOR,
        .key = "display_selectbox_right_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_RIGHT_FILL,
        .key = "display_selectbox_right_fill",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_ALPHA,
        .key = "display_selectbox_alpha",
        .value = "32",
        .type = 'i'
    },
    {
        .id = ST_ZOOMSCALE_IN,
        .key = "display_zoomscale_in",
        .value = "2.0",
        .type = 'r'
    },
    {
        .id = ST_ZOOMSCALE_OUT,
        .key = "display_zoomscale_out",
        .value = "0.5",
        .type = 'r'
    },
    {
        .id = ST_CROSSHAIR_PERCENT,
        .key = "display_crosshair_percent",
        .value = "5.0",
        .type = 'r'
    },
    {
        .id = ST_DISPLAY_UNITS,
        .key = "display_units",
        .value = "mm",
        .type = 's'
    },
    {
        .id = ST_PROMPT_TEXT_COLOR,
        .key = "prompt_text_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_BG_COLOR,
        .key = "prompt_background_color",
        .value = "16777215",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_FONT_FAMILY,
        .key = "prompt_font_family",
        .value = "Monospace",
        .type = 's'
    },
    {
        .id = ST_PROMPT_FONT_STYLE,
        .key = "prompt_font_style",
        .value = "normal",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_FONT_SIZE,
        .key = "prompt_font_size",
        .value = "12",
        .type = 'r'
    },
    {
        .id = ST_SAVE_HISTORY,
        .key = "prompt_save_history",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_HTML_OUTPUT,
        .key = "prompt_save_history_as_html",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_HISTORY_FILE,
        .key = "prompt_save_history_filename",
        .value = "prompt.log",
        .type = 's'
    },
    {
        .id = ST_OPENSAVE_FILTER,
        .key = "opensave_custom_filter",
        .value = "supported",
        .type = 's'
    },
    {
        .id = ST_OPENSAVE_FORMAT,
        .key = "opensave_open_format",
        .value = "*.*",
        .type = 's'
    },
    {
        .id = ST_OPEN_THUMBNAIL,
        .key = "opensave_open_thumbnail",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SAVE_FORMAT,
        .key = "opensave_save_format",
        .value = "*.*",
        .type = 's'
    },
    {
        .id = ST_SAVE_THUMBNAIL,
        .key = "opensave_save_thumbnail",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_RECENT_MAX,
        .key = "opensave_recent_max",
        .value = "10",
        .type = 'i'
    },
    {
        .id = ST_RECENT_FILES,
        .key = "opensave_recent_list_of_files",
        .value = "",
        .type = 's'
    },
    {
        .id = ST_RECENT_DIRECTORY,
        .key = "opensave_recent_directory",
        .value = "samples",
        .type = 's'
    },
    {
        .id = ST_TRIM_NUM_JUMPS,
        .key = "opensave_trim_dst_num_jumps",
        .value = "5",
        .type = 'i'
    },
    {
        .id = ST_DEFAULT_PRINTER,
        .key = "printing_default_device",
        .value = "",
        .type = 's'
    },
    {
        .id = ST_USE_LAST_PRINTER,
        .key = "printing_use_last_device",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_PRINT_DISABLE_BG,
        .key = "printing_disable_bg",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_ON_LOAD,
        .key = "grid_show_on_load",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SHOW_ORIGIN,
        .key = "grid_show_origin",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MATCH_GRID_CROSSHAIR,
        .key = "grid_color_match_crosshair",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_COLOR,
        .key = "grid_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_GRID_LOAD_FROM_FILE,
        .key = "grid_load_from_file",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_TYPE,
        .key = "grid_type",
        .value = "Rectangular",
        .type = 's'
    },
    {
        .id = ST_GRID_CENTER_ORIGIN,
        .key = "grid_center_on_origin",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_CENTER_X,
        .key = "grid_center_x",
        .value = "0.0",
        .type = 'i'
    },
    {
        .id = ST_GRID_CENTER_Y,
        .key = "grid_center_y",
        .value = "0.0",
        .type = 'i'
    },
    {
        .id = ST_GRID_SIZE_X,
        .key = "grid_size_x",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SIZE_Y,
        .key = "grid_size_y",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_X,
        .key = "grid_spacing_x",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_Y,
        .key = "grid_spacing_y",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SIZE_RADIUS,
        .key = "grid_size_radius",
        .value = "50.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_RADIUS,
        .key = "grid_spacing_radius",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_ANGLE,
        .key = "grid_spacing_angle",
        .value = "45.0",
        .type = 'r'
    },
    {
        .id = ST_RULER_ON_LOAD,
        .key = "ruler_show_on_load",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_RULER_METRIC,
        .key = "ruler_metric",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_RULER_COLOR,
        .key = "ruler_color",
        .value = "14479360",
        .type = 'i'
    },
    {
        .id = ST_RULER_SIZE,
        .key = "ruler_pixel_size",
        .value = "20",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_ENABLED,
        .key = "quicksnap_enabled",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_LOCATOR_COLOR,
        .key = "quicksnap_locator_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_LOCATOR_SIZE,
        .key = "quicksnap_locator_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_APERTURE_SIZE,
        .key = "quicksnap_aperture_size",
        .value = "10",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_ENDPOINT,
        .key = "quicksnap_endpoint",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_MIDPOINT,
        .key = "quicksnap_midpoint",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_CENTER,
        .key = "quicksnap_center",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_NODE,
        .key = "quicksnap_node",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_QUADRANT,
        .key = "quicksnap_quadrant",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_INTERSECTION,
        .key = "quicksnap_intersection",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_EXTENSION,
        .key = "quicksnap_extension",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_INSERTION,
        .key = "quicksnap_insertion",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_PERPENDICULAR,
        .key = "quicksnap_perpendicular",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_TANGENT,
        .key = "quicksnap_tangent",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_NEAREST,
        .key = "quicksnap_nearest",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_APPARENT,
        .key = "quicksnap_apparent",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_PARALLEL,
        .key = "quicksnap_parallel",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_LWT_SHOW,
        .key = "lineweight_show_line_weight",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_LWT_REAL,
        .key = "lineweight_real_render",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_LWT_DEFAULT,
        .key = "lineweight_default_line_weight",
        .value = "0.0",
        .type = 'r'
    },
    {
        .id = ST_SELECTION_PICK_FIRST,
        .key = "selection_pick_first",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICK_ADD,
        .key = "selection_pick_add",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICK_DRAG,
        .key = "selection_pick_drag",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_COOLGRIP_COLOR,
        .key = "selection_coolgrip_color",
        .value = "255",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_HOTGRIP_COLOR,
        .key = "selection_hotgrip_color",
        .value = "25500",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_GRIP_SIZE,
        .key = "selection_grip_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICKBOX_SIZE,
        .key = "selection_pickbox_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_TEXT_FONT,
        .key = "text_font",
        .value = "Arial",
        .type = 's'
    },
    {
        .id = ST_TEXT_SIZE,
        .key = "text_size",
        .value = "12.0",
        .type = 'r'
    },
    {
        .id = ST_TEXT_ANGLE,
        .key = "text_angle",
        .value = "0.0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_BOLD,
        .key = "text_style_bold",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_ITALIC,
        .key = "text_style_italic",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_UNDERLINE,
        .key = "text_style_underline",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_STRIKEOUT,
        .key = "text_style_strikeout",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_OVERLINE,
        .key = "text_style_overline",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TICK_DEPTH,
        .key = "ruler_tick_depth",
        .value = "0.5",
        .type = 'r'
    },
    {
        .id = ST_MAJOR_TICK_SEPERATION,
        .key = "major_tick_seperation",
        .value = "0.4",
        .type = 'r'
    },
    {
        .id = ST_NEEDLE_SPEED,
        .key = "needle_speed",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_STITCH_TIME,
        .key = "stitch_time",
        .value = "0.01",
        .type = 'r'
    }
};
