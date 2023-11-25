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
 *
 * Since we use a lot of switch tables and a generally
 * procedural style, most of these constants are for identifying
 * our internal types. More general constants belong
 * in "embroidery.h" which does all the grunt work for our geometry.
 */

#ifndef __EMBROIDERMODDER_CONSTANTS__
#define __EMBROIDERMODDER_CONSTANTS__

/* OBJ_LTYPE_VALUES */
// CAD Linetypes
#define OBJ_LTYPE_CONT                          0
#define OBJ_LTYPE_CENTER                        1
#define OBJ_LTYPE_DOT                           2
#define OBJ_LTYPE_HIDDEN                        3
#define OBJ_LTYPE_PHANTOM                       4
#define OBJ_LTYPE_ZIGZAG                        5
/* Embroidery Stitchtypes */
#define OBJ_LTYPE_RUNNING                       6    // __________
#define OBJ_LTYPE_SATIN                         7    // vvvvvvvvvv
#define OBJ_LTYPE_FISHBONE                      8    // >>>>>>>>>>

/* OBJ_LWT_VALUES */
#define OBJ_LWT_BYLAYER                        -2
#define OBJ_LWT_BYBLOCK                        -1
#define OBJ_LWT_DEFAULT                         0
#define OBJ_LWT_01                              1
#define OBJ_LWT_02                              2
#define OBJ_LWT_03                              3
#define OBJ_LWT_04                              4
#define OBJ_LWT_05                              5
#define OBJ_LWT_06                              6
#define OBJ_LWT_07                              7
#define OBJ_LWT_08                              8
#define OBJ_LWT_09                              9
#define OBJ_LWT_10                             10
#define OBJ_LWT_11                             11
#define OBJ_LWT_12                             12
#define OBJ_LWT_13                             13
#define OBJ_LWT_14                             14
#define OBJ_LWT_15                             15
#define OBJ_LWT_16                             16
#define OBJ_LWT_17                             17
#define OBJ_LWT_18                             18
#define OBJ_LWT_19                             19
#define OBJ_LWT_20                             20
#define OBJ_LWT_21                             21
#define OBJ_LWT_22                             22
#define OBJ_LWT_23                             23
#define OBJ_LWT_24                             24


/* OBJ_SNAP_VALUES */
/* NOTE: Allow this enum to evaluate false. */
#define OBJ_SNAP_NULL                            0
#define OBJ_SNAP_ENDPOINT                        1
#define OBJ_SNAP_MIDPOINT                        2
#define OBJ_SNAP_CENTER                          3
#define OBJ_SNAP_NODE                            4
#define OBJ_SNAP_QUADRANT                        5
#define OBJ_SNAP_INTERSECTION                    6
#define OBJ_SNAP_EXTENSION                       7
#define OBJ_SNAP_INSERTION                       8
#define OBJ_SNAP_PERPENDICULAR                   9
#define OBJ_SNAP_TANGENT                        10
#define OBJ_SNAP_NEAREST                        11
#define OBJ_SNAP_APPINTERSECTION                12
#define OBJ_SNAP_PARALLEL                       13

/* Allow OBJ_TYPE to evaluate false.
 * Values >= 65536 ensure compatibility with qgraphicsitem_cast().
 *
 * For SVG we need: Block.
 * For DXF we meed: Aligned Dimension, Angular Dimension, Arc Length Dimension,
 *     Linear Dimension, Ordinate Dimension, Radial Dimension and Infinite Line
 *     object.
 *
 * The objects which should be removed from stitch output as they exist for
 * drafting reasons are all the ...
 *
 * Ray?
 */
