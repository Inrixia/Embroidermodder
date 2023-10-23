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

/* Maximums for C-style memory arrays. */
#define MAX_STRING_LENGTH                      200
#define MAX_HISTOGRAM_BINS                    1000
#define MAX_TOOLBAR_LENGTH                      30
#define MAX_MENU_LENGTH                         30
#define MAX_MENUBAR_LENGTH                      10
#define MAX_TOOLBARS                            10
#define MAX_MENUS                               10
#define VECTOR_CHUNK_SIZE                       50
#define MAX_ACTIONS                            256
#define MAX_SETTINGS                           256
#define MAX_EDITORS                            300

/* Node types. */
#define NODE_NULL                                0
#define NODE_STRING                              1
#define NODE_REAL                                2
#define NODE_INT                                 3
#define NODE_BOOL                                4
#define NODE_FUNCTION                            5
#define NODE_DICTIONARY                          6
#define NODE_ARRAY                               7
#define NODE_VECTOR                              8
#define NODE_UNKNOWN                             9

/* Actions.
 * These identifiers are subject to change since they are in alphabetical order
 * and the numbers are increasing.
 */
#define ACTION_ABOUT                             0
#define ACTION_ADD_ARC                           1
#define ACTION_ADD_CIRCLE                        2
#define ACTION_ADD_DIM_LEADER                    3
#define ACTION_ADD_ELLIPSE                       4
#define ACTION_ADD_GEOMETRY                      5
#define ACTION_ADD_HORIZONTAL_DIMENSION          6
#define ACTION_ADD_IMAGE                         7
#define ACTION_ADD_INFINITE_LINE                 8
#define ACTION_ADD_LINE                          9
#define ACTION_ADD_PATH                         10
#define ACTION_ADD_POINT                        11
#define ACTION_ADD_POLYGON                      12
#define ACTION_ADD_POLYLINE                     13
#define ACTION_ADD_RAY                          14
#define ACTION_ADD_RECTANGLE                    15
#define ACTION_ADD_REGULAR_POLYGON              16
#define ACTION_ADD_ROUNDED_RECTANGLE            17
#define ACTION_ADD_RUBBER                       18
#define ACTION_ADD_SLOT                         19
#define ACTION_ADD_TEXT_MULTI                   20
#define ACTION_ADD_TEXT_SINGLE                  21
#define ACTION_ADD_TO_SELECTION                 22
#define ACTION_ADD_TRIANGLE                     23
#define ACTION_ADD_VERTICAL_DIMENSION           24
#define ACTION_ALERT                            25
#define ACTION_ALLOW_RUBBER                     26
#define ACTION_APPEND_HISTORY                   27
#define ACTION_CALCULATE_ANGLE                  28
#define ACTION_CALCULATE_DISTANCE               29
#define ACTION_CHANGELOG                        30
#define ACTION_CLEAR_RUBBER                     31
#define ACTION_CLEAR_SELECTION                  32
#define ACTION_COPY                             33
#define ACTION_COPY_SELECTED                    34
#define ACTION_CUT                              35
#define ACTION_CUT_SELECTED                     36
#define ACTION_DAY_VISION                       37
#define ACTION_DEBUG                            38
#define ACTION_DELETE_SELECTED                  39
#define ACTION_DESIGN_DETAILS                   40
#define ACTION_DO_NOTHING                       41
#define ACTION_END                              42
#define ACTION_ERROR                            43
#define ACTION_HELP                             44
#define ACTION_ICON                             45
#define ACTION_INIT                             46
#define ACTION_MESSAGEBOX                       47
#define ACTION_MIRROR_SELECTED                  48
#define ACTION_MOUSE_X                          49
#define ACTION_MOUSE_Y                          50
#define ACTION_MOVE_SELECTED                    51
#define ACTION_NEW                              52
#define ACTION_NIGHT_VISION                     53
#define ACTION_NUM_SELECTED                     54
#define ACTION_OPEN                             55
#define ACTION_PAN                              56
#define ACTION_PASTE                            57
#define ACTION_PASTE_SELECTED                   58
#define ACTION_PERPENDICULAR_DISTANCE           59
#define ACTION_PLATFORM                         60
#define ACTION_PREVIEW_OFF                      61
#define ACTION_PREVIEW_ON                       62
#define ACTION_PRINT                            63
#define ACTION_PRINT_AREA                       64
#define ACTION_QSNAP_X                          65
#define ACTION_QSNAP_Y                          66
#define ACTION_QUIT                             67
#define ACTION_REDO                             68
#define ACTION_ROTATE_SELECTED                  69
#define ACTION_RUBBER                           70
#define ACTION_SCALE_SELECTED                   71
#define ACTION_SELECT_ALL                       72
#define ACTION_SETTINGS_DIALOG                  73
#define ACTION_SET_BACKGROUND_COLOR             74
#define ACTION_SET_CROSSHAIR_COLOR              75
#define ACTION_SET_CURSOR_SHAPE                 76
#define ACTION_SET_GRID_COLOR                   77
#define ACTION_SET_PROMPT_PREFIX                78
#define ACTION_SET_RUBBER_FILTER                79
#define ACTION_SET_RUBBER_MODE                  80
#define ACTION_SET_RUBBER_POINT                 81
#define ACTION_SET_RUBBER_TEXT                  82
#define ACTION_SPARE_RUBBER                     83
#define ACTION_TIP_OF_THE_DAY                   84
#define ACTION_TODO                             85
#define ACTION_UNDO                             86
#define ACTION_VERSION                          87
#define ACTION_VULCANIZE                        88
#define ACTION_WHATS_THIS                       89
#define ACTION_WINDOW                           90
#define ACTION_ZOOM                             91
#define N_ACTIONS                               92

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

