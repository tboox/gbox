/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        argb8888.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_ARGB8888_H
#define GB_CORE_PIXMAP_ARGB8888_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "rgb32.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_argb8888_pixel(gb_color_t color)
{
    return gb_pixmap_rgb32_pixel(color);
}
static __tb_inline__ gb_color_t gb_pixmap_argb8888_color(gb_pixel_t pixel)
{
    return gb_pixmap_rgb32_color(pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixel_set_la(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_le(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_le(data), pixel, alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixel_set_ba(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_be(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_be(data), pixel, alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u32_le(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_le(data), tb_bits_get_u32_le(source), alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u32_be(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_be(data), tb_bits_get_u32_be(source), alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_color_set_lo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u32_le(data, gb_pixmap_argb8888_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_color_set_bo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u32_be(data, gb_pixmap_argb8888_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_color_set_la(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u32_le(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_le(data), gb_pixmap_argb8888_pixel(color), color.a));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_color_set_ba(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u32_be(data, gb_pixmap_rgb32_blend(tb_bits_get_u32_be(data), gb_pixmap_argb8888_pixel(color), color.a));
}
static __tb_inline__ gb_color_t gb_pixmap_argb8888_color_get_l(tb_cpointer_t data)
{
    return gb_pixmap_argb8888_color(tb_bits_get_u32_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_argb8888_color_get_b(tb_cpointer_t data)
{
    return gb_pixmap_argb8888_color(tb_bits_get_u32_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l;
    tb_uint32_t*    p = (tb_uint32_t*)data;
    tb_uint32_t*    e = p + count;
    tb_uint32_t     hs = (pixel >> 8) & 0x00ff00ff;
    tb_uint32_t     ls = pixel & 0x00ff00ff;
    while (p < e)
    {
        tb_bits_set_u32_le(&p[0], gb_pixmap_rgb32_blend2(tb_bits_get_u32_le(&p[0]), hs, ls, alpha));
        tb_bits_set_u32_le(&p[1], gb_pixmap_rgb32_blend2(tb_bits_get_u32_le(&p[1]), hs, ls, alpha));
        tb_bits_set_u32_le(&p[2], gb_pixmap_rgb32_blend2(tb_bits_get_u32_le(&p[2]), hs, ls, alpha));
        tb_bits_set_u32_le(&p[3], gb_pixmap_rgb32_blend2(tb_bits_get_u32_le(&p[3]), hs, ls, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u32_le(&p[0], gb_pixmap_rgb32_blend2(tb_bits_get_u32_le(&p[0]), hs, ls, alpha));
        p++;
    }
}
static __tb_inline__ tb_void_t gb_pixmap_argb8888_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l;
    tb_uint32_t*    p = (tb_uint32_t*)data;
    tb_uint32_t*    e = p + count;
    tb_uint32_t     hs = (pixel >> 8) & 0x00ff00ff;
    tb_uint32_t     ls = pixel & 0x00ff00ff;
    while (p < e)
    {
        tb_bits_set_u32_be(&p[0], gb_pixmap_rgb32_blend2(tb_bits_get_u32_be(&p[0]), hs, ls, alpha));
        tb_bits_set_u32_be(&p[1], gb_pixmap_rgb32_blend2(tb_bits_get_u32_be(&p[1]), hs, ls, alpha));
        tb_bits_set_u32_be(&p[2], gb_pixmap_rgb32_blend2(tb_bits_get_u32_be(&p[2]), hs, ls, alpha));
        tb_bits_set_u32_be(&p[3], gb_pixmap_rgb32_blend2(tb_bits_get_u32_be(&p[3]), hs, ls, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u32_be(&p[0], gb_pixmap_rgb32_blend2(tb_bits_get_u32_be(&p[0]), hs, ls, alpha));
        p++;
    }
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_argb8888 =
{   
    "argb8888"
,   32
,   4
,   GB_PIXFMT_ARGB8888 | GB_PIXFMT_LENDIAN
,   gb_pixmap_argb8888_pixel
,   gb_pixmap_argb8888_color
,   gb_pixmap_rgb32_pixel_get_l
,   gb_pixmap_rgb32_pixel_set_lo
,   gb_pixmap_rgb32_pixel_cpy_o
,   gb_pixmap_argb8888_color_get_l
,   gb_pixmap_argb8888_color_set_lo
,   gb_pixmap_rgb32_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_argb8888 =
{   
    "argb8888"
,   32
,   4
,   GB_PIXFMT_ARGB8888 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb8888_pixel
,   gb_pixmap_argb8888_color
,   gb_pixmap_rgb32_pixel_get_b
,   gb_pixmap_rgb32_pixel_set_bo
,   gb_pixmap_rgb32_pixel_cpy_o
,   gb_pixmap_argb8888_color_get_b
,   gb_pixmap_argb8888_color_set_bo
,   gb_pixmap_rgb32_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_argb8888 =
{   
    "argb8888"
,   32
,   4
,   GB_PIXFMT_ARGB8888 | GB_PIXFMT_LENDIAN
,   gb_pixmap_argb8888_pixel
,   gb_pixmap_argb8888_color
,   gb_pixmap_rgb32_pixel_get_l
,   gb_pixmap_argb8888_pixel_set_la
,   gb_pixmap_argb8888_pixel_cpy_la
,   gb_pixmap_argb8888_color_get_l
,   gb_pixmap_argb8888_color_set_la
,   gb_pixmap_argb8888_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_argb8888 =
{   
    "argb8888"
,   32
,   4
,   GB_PIXFMT_ARGB8888 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb8888_pixel
,   gb_pixmap_argb8888_color
,   gb_pixmap_rgb32_pixel_get_b
,   gb_pixmap_argb8888_pixel_set_ba
,   gb_pixmap_argb8888_pixel_cpy_ba
,   gb_pixmap_argb8888_color_get_b
,   gb_pixmap_argb8888_color_set_ba
,   gb_pixmap_argb8888_pixels_fill_ba
};


#endif