#define OBJ_TYPE_NULL                            0
#define OBJ_TYPE_BASE                       100000
#define OBJ_TYPE_ARC                        100001
#define OBJ_TYPE_BLOCK                      100002
#define OBJ_TYPE_CIRCLE                     100003
#define OBJ_TYPE_DIMALIGNED                 100004
#define OBJ_TYPE_DIMANGULAR                 100005
#define OBJ_TYPE_DIMARCLENGTH               100006
#define OBJ_TYPE_DIMDIAMETER                100007
#define OBJ_TYPE_DIMLEADER                  100008
#define OBJ_TYPE_DIMLINEAR                  100009
#define OBJ_TYPE_DIMORDINATE                100010
#define OBJ_TYPE_DIMRADIUS                  100011
#define OBJ_TYPE_ELLIPSE                    100012
#define OBJ_TYPE_ELLIPSEARC                 100013
#define OBJ_TYPE_RUBBER                     100014
#define OBJ_TYPE_GRID                       100015
#define OBJ_TYPE_HATCH                      100016
#define OBJ_TYPE_IMAGE                      100017
#define OBJ_TYPE_INFINITELINE               100018
#define OBJ_TYPE_LINE                       100019
#define OBJ_TYPE_PATH                       100020
#define OBJ_TYPE_POINT                      100021
#define OBJ_TYPE_POLYGON                    100022
#define OBJ_TYPE_POLYLINE                   100023
#define OBJ_TYPE_RAY                        100024
#define OBJ_TYPE_RECTANGLE                  100025
#define OBJ_TYPE_SLOT                       100026
#define OBJ_TYPE_SPLINE                     100027
#define OBJ_TYPE_TEXTMULTI                  100028
#define OBJ_TYPE_TEXTSINGLE                 100029
#define OBJ_TYPE_UNKNOWN                    100030

/*
 * Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 *
 * OBJ_KEYS
 *
 * TYPE value type - int: See OBJ_TYPE_VALUES
 * NAME value type - str: See OBJ_NAME_VALUES
 * LAYER value type - str: "USER", "DEFINED", "STRINGS", etc...
 * COLOR value type - int: 0-255
 *     TODO: Use color chart in formats/format-dxf.h for this.
 * LTYPE value type - int: See OBJ_LTYPE_VALUES
 * LWT value type - int: 0-27
 * RUBBER value type - int: See OBJ_RUBBER_VALUES
 */
#define OBJ_TYPE                                 0
#define OBJ_NAME                                 1
#define OBJ_LAYER                                2
#define OBJ_COLOR                                3
#define OBJ_LTYPE                                4
#define OBJ_LWT                                  5
#define OBJ_RUBBER                               6

/* Arrow Styles */
/* NOTE: Allow this enum to evaluate false. */
#define ARROW_STYLE_NONE                         0
#define ARROW_STYLE_OPEN                         1
#define ARROW_STYLE_CLOSED                       2
#define ARROW_STYLE_DOT                          3
#define ARROW_STYLE_BOX                          4
#define ARROW_STYLE_TICK                         5

/* Line Styles */
/* NOTE: Allow this enum to evaluate false. */
#define LINE_STYLE_NONE                          0
#define LINE_STYLE_FLARED                        1
#define LINE_STYLE_FLETCHING                     2

/* UI Editing Modes */

/* Polygon editing modes. */
#define POLYGON_MODE_NUM_SIDES                   0
#define POLYGON_MODE_CENTER_PT                   1
#define POLYGON_MODE_POLYTYPE                    2
#define POLYGON_MODE_INSCRIBE                    3
#define POLYGON_MODE_CIRCUMSCRIBE                4
#define POLYGON_MODE_DISTANCE                    5
#define POLYGON_MODE_SIDE_LEN                    6

/* Point identifiers. */
#define VECTOR_ARC_START_POINT                   0
#define VECTOR_ARC_MID_POINT                     1
#define VECTOR_ARC_END_POINT                     2
#define VECTOR_ARC_POSITION                      3

/* Real Identifiers */
#define REAL_ARC_RADIUS                          0
#define REAL_ARC_START_ANGLE                     1
#define REAL_ARC_END_ANGLE                       2
#define REAL_ARC_DIAMETER                        3
#define REAL_ARC_AREA                            4
#define REAL_ARC_CIRCUMFERENCE                   5
#define REAL_ARC_LENGTH                          6
#define REAL_CHORD                               7
#define REAL_TEXT                                8
#define REAL_TEXT_FONT                           9
#define REAL_TEXT_JUSTIFY                       10
#define REAL_TEXT_SIZE                          11
#define REAL_RADIUS_MAJOR                       12
#define REAL_RADIUS_MINOR                       13
#define REAL_DIAMETER_MAJOR                     14
#define REAL_DIAMETER_MINOR                     15
#define REAL_LENGTH                             16
#define REAL_AREA                               17
#define REAL_ANGLE                              18

