/* Copyright 2011 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* From dev/pp_cursor_type_dev.idl modified Thu Nov 17 15:27:17 2011. */

#ifndef PPAPI_C_DEV_PP_CURSOR_TYPE_DEV_H_
#define PPAPI_C_DEV_PP_CURSOR_TYPE_DEV_H_

#include "ppapi/c/pp_macros.h"

/**
 * @file
 * This file defines enumerations for cursor types.
 */


/**
 * @addtogroup Enums
 * @{
 */
enum PP_CursorType_Dev {
  PP_CURSORTYPE_CUSTOM = -1,
  PP_CURSORTYPE_POINTER = 0,
  PP_CURSORTYPE_CROSS = 1,
  PP_CURSORTYPE_HAND = 2,
  PP_CURSORTYPE_IBEAM = 3,
  PP_CURSORTYPE_WAIT = 4,
  PP_CURSORTYPE_HELP = 5,
  PP_CURSORTYPE_EASTRESIZE = 6,
  PP_CURSORTYPE_NORTHRESIZE = 7,
  PP_CURSORTYPE_NORTHEASTRESIZE = 8,
  PP_CURSORTYPE_NORTHWESTRESIZE = 9,
  PP_CURSORTYPE_SOUTHRESIZE = 10,
  PP_CURSORTYPE_SOUTHEASTRESIZE = 11,
  PP_CURSORTYPE_SOUTHWESTRESIZE = 12,
  PP_CURSORTYPE_WESTRESIZE = 13,
  PP_CURSORTYPE_NORTHSOUTHRESIZE = 14,
  PP_CURSORTYPE_EASTWESTRESIZE = 15,
  PP_CURSORTYPE_NORTHEASTSOUTHWESTRESIZE = 16,
  PP_CURSORTYPE_NORTHWESTSOUTHEASTRESIZE = 17,
  PP_CURSORTYPE_COLUMNRESIZE = 18,
  PP_CURSORTYPE_ROWRESIZE = 19,
  PP_CURSORTYPE_MIDDLEPANNING = 20,
  PP_CURSORTYPE_EASTPANNING = 21,
  PP_CURSORTYPE_NORTHPANNING = 22,
  PP_CURSORTYPE_NORTHEASTPANNING = 23,
  PP_CURSORTYPE_NORTHWESTPANNING = 24,
  PP_CURSORTYPE_SOUTHPANNING = 25,
  PP_CURSORTYPE_SOUTHEASTPANNING = 26,
  PP_CURSORTYPE_SOUTHWESTPANNING = 27,
  PP_CURSORTYPE_WESTPANNING = 28,
  PP_CURSORTYPE_MOVE = 29,
  PP_CURSORTYPE_VERTICALTEXT = 30,
  PP_CURSORTYPE_CELL = 31,
  PP_CURSORTYPE_CONTEXTMENU = 32,
  PP_CURSORTYPE_ALIAS = 33,
  PP_CURSORTYPE_PROGRESS = 34,
  PP_CURSORTYPE_NODROP = 35,
  PP_CURSORTYPE_COPY = 36,
  PP_CURSORTYPE_NONE = 37,
  PP_CURSORTYPE_NOTALLOWED = 38,
  PP_CURSORTYPE_ZOOMIN = 39,
  PP_CURSORTYPE_ZOOMOUT = 40,
  PP_CURSORTYPE_GRAB = 41,
  PP_CURSORTYPE_GRABBING = 42,
  PP_CURSORTYPE_MIDDLEPANNINGVERTICAL = 43,
  PP_CURSORTYPE_MIDDLEPANNINGHORIZONTAL = 44
};
PP_COMPILE_ASSERT_ENUM_SIZE_IN_BYTES(PP_CursorType_Dev, 4);
/**
 * @}
 */

#endif  /* PPAPI_C_DEV_PP_CURSOR_TYPE_DEV_H_ */

