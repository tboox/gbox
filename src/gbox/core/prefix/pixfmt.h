/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        pixfmt.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_PIXFMT_H
#define GB_CORE_PREFIX_PIXFMT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// argb => pixel
#define GB_RGB_565(r, g, b)         ((((r) << 8) & 0xf800) | (((g) << 3) & 0x7e0) | (((b) >> 3)))
#define GB_RGB_888(r, g, b)         ((b) | ((g) << 8) | ((r) << 16))

#define GB_ARGB_1555(a, r, g, b)    ((((a) << 8) & 0x8000) | (((r) << 7) & 0x7c00) | (((g) << 2) & 0x3e0) | (((b) >> 3)))
#define GB_XRGB_1555(r, g, b)       ((0x8000) | (((r) << 7) & 0x7c00) | (((g) << 2) & 0x3e0) | (((b) >> 3)))

#define GB_ARGB_4444(a, r, g, b)    ((((a) << 8) & 0xf000) | (((r) << 4) & 0x0f00) | ((g) & 0x00f0) | (((b) >> 4)))
#define GB_XRGB_4444(r, g, b)       ((0xf000) | (((r) << 4) & 0x0f00) | ((g) & 0x00f0) | (((b) >> 4)))

#define GB_ARGB_8888(a, r, g, b)    ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))
#define GB_XRGB_8888(r, g, b)       ((b) | ((g) << 8) | ((r) << 16) | ((0xff) << 24))

#define GB_RGBA_5551(r, g, b, a)    ((((r) << 8) & 0xf800) | (((g) << 3) & 0x07c0) | (((b) >> 2) & 0x03e) | (((a) >> 7)))
#define GB_RGBX_5551(r, g, b)       ((((r) << 8) & 0xf800) | (((g) << 3) & 0x07c0) | (((b) >> 2) & 0x03e) | (0x0001))

#define GB_RGBA_4444(r, g, b, a)    ((((r) << 8) & 0xf000) | (((g) << 4) & 0x0f00) | ((b) & 0x00f0) | (((a) >> 4)))
#define GB_RGBX_4444(r, g, b)       ((((r) << 8) & 0xf000) | (((g) << 4) & 0x0f00) | ((b) & 0x00f0) | (0x000f))

#define GB_RGBA_8888(r, g, b, a)    ((a) | ((b) << 8) | ((g) << 16) | ((r) << 24))
#define GB_RGBX_8888(r, g, b)       (0xff | ((b) << 8) | ((g) << 16) | ((r) << 24))

// pixel => argb
#define GB_RGB_565_R(c)             (((c) >> 8) & 0x00f8)   //!< 1111 1000
#define GB_RGB_565_G(c)             (((c) >> 3) & 0x00fc)   //!< 1111 1100
#define GB_RGB_565_B(c)             (((c) << 3) & 0x00f8)   //!< 1111 1000
    
#define GB_RGB_888_R(c)             (((c) >> 16) & 0x0000ff)
#define GB_RGB_888_G(c)             (((c) >> 8) & 0x0000ff)
#define GB_RGB_888_B(c)             ((c) & 0x0000ff)

#define GB_ARGB_1555_A(c)           (((c) >> 8) & 0x0080)   //!< 1000 0000
#define GB_ARGB_1555_R(c)           (((c) >> 7) & 0x00f8)   //!< 1111 1000
#define GB_ARGB_1555_G(c)           (((c) >> 2) & 0x00f8)   //!< 1111 1000
#define GB_ARGB_1555_B(c)           (((c) << 3) & 0x00f8)   //!< 1111 1000

#define GB_XRGB_1555_R(c)           (((c) >> 7) & 0x00f8)   //!< 1111 1000
#define GB_XRGB_1555_G(c)           (((c) >> 2) & 0x00f8)   //!< 1111 1000
#define GB_XRGB_1555_B(c)           (((c) << 3) & 0x00f8)   //!< 1111 1000

#define GB_ARGB_4444_A(c)           (((c) >> 8) & 0x00f0)   //!< 1111 0000
#define GB_ARGB_4444_R(c)           (((c) >> 4) & 0x00f0)   //!< 1111 0000
#define GB_ARGB_4444_G(c)           ((c) & 0x00f0)          //!< 1111 0000
#define GB_ARGB_4444_B(c)           (((c) << 4) & 0x00f0)   //!< 1111 0000