/* Integer identifiers. */
#define INT_TEXT_BOLD                            0
#define INT_TEXT_ITALIC                          1
#define INT_TEXT_UNDERLINE                       2
#define INT_TEXT_STRIKEOUT                       3
#define INT_TEXT_OVERLINE                        4
#define INT_TEXT_BACKWARD                        5
#define INT_TEXT_UPSIDEDOWN                      6

/* Rubber modes */
#define RUBBER_CIRCLE_1P_RAD                     0
#define RUBBER_CIRCLE_1P_DIA                     1
#define RUBBER_CIRCLE_2P                         2
#define RUBBER_CIRCLE_3P                         3
#define RUBBER_CIRCLE_TTR                        5
#define RUBBER_CIRCLE_TTT                        6
#define RUBBER_DIMLEADER_LINE                    7
#define RUBBER_ELLIPSE_LINE                      8
#define RUBBER_ELLIPSE_MAJORDIAMETER_MINORDIAMETER 9
#define RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS  10
#define RUBBER_ELLIPSE_ROTATION                 11
#define RUBBER_LINE                             12
#define RUBBER_POLYGON                          13
#define RUBBER_POLYGON_INSCRIBE                 14
#define RUBBER_POLYGON_CIRCUMSCRIBE             15
#define RUBBER_POLYLINE                         16
#define RUBBER_RECTANGLE                        17
#define RUBBER_TEXTSINGLE                       18
#define N_RUBBER_MODES                          40

/* Justify */
#define JUSTIFY_LEFT                             0
#define JUSTIFY_CENTER                           1
#define JUSTIFY_RIGHT                            2
#define JUSTIFY_ALIGNED                          3
#define JUSTIFY_MIDDLE                           4
#define JUSTIFY_FIT                              5
#define JUSTIFY_TOP_LEFT                         6
#define JUSTIFY_TOP_CENTER                       7
#define JUSTIFY_TOP_RIGHT                        8
#define JUSTIFY_MIDDLE_LEFT                      9
#define JUSTIFY_MIDDLE_CENTER                   10
#define JUSTIFY_MIDDLE_RIGHT                    11
#define JUSTIFY_BOTTOM_LEFT                     12
#define JUSTIFY_BOTTOM_CENTER                   13
#define JUSTIFY_BOTTOM_RIGHT                    14
#define TOTAL_JUSTIFY                           15

/* User Interface Mode */
#define MODE_ARC_INIT                            0
#define MODE_CIRCLE_INIT                         1
#define MODE_CIRCLE_1P_RAD                       2
#define MODE_CIRCLE_1P_DIA                       3
#define MODE_CIRCLE_2P                           4
#define MODE_CIRCLE_3P                           5
#define MODE_CIRCLE_TTR                          6
#define MODE_CIRCLE_TTR_SET_POINT_2              7
#define MODE_CIRCLE_TTR_SET_POINT_3              8
#define MODE_ELLIPSE_INIT                        9
#define MODE_RECTANGLE_INIT                     10
#define MODE_STAR_INIT                          11
#define MODE_STAR_NUM_POINTS                    12
#define MODE_STAR_CENTER_PT                     13
#define MODE_STAR_RAD_INNER                     14
#define MODE_STAR_RAD_OUTER                     15
#define MODE_POLYGON_INIT                       16
#define MODE_POLYGON_NUM_SIDES                  17
#define MODE_POLYGON_POLYTYPE                   18
#define MODE_POLYGON_DISTANCE                   19
#define MODE_POLYGON_CENTER_PT                  20
#define MODE_POLYGON_INSCRIBE                   21
#define MODE_POLYGON_CIRCUMSCRIBE               22
#define MODE_POLYGON_SIDE_LEN                   23
#define MODE_POINT_INIT                         24
#define MODE_RGB                                25
#define MODE_LINE                               26
#define MODE_ROTATE                             27
#define MODE_SCALE                              28
#define MODE_TEXT_SINGLE_SETGEOM                29
#define MODE_PATH_INIT                          30
#define MODE_POLYLINE_INIT                      31
#define MODE_MOVE                               32
#define MODE_DISTANCE                           33
#define MODE_QUICKLEADER_INIT                   34
#define MODE_SNOWFLAKE_INIT                     35
#define MODE_SNOWFLAKE_NUM_POINTS               36
#define MODE_LOCATE_POINT                       37
#define MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS  38
#define MODE_DOLPHIN_INIT                       39
#define MODE_DOLPHIN_NUM_POINTS                 40
#define MODE_ERASE                              41
#define TOTAL_MODES                             42

