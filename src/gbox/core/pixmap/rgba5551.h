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
 * @file        rgba5551.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGBA5551_H
#define GB_CORE_PIXMAP_RGBA5551_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "rgb16.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* the alpha blend 
 *
 * c: 
 * 0000 0000 0000 0000 rrrr rggg ggbb bbba
 *
 * c | c << 16:
 * rrrr rggg ggbb bbba rrrr rggg ggbb bbba
 *
 * 0x7c0f83e:
 * 0000 0111 1100 0000 1111 1000 0011 1110
 *
 * d = (c | c << 16) & 0x7c0f83e:
 * 0000 0ggg gg00 0000 rrrr r000 00bb bbb0
 *
 * (d & 0xffff) | (d >> 16):
 * 0000 0000 0000 0000 rrrr rrgg ggbb bbb0
 *
 * (s * a + d * (32 - a)) >> 5 => ((s - d) * a) >> 5 + d
 */
static __tb_inline__ tb_uint16_t gb_pixmap_rgba5551_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    // FIXME: s - d? overflow?
    s = (s | (s << 16)) & 0x7c0f83e;
    d = (d | (d << 16)) & 0x7c0f83e;
    d = ((((s - d) * a) >> 5) + d) & 0x7c0f83e;
    return (tb_uint16_t)((d & 0xffff) | (d >> 16) | 0x0001); // FIXME: ignore the alpha bit now.
}
static __tb_inline__ tb_uint16_t gb_pixmap_rgba5551_blend2(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    d = (d | (d << 16)) & 0x7c0f83e;
    d = ((((s - d) * a) >> 5) + d) & 0x7c0f83e;
    return (tb_uint16_t)((d & 0xffff) | (d >> 16) | 0x0001); // FIXME: ignore the alpha bit now.
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgba5551_pixel(gb_color_t color)
{
    return GB_RGBA_5551(color.r, color.g, color.b, color.a);
}
static __tb_inline__ gb_color_t gb_pixmap_rgba5551_color(gb_pixel_t pixel)
{
    gb_color_t color;
    color.r = GB_RGBA_5551_R(pixel);
    color.g = GB_RGBA_5551_G(pixel);
    color.b = GB_RGBA_5551_B(pixel);
    color.a = GB_RGBA_5551_A(pixel);
    return color;
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixel_set_la(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_le(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixel_set_ba(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_be(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_le(data), tb_bits_get_u16_le(source), alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_be(data), tb_bits_get_u16_be(source), alpha >> 3));
}
static __tb_inline__ gb_color_t gb_pixmap_rgba5551_color_get_l(tb_cpointer_t data)
{
    return gb_pixmap_rgba5551_color(tb_bits_get_u16_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_rgba5551_color_get_b(tb_cpointer_t data)
{
    return gb_pixmap_rgba5551_color(tb_bits_get_u16_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_color_set_lo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba5551_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_color_set_bo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba5551_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_color_set_la(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_le(data), gb_pixmap_rgba5551_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_color_set_ba(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba5551_blend(tb_bits_get_u16_be(data), gb_pixmap_rgba5551_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x7c0f83e;
    while (p < e)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        tb_bits_set_u16_le(&p[1], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_le(&p[1]), s, alpha));
        tb_bits_set_u16_le(&p[2], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_le(&p[2]), s, alpha));
        tb_bits_set_u16_le(&p[3], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_le(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        p++;
    }
}
static __tb_inline__ tb_void_t gb_pixmap_rgba5551_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x7c0f83e;
    while (p < e)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        tb_bits_set_u16_be(&p[1], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_be(&p[1]), s, alpha));
        tb_bits_set_u16_be(&p[2], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_be(&p[2]), s, alpha));
        tb_bits_set_u16_be(&p[3], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_be(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_rgba5551_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        p++;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_rgba5551 =
{   
    "rgba5551"
,   16
,   2
,   GB_PIXFMT_RGBA5551 
,   gb_pixmap_rgba5551_pixel
,   gb_pixmap_rgba5551_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgb16_pixel_set_lo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_rgba5551_color_get_l
,   gb_pixmap_rgba5551_color_set_lo
,   gb_pixmap_rgb16_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_rgba5551 =
{   
    "rgba5551"
,   16
,   2
,   GB_PIXFMT_RGBA5551 | GB_PIXFMT_BENDIAN
,   gb_pixmap_rgba5551_pixel
,   gb_pixmap_rgba5551_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgb16_pixel_set_bo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_rgba5551_color_get_b
,   gb_pixmap_rgba5551_color_set_bo
,   gb_pixmap_rgb16_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_rgba5551 =
{   
    "rgba5551"
,   16
,   2
,   GB_PIXFMT_RGBA5551 
,   gb_pixmap_rgba5551_pixel
,   gb_pixmap_rgba5551_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgba5551_pixel_set_la
,   gb_pixmap_rgba5551_pixel_cpy_la
,   gb_pixmap_rgba5551_color_get_l
,   gb_pixmap_rgba5551_color_set_la
,   gb_pixmap_rgba5551_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_rgba5551 =
{   
    "rgba5551"
,   16
,   2
,   GB_PIXFMT_RGBA5551 | GB_PIXFMT_BENDIAN
,   gb_pixmap_rgba5551_pixel
,   gb_pixmap_rgba5551_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgba5551_pixel_set_ba
,   gb_pixmap_rgba5551_pixel_cpy_ba
,   gb_pixmap_rgba5551_color_get_b
,   gb_pixmap_rgba5551_color_set_ba
,   gb_pixmap_rgba5551_pixels_fill_ba
};

#endif