#define GB_XRGB_4444_R(c)           (((c) >> 4) & 0x00f0)   //!< 1111 0000
#define GB_XRGB_4444_G(c)           ((c) & 0x00f0)          //!< 1111 0000
#define GB_XRGB_4444_B(c)           (((c) << 4) & 0x00f0)   //!< 1111 0000

#define GB_ARGB_8888_A(c)           (((c) >> 24) & 0x000000ff)
#define GB_ARGB_8888_R(c)           (((c) >> 16) & 0x000000ff)
#define GB_ARGB_8888_G(c)           (((c) >> 8) & 0x000000ff)
#define GB_ARGB_8888_B(c)           ((c) & 0x000000ff)

#define GB_XRGB_8888_R(c)           (((c) >> 16) & 0x000000ff)
#define GB_XRGB_8888_G(c)           (((c) >> 8) & 0x000000ff)
#define GB_XRGB_8888_B(c)           ((c) & 0x000000ff)

#define GB_RGBA_5551_R(c)           (((c) >> 8) & 0x00f8)   //!< 1111 1000
#define GB_RGBA_5551_G(c)           (((c) >> 3) & 0x00f8)   //!< 1111 1000
#define GB_RGBA_5551_B(c)           (((c) << 2) & 0x00f8)   //!< 1111 1000
#define GB_RGBA_5551_A(c)           (((c) << 7) & 0x0080)   //!< 1000 0000

#define GB_RGBX_5551_R(c)           (((c) >> 8) & 0x00f8)   //!< 1111 1000
#define GB_RGBX_5551_G(c)           (((c) >> 3) & 0x00f8)   //!< 1111 1000
#define GB_RGBX_5551_B(c)           (((c) << 2) & 0x00f8)   //!< 1111 1000

#define GB_RGBA_4444_R(c)           (((c) >> 8) & 0x00f0)   //!< 1111 0000
#define GB_RGBA_4444_G(c)           (((c) >> 4) & 0x00f0)   //!< 1111 0000
#define GB_RGBA_4444_B(c)           ((c) & 0x00f0)          //!< 1111 0000
#define GB_RGBA_4444_A(c)           (((c) << 4) & 0x00f0)   //!< 1111 0000

#define GB_RGBX_4444_R(c)           (((c) >> 8) & 0x00f0)   //!< 1111 0000
#define GB_RGBX_4444_G(c)           (((c) >> 4) & 0x00f0)   //!< 1111 0000
#define GB_RGBX_4444_B(c)           ((c) & 0x00f0)          //!< 1111 0000

#define GB_RGBA_8888_R(c)           (((c) >> 24) & 0x000000ff)
#define GB_RGBA_8888_G(c)           (((c) >> 16) & 0x000000ff)
#define GB_RGBA_8888_B(c)           (((c) >> 8) & 0x000000ff)
#define GB_RGBA_8888_A(c)           ((c) & 0x000000ff)

#define GB_RGBX_8888_R(c)           (((c) >> 24) & 0x000000ff)
#define GB_RGBX_8888_G(c)           (((c) >> 16) & 0x000000ff)
#define GB_RGBX_8888_B(c)           (((c) >> 8) & 0x000000ff)

/// the big-endian mask
#define GB_PIXFMT_BENDIAN           (0x0100)

/*! @def  GB_PIXFMT_NENDIAN
 *
 * the native endian mask
 */
#ifdef TB_WORDS_BIGENDIAN
#   define GB_PIXFMT_NENDIAN        (GB_PIXFMT_BENDIAN)
#else
#   define GB_PIXFMT_NENDIAN        (0x0000)
#endif

/// the alpha mask?
#define GB_PIXFMT_ALPHA             (0x0200)

/// pixfmt
#define GB_PIXFMT(pixfmt)           (((pixfmt) & 0xff))

/// is big endian?
#define GB_PIXFMT_BE(pixfmt)        (((pixfmt) & GB_PIXFMT_BENDIAN))

/// has alpha
#define GB_PIXFMT_HAS_ALPHA(pixfmt) (((pixfmt) & GB_PIXFMT_ALPHA))