/* Object Properties: packed into the uint64_t flags variable. */
#define PROP_BOLD                           0x0001
#define PROP_ITALIC                         0x0002
#define PROP_UNDERLINE                      0x0004
#define PROP_STRIKEOUT                      0x0008
#define PROP_OVERLINE                       0x0010
#define PROP_BACKWARD                       0x0020
#define PROP_UPSIDEDOWN                     0x0040
#define PROP_CURVED                         0x0080
#define PROP_FILLED                         0x0100

/* Groupboxen */
#define GB_GENERAL                               0
#define GB_GEOM_ARC                              1
#define GB_MISC_ARC                              2
#define GB_GEOM_BLOCK                            3
#define GB_GEOM_CIRCLE                           4
#define GB_GEOM_DIM_ALIGNED                      5
#define GB_GEOM_DIM_ANGULAR                      6
#define GB_GEOM_DIM_ARC_LENGTH                   7
#define GB_GEOM_DIM_DIAMETER                     8
#define GB_GEOM_DIM_LEADER                       9
#define GB_GEOM_DIM_LINEAR                      10
#define GB_GEOM_DIM_ORDINATE                    11
#define GB_GEOM_DIM_RADIUS                      12
#define GB_GEOM_ELLIPSE                         13
#define GB_GEOM_IMAGE                           14
#define GB_MISC_IMAGE                           15
#define GB_GEOM_INFINITE_LINE                   16
#define GB_GEOM_LINE                            17
#define GB_GEOM_PATH                            18
#define GB_MISC_PATH                            19
#define GB_GEOM_POINT                           20
#define GB_GEOM_POLYGON                         21
#define GB_GEOM_POLYLINE                        22
#define GB_MISC_POLYLINE                        23
#define GB_GEOM_RAY                             24
#define GB_GEOM_RECTANGLE                       25
#define GB_GEOM_TEXT_MULTI                      26
#define GB_TEXT_TEXT_SINGLE                     27
#define GB_GEOM_TEXT_SINGLE                     28
#define GB_MISC_TEXT_SINGLE                     29
#define GB_TOTAL                                30

/* Settings indices. */
#define ST_LANGUAGE                              0
#define ST_ICON_THEME                            1
#define ST_ICON_SIZE                             2
#define ST_MDI_USE_LOGO                          3
#define ST_MDI_USE_TEXTURE                       4
#define ST_MDI_USE_COLOR                         5
#define ST_MDI_LOGO                              6
#define ST_MDI_TEXTURE                           7
#define ST_MDI_COLOR                             8
#define ST_TIP_OF_THE_DAY                        9
#define ST_CURRENT_TIP                          10
#define ST_SYSTEM_HELP_BROWSER                  11

/* Window settings. */
#define ST_WINDOW_POS_X                         12
#define ST_WINDOW_POS_Y                         13
#define ST_WINDOW_SIZE_X                        14
#define ST_WINDOW_SIZE_Y                        15

/* Display settings. */
#define ST_USE_OPENGL                           16
#define ST_ANTI_ALIAS                           17
#define ST_TEXT_ANTI_ALIAS                      18
#define ST_SMOOTH_PIXMAP                        19
#define ST_HQ_ANTI_ALIAS                        20
#define ST_NON_COSMETIC                         21
#define ST_SHOW_SCROLLBARS                      22
#define ST_SCROLLBAR_WIDGET_NUM                 23
#define ST_CROSSHAIR_COLOR                      24
#define ST_BG_COLOR                             25
#define ST_SELECTBOX_LEFT_COLOR                 26
#define ST_SELECTBOX_LEFT_FILL                  27
#define ST_SELECTBOX_RIGHT_COLOR                28
#define ST_SELECTBOX_RIGHT_FILL                 29
#define ST_SELECTBOX_ALPHA                      30
#define ST_ZOOMSCALE_IN                         31
#define ST_ZOOMSCALE_OUT                        32
#define ST_CROSSHAIR_PERCENT                    33
#define ST_DISPLAY_UNITS                        34