/* Mathematical Constants. */
#define CONSTANT_PI         3.14159265358979323846
#define RADIANS_TO_DEGREES    (180.0f/CONSTANT_PI)
#define DEGREES_TO_RADIANS    (CONSTANT_PI/180.0f)

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
#define ARC_START_POINT                          0
#define ARC_MID_POINT                            1
#define ARC_END_POINT                            2

/* Real Identifiers */
#define ARC_RADIUS                               3
#define ARC_START_ANGLE                          4
#define ARC_END_ANGLE                            5
#define ARC_DIAMETER                             6
#define ARC_AREA                                 7

/*
#define ARC_CIRCUMFERENCE                    9
#define ARC_POSITION                        10
#define OBJECT_TEXT                        12
#define OBJECT_TEXT_FONT                    13
#define OBJECT_TextJustify(QString justify);
#define OBJECT_TextSize(EmbReal size);
#define OBJECT_TextStyle(bool bold, bool italic, bool under, bool strike, bool over);
#define OBJECT_TextBold(bool val);
#define OBJECT_TextItalic(bool val);
#define OBJECT_TextUnderline(bool val);
#define OBJECT_TextStrikeOut(bool val);
#define OBJECT_TextOverline(bool val);
#define OBJECT_TextBackward(bool val);
#define OBJECT_TextUpsideDown(bool val);
#define OBJECT_RadiusMajor(EmbReal radius);
#define OBJECT_RadiusMinor(EmbReal radius);
#define OBJECT_DiameterMajor(EmbReal diameter);
#define OBJECT_DiameterMinor(EmbReal diameter);
*/

#define N_RUBBER_MODES                          40

/* Per object constants. */
#define POINTS_PER_BASE_OBJECT                 100
#define REALS_PER_BASE_OBJECT                  100
#define BOOLS_PER_BASE_OBJECT                  100
#define STRINGS_PER_BASE_OBJECT                 10

/* User Interface Mode */
#define MODE_ARC                                 0
#define MODE_CIRCLE_1P_RAD                       1
#define MODE_CIRCLE_1P_DIA                       2
#define MODE_CIRCLE_2P                           3
#define MODE_CIRCLE_3P                           4
#define MODE_CIRCLE_TTR                          5
#define MODE_CIRCLE_TTR_SET_POINT_2              6
#define MODE_CIRCLE_TTR_SET_POINT_3              7
#define MODE_ELLIPSE                             8
#define MODE_RECTANGLE                           9
#define MODE_STAR_NUM_POINTS                    10
#define MODE_STAR_CENTER_PT                     11
#define MODE_STAR_RAD_INNER                     12
#define MODE_STAR_RAD_OUTER                     13
#define MODE_POLYGON_NUM_SIDES                  14
#define MODE_POLYGON_POLYTYPE                   15
#define MODE_POLYGON_DISTANCE                   16
#define MODE_POLYGON_CENTER_PT                  17
#define MODE_POLYGON_INSCRIBE                   18
#define MODE_POLYGON_CIRCUMSCRIBE               19
#define MODE_POLYGON_SIDE_LEN                   20

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

/* View state */
#define VIEW_SNAP_MODE                      0x0001
#define VIEW_GRID_MODE                      0x0002
#define VIEW_RULER_MODE                     0x0004

/* Groupboxen */
#define GB_GENERAL                               0
#define GB_GROUPBOX_ARC                          1
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
#define GB_INFINITE_LINE                        16
#define GB_GEOM_LINE                            17
#define GB_GEOM_PATH                            18
#define GB_MISC_PATH                            19
#define GB_GEOM_POINT                           20
/*
    geometry_polygon_gb = createGroupBox("geometry_polygon", "Geometry");
    geometry_polyline_gb = createGroupBox("geometry_polyline", "Geometry");
    misc_polyline_gb = createGroupBox("misc_polyline", "Misc");
    geometry_ray_gb = createGroupBox("geometry_ray", "Geometry");
    geometry_rectangle_gb = createGroupBox("geometry_rectangle", "Geometry");
    geometry_text_multi_gb = createGroupBox("geometry_text_multi", "Geometry");
    text_text_single_gb = createGroupBox("text_text_single", "Text");
    geometry_text_single_gb = createGroupBox("geometry_text_single", "Geometry");
    misc_text_single_gb = createGroupBox("misc_text_single", "Misc
*/
#define GB_TOTAL                                22

