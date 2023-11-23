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

#ifndef _EMBROIDERMODDER_ACTIONS_
#define _EMBROIDERMODDER_ACTIONS_

#include "sizes.h"

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
#define ACTION_DAY                              37
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
#define ACTION_NIGHT                            53
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
#define ACTION_EXIT                             67
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
#define ACTION_WINDOW_CLOSE                     90
#define ACTION_WINDOW_CLOSE_ALL                 91
#define ACTION_WINDOW_TILE                      92
#define ACTION_WINDOW_CASCADE                   93
#define ACTION_WINDOW_NEXT                      94
#define ACTION_WINDOW_PREVIOUS                  95
#define ACTION_ZOOM                             96
#define ACTION_ZOOM_IN                          97
#define ACTION_TEST                             98
#define ACTION_SLEEP                            99
#define ACTION_LAYER_EDITOR                    100
#define ACTION_MAKE_LAYER_CURRENT              101
#define ACTION_TEXT_BOLD                       102
#define ACTION_TEXT_ITALIC                     103
#define ACTION_TEXT_UNDERLINE                  104
#define ACTION_TEXT_STRIKEOUT                  105
#define ACTION_TEXT_OVERLINE                   106
#define ACTION_LAYER_PREVIOUS                  107
#define ACTION_ICON16                          108
#define ACTION_ICON24                          109
#define ACTION_ICON32                          110
#define ACTION_ICON48                          111
#define ACTION_ICON64                          112
#define ACTION_ICON128                         113
#define ACTION_SAVE                            114
#define ACTION_SAVEAS                          115
#define ACTION_PAN_REAL_TIME                   116
#define ACTION_PAN_POINT                       117
#define ACTION_PAN_LEFT                        118
#define ACTION_PAN_RIGHT                       119
#define ACTION_PAN_UP                          120
#define ACTION_PAN_DOWN                        121
#define ACTION_ZOOM_REAL_TIME                  122
#define ACTION_ZOOM_PREVIOUS                   123
#define ACTION_ZOOM_WINDOW                     124
#define ACTION_ZOOM_DYNAMIC                    125
#define ACTION_ZOOM_OUT                        126
#define ACTION_ZOOM_EXTENTS                    127
#define ACTION_LAYERS                          128
#define ACTION_LAYER_SELECTOR                  129
#define ACTION_TREBLECLEF                      130
#define ACTION_COLOR_SELECTOR                  131
#define ACTION_LINE_TYPE_SELECTOR              132
#define ACTION_LINE_WEIGHT_SELECTOR            133
#define ACTION_ZOOM_SCALE                      134
#define ACTION_ZOOM_CENTER                     135
#define ACTION_HIDE_ALL_LAYERS                 136
#define ACTION_ZOOM_SELECTED                   137
#define ACTION_ZOOM_ALL                        138
#define ACTION_ADD_HEART                       139
#define ACTION_ADD_SINGLE_LINE_TEXT            140
#define ACTION_SHOW_ALL_LAYERS                 141
#define ACTION_FREEZE_ALL_LAYERS               142
#define ACTION_THAW_ALL_LAYERS                 143
#define ACTION_LOCK_ALL_LAYERS                 144
#define ACTION_UNLOCK_ALL_LAYERS               145
#define ACTION_ADD_DOLPHIN                     146
#define ACTION_ADD_DISTANCE                    147
#define ACTION_LOCATE_POINT                    148
#define ACTION_QUICKSELECT                     149
#define ACTION_SPELLCHECK                      150
#define ACTION_DISTANCE                        151
#define ACTION_MOVE                            152
#define ACTION_QUICKLEADER                     153
#define ACTION_RGB                             154
#define ACTION_ROTATE                          155
#define ACTION_SANDBOX                         156
#define ACTION_ADD_SNOWFLAKE                   157
#define ACTION_ADD_STAR                        158
#define ACTION_DELETE                          159
#define ACTION_SCALE                           160
#define ACTION_SINGLE_LINE_TEXT                161
#define ACTION_SYSWINDOWS                      162
#define N_ACTIONS                              163

#ifdef __cplusplus
extern "C" {
#endif

/*
 */
typedef struct ActionData_ {
    int32_t id;
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

extern const ActionData action_table[MAX_ACTIONS];

#ifdef __cplusplus
}
#endif

#endif