/* Prompt settings. */
#define ST_PROMPT_TEXT_COLOR                    35
#define ST_PROMPT_BG_COLOR                      36
#define ST_PROMPT_FONT_FAMILY                   37
#define ST_PROMPT_FONT_STYLE                    38
#define ST_PROMPT_FONT_SIZE                     39
#define ST_SAVE_HISTORY                         40
#define ST_HTML_OUTPUT                          41
#define ST_HISTORY_FILE                         42

/* Open/save settings. */
#define ST_OPENSAVE_FILTER                      43
#define ST_OPENSAVE_FORMAT                      44
#define ST_OPEN_THUMBNAIL                       45
#define ST_SAVE_FORMAT                          46
#define ST_SAVE_THUMBNAIL                       47
#define ST_RECENT_MAX                           48
#define ST_RECENT_FILES                         49
#define ST_RECENT_DIRECTORY                     50
#define ST_TRIM_NUM_JUMPS                       51

/* Print settings. */
#define ST_DEFAULT_PRINTER                      52
#define ST_USE_LAST_PRINTER                     53
#define ST_PRINT_DISABLE_BG                     54

/* Grid settings. */
#define ST_GRID_ON_LOAD                         55
#define ST_SHOW_ORIGIN                          56
#define ST_MATCH_GRID_CROSSHAIR                 57
#define ST_GRID_COLOR                           58
#define ST_GRID_LOAD_FROM_FILE                  59
#define ST_GRID_TYPE                            60
#define ST_GRID_CENTER_ORIGIN                   61
#define ST_GRID_SIZE_RADIUS                     62
#define ST_GRID_SPACING_RADIUS                  63
#define ST_GRID_SPACING_ANGLE                   64

/* Ruler settings. */
#define ST_RULER_ON_LOAD                        65
#define ST_RULER_METRIC                         66
#define ST_RULER_COLOR                          67
#define ST_RULER_SIZE                           68

/* Quicksnap settings. */
#define ST_QSNAP_ENABLED                        69
#define ST_QSNAP_LOCATOR_COLOR                  70
#define ST_QSNAP_LOCATOR_SIZE                   71
#define ST_QSNAP_APERTURE_SIZE                  72
#define ST_QSNAP_ENDPOINT                       73
#define ST_QSNAP_MIDPOINT                       74
#define ST_QSNAP_CENTER                         75
#define ST_QSNAP_NODE                           76
#define ST_QSNAP_QUADRANT                       77
#define ST_QSNAP_INTERSECTION                   78
#define ST_QSNAP_EXTENSION                      79
#define ST_QSNAP_INSERTION                      80
#define ST_QSNAP_PERPENDICULAR                  81
#define ST_QSNAP_TANGENT                        82
#define ST_QSNAP_NEAREST                        83
#define ST_QSNAP_APPARENT                       84
#define ST_QSNAP_PARALLEL                       85

/* Lineweight settings. */
#define ST_LWT_SHOW                             86
#define ST_LWT_REAL                             87
#define ST_LWT_DEFAULT                          88

/* Selection settings. */
#define ST_SELECTION_PICK_FIRST                 89
#define ST_SELECTION_PICK_ADD                   90
#define ST_SELECTION_PICK_DRAG                  91
#define ST_SELECTION_COOLGRIP_COLOR             92
#define ST_SELECTION_HOTGRIP_COLOR              93
#define ST_SELECTION_GRIP_SIZE                  94
#define ST_SELECTION_PICKBOX_SIZE               95

/* Text settings. */
#define ST_TEXT_FONT                            96
#define ST_TEXT_SIZE                            97
#define ST_TEXT_ANGLE                           98
#define ST_TEXT_BOLD                            99
#define ST_TEXT_ITALIC                         100
#define ST_TEXT_UNDERLINE                      101
#define ST_TEXT_STRIKEOUT                      102
#define ST_TEXT_OVERLINE                       103

#define ST_TICK_DEPTH                          104
#define ST_MAJOR_TICK_SEPERATION               105
#define ST_NEEDLE_SPEED                        106
#define ST_STITCH_TIME                         107

#define ST_GRID_CENTER_X                       108
#define ST_GRID_CENTER_Y                       109
#define ST_GRID_SIZE_X                         110
#define ST_GRID_SIZE_Y                         111
#define ST_GRID_SPACING_X                      112
#define ST_GRID_SPACING_Y                      113

#define SETTINGS_TOTAL                         114

