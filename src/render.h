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

#ifndef __EMBROIDERMODDER_RENDER__
#define __EMBROIDERMODDER_RENDER__

#ifdef __cplusplus
extern "C" {
#endif

void init_render(int width, int height);
void main_render(void);
void resize_render(int width, int height);

#ifdef __cplusplus
}
#endif

#endif