#ifdef __cplusplus
extern "C" {
#endif

/* C/C++ Standard Libraries. */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

/* We assume here that all free systems and MacOS are POSIX compliant. */
#if !defined(WIN32)
#include <sys/utsname.h>
#endif

#include <errno.h>

#include "../extern/libembroidery/src/embroidery.h"

/*
 * Expected Keys for actions
 * String icon;
 *      The stub used for finding the icon image file.
 * String command;
 *      The command sent to the actuator.
 * String tooltip;
 *      The label in the menus and the message that appears when
 *      you hover over an icon.
 * String statustip;
 *       The message that appears at the bottom of the .
 * String shortcut;
 *       The keyboard shortcut for this action.
 * StringList aliases;
 *       A list of all alternative commands, if empty only
 *       the icon sttring will be .
 * StringList script;
 *      If this is a compound action this will be a
 *      list of commands or it can allow for command line
 *      style command aliases. For example: icon16 would become
 *      the string list {"iconResize 16"}.
 */
typedef struct ActionData_ {
    char icon[MAX_STRING_LENGTH];
    char command[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
    int32_t min_args;
    int32_t gview;
    int32_t gscene;
    int32_t undo;
} ActionData;

/*
 */
typedef struct LineEditData_ {
    char groupbox[MAX_STRING_LENGTH];
    char key[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char type[MAX_STRING_LENGTH];
    char map_signal[MAX_STRING_LENGTH];
} LineEditData;

/*
 */
typedef struct Settings_ {
    EmbReal quicksnap_locator_size;
    bool selection_mode_pickadd;
    bool grid_show_on_load;
    bool grid_center_on_origin;
    bool grid_show_origin;
    bool ruler_show_on_load;
    bool ruler_metric;
    uint32_t ruler_color;
} Settings;

/* To allow us to resize general C arrays when necessary.
 * Note that for char arrays, the buffer is a normal c-style string.
 */
typedef struct Cvector_ {
    char *buffer;
    int32_t size;
    int32_t max_length;
    int32_t element_size;
} Cvector;

/*
 *
 */
struct Node_ {
    struct Node_ **leaves;
    int32_t n_leaves;
    int32_t max_leaves;
    char key[MAX_STRING_LENGTH];
    char data[MAX_STRING_LENGTH];
    char s[MAX_STRING_LENGTH];
    int32_t i;
    EmbReal r;
    Cvector *vec;
    int32_t type;
};

typedef struct Node_ Node;

Cvector *cvector_create(size_t element_size);
void cvector_append(Cvector *a, Cvector *b);
void cvector_add_cstr(Cvector *a, char *b);
Cvector *cvector_copy(Cvector *a);
void cvector_free(Cvector *vector);
int string_array_length(const char *list[]);

unsigned char validRGB(int r, int g, int b);
int str_contains(char *s, char c);

Node *create_node(int type);
int add_leaf(Node *branch, Node *leaf);
void print_tree(Node *branch, int indent);
Node *find_node(Node *branch, char key[MAX_STRING_LENGTH]);
void free_node(Node *branch);
Node *create_and_add_leaf(Node *parent, char *key, char *value);
int insert_node(Node *branch, char key[MAX_STRING_LENGTH], Node *node);

extern Node *root;
extern const ActionData action_table[MAX_ACTIONS];
extern const LineEditData all_line_editors[MAX_EDITORS];
extern const char *version;
extern const char *usage_msg;
extern const char default_settings[MAX_SETTINGS][MAX_STRING_LENGTH];

extern const char *group_box_data[];
extern const char *toolbar_list[];
extern const char *menubar_order[];
extern const char *top_toolbar_layout[];
extern const char *bottom_toolbar_layout[];
extern const char *side_toolbar_layout[];
extern const char *file_menu[];
extern const char *edit_menu[];
extern const char *pan_menu[];
extern const char *zoom_menu[];
extern const char *view_menu[];
extern const char *settings_menu[];
extern const char *window_menu[];
extern const char *help_menu[];
extern const char *draw_menu[];

extern const char *file_toolbar[];
extern const char *edit_toolbar[];
extern const char *pan_toolbar[];
extern const char *zoom_toolbar[];
extern const char *view_toolbar[];
extern const char *settings_toolbar[];
extern const char *window_toolbar[];
extern const char *help_toolbar[];
extern const char *draw_toolbar[];
extern const char *icon_toolbar[];

extern const char *default_prompt_style[];
extern const char *details_labels[];
extern const char *command_labels[];
extern const char *justify_options[];
extern const char *object_names[];
extern const char *button_list[];
extern const char *tips[];
extern const char *group_box_types[];

extern const int32_t group_box_ids[];

#ifdef __cplusplus
}
#endif

#endif