/* Line editors */
#define ED_GENERAL_LAYER                         0
#define ED_GENERAL_COLOR                         1
#define ED_GENERAL_LINE_TYPE                     2
#define ED_GENERAL_LINE_WEIGHT                   3
#define ED_ARC_CENTER_X                          4
#define ED_ARC_CENTER_Y                          5
#define ED_ARC_RADIUS                            6
#define ED_ARC_START_ANGLE                       7
#define ED_ARC_END_ANGLE                         8
#define ED_ARC_START_X                           9
#define ED_ARC_START_Y                          10
#define ED_ARC_END_X                            11
#define ED_ARC_END_Y                            12
#define ED_ARC_AREA                             13
#define ED_ARC_LENGTH                           14
#define ED_ARC_CHORD                            15
#define ED_ARC_INC_ANGLE                        16
#define ED_ARC_CLOCKWISE                        17
#define ED_BLOCK_X                              18
#define ED_BLOCK_Y                              19
#define ED_CIRCLE_RADIUS                        15
#define ED_CIRCLE_DIAMETER                      16
#define ED_CIRCLE_CENTER_X                      17
#define ED_CIRCLE_CENTER_Y                      18
#define ED_CIRCLE_AREA                          21
#define ED_CIRCLE_CIRCUMFERENCE                 22
#define ED_DIM_ALIGNED_X                        23
#define ED_DIM_ANGULAR_X                        24
#define ED_DIM_ARC_LENGTH_X                     25
#define ED_DIM_DIAMETER_X                       26
#define ED_ELLIPSE_CENTER_X                     27
#define ED_ELLIPSE_CENTER_Y                     28
#define ED_ELLIPSE_RADIUS_MAJOR                 29
#define ED_ELLIPSE_RADIUS_MINOR                 30
#define ED_ELLIPSE_DIAMETER_MAJOR               31
#define ED_ELLIPSE_DIAMETER_MINOR               32
#define ED_TEXT_SINGLE_HEIGHT                   33
#define ED_TEXT_SINGLE_ROTATION                 34
#define ED_TEXT_SINGLE_X                        35
#define ED_TEXT_SINGLE_Y                        36
#define ED_TEXT_SINGLE_BACKWARD                 37
#define ED_TEXT_SINGLE_UPSIDE_DOWN              38
#define ED_TEXT_MULTI_X                         39
#define ED_TEXT_MULTI_Y                         40
#define ED_LINE_START_X                         41
#define ED_LINE_START_Y                         42
#define ED_LINE_END_X                           43
#define ED_LINE_END_Y                           44
#define ED_LINE_POINT_X                         45
#define ED_LINE_POINT_Y                         46
#define ED_TEXT_SINGLE_CONTENTS                 47
#define ED_TEXT_SINGLE_FONT                     48
#define ED_TEXT_SINGLE_JUSTIFY                  49
#define ED_RECTANGLE_CORNER1_X                  50
#define ED_RECTANGLE_CORNER1_Y                  51
#define ED_RECTANGLE_CORNER2_X                  52
#define ED_RECTANGLE_CORNER2_Y                  53
#define ED_RECTANGLE_CORNER3_X                  54
#define ED_RECTANGLE_CORNER3_Y                  55
#define ED_RECTANGLE_CORNER4_X                  56
#define ED_RECTANGLE_CORNER4_Y                  57
#define ED_RECTANGLE_WIDTH                      58
#define ED_RECTANGLE_HEIGHT                     59
#define ED_RECTANGLE_AREA                       60
#define ED_RAY_X1                               61
#define ED_RAY_Y1                               62
#define ED_RAY_X2                               63
#define ED_RAY_Y2                               64
#define ED_RAY_VECTOR_X                         65
#define ED_RAY_VECTOR_Y                         66
#define ED_GEOMETRY_DIM_LINEAR_X                67
#define ED_GEOM_DIM_ORDINATE_X                  68
#define ED_DIM_RADIUS_X                         69
#define ED_ELLIPSE_SEMI_MINOR_AXIS              70
#define ED_ELLIPSE_SEMI_MAJOR_AXIS              71
#define ED_ELLIPSE_MAJOR_AXIS                   72
#define ED_ELLIPSE_MINOR_AXIS                   73
#define ED_IMAGE_X                              74
#define ED_IMAGE_Y                              75
#define ED_IMAGE_WIDTH                          76
#define ED_IMAGE_HEIGHT                         77
#define ED_IMAGE_NAME                           78
#define ED_IMAGE_PATH                           79
#define ED_INFINITE_LINE_X1                     80
#define ED_INFINITE_LINE_Y1                     81
#define ED_INFINITE_LINE_X2                     82
#define ED_INFINITE_LINE_Y2                     83
#define ED_INFINITE_LINE_VECTOR_X               84
#define ED_INFINITE_LINE_VECTOR_Y               85
#define ED_LINE_DELTA_X                         86
#define ED_LINE_DELTA_Y                         87
#define ED_LINE_ANGLE                           88
#define ED_LINE_LENGTH                          89
#define ED_PATH_VERTEX_NUM                      90
#define ED_PATH_VERTEX_X                        91
#define ED_PATH_VERTEX_Y                        92
#define ED_PATH_AREA                            93
#define ED_PATH_LENGTH                          94
#define ED_PATH_CLOSED                          95
#define ED_POINT_X                              96
#define ED_POINT_Y                              97
#define ED_POLYGON_CENTER_X                     98
#define ED_POLYGON_CENTER_Y                     99
#define ED_POLYGON_RADIUS_VERTEX               100
#define ED_POLYGON_RADIUS_SIDE                 101
#define ED_POLYGON_DIAMETER_VERTEX             102
#define ED_POLYGON_DIAMETER_SIDE               103
#define ED_POLYGON_INTERIOR_ANGLE              104
#define ED_POLYLINE_VERTEX_NUM                 105
#define ED_POLYLINE_VERTEX_X                   106
#define ED_POLYLINE_VERTEX_Y                   107
#define ED_POLYLINE_VERTEX_AREA                108
#define ED_POLYLINE_VERTEX_LENGTH              109
#define ED_POLYLINE_VERTEX_CLOSED              110
#define TOTAL_EDITORS                          111