/// pixfmt ok?
#define GB_PIXFMT_OK(pixfmt)        (GB_PIXFMT(pixfmt) != GB_PIXFMT_NONE)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the pixel format type
typedef enum __gb_pixfmt_t
{
    GB_PIXFMT_NONE          = 0     
,   GB_PIXFMT_PAL8          = 1     | GB_PIXFMT_ALPHA       //!< 8-bit palette

,   GB_PIXFMT_RGB565        = 2                             //!< 16-bit r g b: 5 6 5 r: 11111000 g: 11111100 b: 11111000
,   GB_PIXFMT_RGB888        = 3                             //!< 24-bit r g b: 8 8 8
,   GB_PIXFMT_ARGB1555      = 4     | GB_PIXFMT_ALPHA       //!< 16-bit a r g b: 1 5 5 5 a: 10000000 r: 11111000 g: 11111000 b: 11111000
,   GB_PIXFMT_XRGB1555      = 5                             //!< 16-bit x r g b: 1 5 5 5 x: 10000000 r: 11111000 g: 11111000 b: 11111000
,   GB_PIXFMT_ARGB4444      = 6     | GB_PIXFMT_ALPHA       //!< 16-bit a r g b: 4 4 4 4
,   GB_PIXFMT_XRGB4444      = 7                             //!< 16-bit x r g b: 4 4 4 4
,   GB_PIXFMT_ARGB8888      = 8     | GB_PIXFMT_ALPHA       //!< 32-bit a r g b: 8 8 8 8
,   GB_PIXFMT_XRGB8888      = 9                             //!< 32-bit x r g b: 8 8 8 8
,   GB_PIXFMT_RGBA5551      = 10    | GB_PIXFMT_ALPHA       //!< 16-bit r g b a: 5 5 5 1 r: 11111000 g: 11111000 b: 11111000 a: 10000000
,   GB_PIXFMT_RGBX5551      = 11                            //!< 16-bit r g b x: 5 5 5 1 r: 11111000 g: 11111000 b: 11111000 x: 10000000
,   GB_PIXFMT_RGBA4444      = 12    | GB_PIXFMT_ALPHA       //!< 16-bit r g b a: 4 4 4 4
,   GB_PIXFMT_RGBX4444      = 13                            //!< 16-bit r g b x: 4 4 4 4
,   GB_PIXFMT_RGBA8888      = 14    | GB_PIXFMT_ALPHA       //!< 32-bit r g b a: 8 8 8 8
,   GB_PIXFMT_RGBX8888      = 15                            //!< 32-bit r g b x: 8 8 8 8

,   GB_PIXFMT_BGR565        = 16                            //!< 16-bit b g r: 5 6 5 b: 11111000 g: 11111100 r: 11111000
,   GB_PIXFMT_BGR888        = 17                            //!< 24-bit b g r: 8 8 8
,   GB_PIXFMT_ABGR1555      = 18    | GB_PIXFMT_ALPHA       //!< 16-bit a b g r: 1 5 5 5 a: 10000000 b: 11111000 g: 11111000 r: 11111000
,   GB_PIXFMT_XBGR1555      = 19                            //!< 16-bit x b g r: 1 5 5 5 x: 10000000 b: 11111000 g: 11111000 r: 11111000
,   GB_PIXFMT_ABGR4444      = 20    | GB_PIXFMT_ALPHA       //!< 16-bit a b g r: 4 4 4 4
,   GB_PIXFMT_XBGR4444      = 21                            //!< 16-bit x b g r: 4 4 4 4
,   GB_PIXFMT_ABGR8888      = 22    | GB_PIXFMT_ALPHA       //!< 32-bit a b g r: 8 8 8 8
,   GB_PIXFMT_XBGR8888      = 23                            //!< 32-bit x b g r: 8 8 8 8
,   GB_PIXFMT_BGRA5551      = 24    | GB_PIXFMT_ALPHA       //!< 16-bit b g r a: 5 5 5 1 b: 11111000 g: 11111000 r: 11111000 a: 10000000 
,   GB_PIXFMT_BGRX5551      = 25                            //!< 16-bit b g r x: 5 5 5 1 b: 11111000 g: 11111000 r: 11111000 x: 10000000
,   GB_PIXFMT_BGRA4444      = 26    | GB_PIXFMT_ALPHA       //!< 16-bit b g r a: 4 4 4 4
,   GB_PIXFMT_BGRX4444      = 27                            //!< 16-bit b g r x: 4 4 4 4
,   GB_PIXFMT_BGRA8888      = 28    | GB_PIXFMT_ALPHA       //!< 32-bit b g r a: 8 8 8 8
,   GB_PIXFMT_BGRX8888      = 29                            //!< 32-bit b g r x: 8 8 8 8

}gb_pixfmt_t;


#endif