/* Editor types */
#define EDITOR_DOUBLE                            0
#define EDITOR_COMBOBOX                          1
#define EDITOR_FONT                              2
#define EDITOR_INT                               3
#define EDITOR_STRING                            4
#define EDITOR_CHECKBOX                          5

#define SNAP_POINT_TYPES                        13

/* View state */
#define VIEW_STATE_GRID                       0x01
#define VIEW_STATE_ORTHO                      0x02
#define VIEW_STATE_POLAR                      0x04
#define VIEW_STATE_ISO                        0x08
#define VIEW_STATE_QSNAP                      0x10
#define VIEW_STATE_QTRACK                     0x20
#define VIEW_STATE_LWT                        0x40
#define VIEW_STATE_REAL                       0x80
#define VIEW_STATE_METRIC                    0x100
#define VIEW_STATE_SIMULATE                  0x200
#define VIEW_STATE_SNAP                      0x400
#define VIEW_STATE_RULER                     0x800

/* Preview mode */
#define PREVIEW_MODE_NULL                        0
#define PREVIEW_MODE_MOVE                        1
#define PREVIEW_MODE_ROTATE                      2
#define PREVIEW_MODE_SCALE                       3

/* Preview clone */
#define PREVIEW_CLONE_NULL                       0
#define PREVIEW_CLONE_RUBBER                     1
#define PREVIEW_CLONE_SELECTED                   2


/* Command Prompt Style. */
#define CONSOLE_STYLE_COLOR                      0
#define CONSOLE_STYLE_BG_COLOR                   1
#define CONSOLE_STYLE_SELECTION_COLOR            2
#define CONSOLE_STYLE_SELECTION_BG_COLOR         3
#define CONSOLE_STYLE_FONT_FAMILY                4
#define CONSOLE_STYLE_FONT_STYLE                 5
#define CONSOLE_STYLE_FONT_SIZE                  6

/* Statusbar buttons */
#define STATUSBAR_SNAP                           0
#define STATUSBAR_GRID                           1
#define STATUSBAR_RULER                          2
#define STATUSBAR_ORTHO                          3
#define STATUSBAR_POLAR                          4
#define STATUSBAR_QSNAP                          5
#define STATUSBAR_QTRACK                         6
#define STATUSBAR_LWT                            7

#ifdef __cplusplus
extern "C" {
#endif

/* C/C++ Standard Libraries. */
#include <stdio.h>

#include "sizes.h"

#include "../extern/libembroidery/src/embroidery.h"

/*
 */
typedef struct Setting_ {
    int32_t id;
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    int type;
} Setting;

/*
 */
typedef struct GeometryData_ {
    uint64_t flags;
    uint32_t mode;
    uint32_t type;
    int32_t objID;

    int firstRun;

    int minPoints;
    int numPoints;
    int maxPoints;

    EmbReal width;
    EmbReal height;
    EmbReal rotation;

    EmbArc arc;
    EmbCircle circle;
    EmbEllipse ellipse;
    EmbLine line;
    EmbVector vector;

    EmbVector center;
    EmbVector point1;
    EmbVector point2;
    EmbVector first;
    EmbVector prev;
    EmbVector scale;

    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    EmbReal text_size;
} GeometryData;

/*
 */
typedef struct ViewData_ {
    uint8_t grippingActive;
    uint8_t rapidMoveActive;
    uint8_t previewActive;
    uint8_t pastingActive;
    uint8_t movingActive;
    uint8_t selectingActive;
    uint8_t zoomWindowActive;
    uint8_t panningRealTimeActive;
    uint8_t panningPointActive;
    uint8_t panningActive;
    uint8_t qSnapActive;
} ViewData;

typedef struct RubberPoint_ {
    char key[MAX_STRING_LENGTH];
    char text[MAX_STRING_LENGTH];
    EmbVector position;
} RubberPoint;

typedef struct Node_ {
    char s[MAX_STRING_LENGTH];
    int32_t i;
    EmbReal r;
    int32_t type;
} Node;

typedef struct WidgetData_ {
    int32_t id;
    int32_t type;
    int32_t groupbox;
    int32_t setting;
    int32_t key;
    EmbReal lower;
    EmbReal upper;
    EmbReal single_step;
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char map_signal[MAX_STRING_LENGTH];
} WidgetData;

/* Utility Functions. */
unsigned char validRGB(int r, int g, int b);
int str_contains(char *s, char c);
EmbReal fourier_series(EmbReal arg, EmbReal *terms, int n_terms);
uint8_t willUnderflowInt32(int64_t a, int64_t b);
uint8_t willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(uint8_t roundUp, int numToRound, int multiple);
int string_array_length(const char *list[]);
int tokenize(char **argv, char *str, const char delim);

/* Embroidermodder C++ functions that can be declared with C linkage */
void prompt_output(char *txt);
const char *translate_str(const char *str);
int validFileFormat(const char *fileName);
void debug_message(const char *msg);
int save_current_file(const char *fileName);
int read_settings(void);
void write_settings(void);
EmbVector rotate_vector(EmbVector v, EmbReal alpha);

/* Core functionality */
const char *actuator(char *string);

/* Geometry UI Processors */
GeometryData *geometry_init(int type);
void geometry_free(GeometryData *g);
void geometry_left_click(GeometryData *geometry, EmbVector v);
void geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH]);
void geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH]);
void geometry_update(GeometryData *g);
void geometry_set_flag(GeometryData *g, uint64_t flag);
void geometry_unset_flag(GeometryData *g, uint64_t flag);

/* The Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 */
extern Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

extern const WidgetData all_line_editors[MAX_EDITORS];
extern const WidgetData all_spinbox_editors[MAX_EDITORS];
extern WidgetData snap_point_data[];
extern WidgetData render_data[];
extern WidgetData save_history_data[];

extern Setting settings_data[];

/* Properties */
extern int general_props[];
extern int display_props[];
extern int prompt_props[];
extern int quick_snap_props[];
extern int opensave_props[];
extern int selection_props[];
extern int grid_ruler_props[];
extern int accept_preview_props[];
extern int accept_accept_props[];
extern const char *load_from_file_enabled[];
extern const char *grid_type_visibility_lattice[];
extern const char *grid_type_visibility_circular[];
extern const char *tab_names[];

/* Other strings and string tables. */
extern const char *version;
extern const char *usage_msg;
extern const char *extensions[];
extern const char *default_prompt_style[];
extern const char *details_labels[];
extern const char *command_labels[];
extern const char *justify_options[];
extern const char *object_names[];
extern const char *button_list[];
extern const char *tips[];
extern const char *settings_labels[];
extern char *coverage_test_script[];

/* Property editor data */
extern const char *group_box_data[];
extern const char *group_box_types[];
extern const int32_t group_box_ids[];

#ifdef __cplusplus
}
#endif

#endif
