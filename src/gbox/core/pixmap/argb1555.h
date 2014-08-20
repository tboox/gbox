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
 * @file        argb1555.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_ARGB1555_H
#define GB_CORE_PIXMAP_ARGB1555_H

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
 * 0000 0000 0000 0000 arrr rrgg gggb bbbb
 *
 * c | c << 15:
 * 0arr rrrg gggg bbbb arrr rrgg gggb bbbb
 *
 * 0x41f07c1f:
 * 0100 0001 1111 0000 0111 1100 0001 1111
 *
 * d = (c | c << 15) & 0x41f07c1f:
 * 0a00 000g gggg 0000 0rrr rr00 000b bbbb
 *
 * (d & 0xffff) | (d >> 15):
 * 0000 0000 0000 0000 arrr rrgg gggb bbbb
 *
 * (s * a + d * (32 - a)) >> 5 => ((s - d) * a) >> 5 + d
 */
static __tb_inline__ tb_uint16_t gb_pixmap_argb1555_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    // FIXME: s - d? overflow?
    s = (s | (s << 15)) & 0x41f07c1f;
    d = (d | (d << 15)) & 0x41f07c1f;
    d = ((((s - d) * a) >> 5) + d) & 0x41f07c1f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 15) | 0x8000);
}
static __tb_inline__ tb_uint16_t gb_pixmap_argb1555_blend2(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    d = (d | (d << 15)) & 0x41f07c1f;
    d = ((((s - d) * a) >> 5) + d) & 0x41f07c1f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 15) | 0x8000);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_argb1555_pixel(gb_color_t color)
{
    return GB_ARGB_1555(color.r, color.g, color.b, color.a);
}
static __tb_inline__ gb_color_t gb_pixmap_argb1555_color(gb_pixel_t pixel)
{
    gb_color_t color;
    color.a = GB_ARGB_1555_A(pixel);
    color.r = GB_ARGB_1555_R(pixel);
    color.g = GB_ARGB_1555_G(pixel);
    color.b = GB_ARGB_1555_B(pixel);
    return color;
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixel_set_la(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_le(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixel_set_ba(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_be(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_le(data), tb_bits_get_u16_le(source), alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_be(data), tb_bits_get_u16_be(source), alpha >> 3));
}
static __tb_inline__ gb_color_t gb_pixmap_argb1555_color_get_l(tb_cpointer_t data)
{
    return gb_pixmap_argb1555_color(tb_bits_get_u16_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_argb1555_color_get_b(tb_cpointer_t data)
{
    return gb_pixmap_argb1555_color(tb_bits_get_u16_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_color_set_lo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb1555_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_color_set_bo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb1555_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_color_set_la(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_le(data), gb_pixmap_argb1555_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_color_set_ba(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb1555_blend(tb_bits_get_u16_be(data), gb_pixmap_argb1555_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x41f07c1f;
    while (p < e)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_argb1555_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        tb_bits_set_u16_le(&p[1], gb_pixmap_argb1555_blend2(tb_bits_get_u16_le(&p[1]), s, alpha));
        tb_bits_set_u16_le(&p[2], gb_pixmap_argb1555_blend2(tb_bits_get_u16_le(&p[2]), s, alpha));
        tb_bits_set_u16_le(&p[3], gb_pixmap_argb1555_blend2(tb_bits_get_u16_le(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_argb1555_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        p++;
    }
}
static __tb_inline__ tb_void_t gb_pixmap_argb1555_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x41f07c1f;
    while (p < e)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_argb1555_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        tb_bits_set_u16_be(&p[1], gb_pixmap_argb1555_blend2(tb_bits_get_u16_be(&p[1]), s, alpha));
        tb_bits_set_u16_be(&p[2], gb_pixmap_argb1555_blend2(tb_bits_get_u16_be(&p[2]), s, alpha));
        tb_bits_set_u16_be(&p[3], gb_pixmap_argb1555_blend2(tb_bits_get_u16_be(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_argb1555_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        p++;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_argb1555 =
{   
    "argb1555"
,   16
,   2
,   GB_PIXFMT_ARGB1555 | GB_PIXFMT_LENDIAN
,   gb_pixmap_argb1555_pixel
,   gb_pixmap_argb1555_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgb16_pixel_set_lo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_argb1555_color_get_l
,   gb_pixmap_argb1555_color_set_lo
,   gb_pixmap_rgb16_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_argb1555 =
{   
    "argb1555"
,   16
,   2
,   GB_PIXFMT_ARGB1555 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb1555_pixel
,   gb_pixmap_argb1555_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgb16_pixel_set_bo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_argb1555_color_get_b
,   gb_pixmap_argb1555_color_set_bo
,   gb_pixmap_rgb16_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_argb1555 =
{   
    "argb1555"
,   16
,   2
,   GB_PIXFMT_ARGB1555 | GB_PIXFMT_LENDIAN
,   gb_pixmap_argb1555_pixel
,   gb_pixmap_argb1555_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_argb1555_pixel_set_la
,   gb_pixmap_argb1555_pixel_cpy_la
,   gb_pixmap_argb1555_color_get_l
,   gb_pixmap_argb1555_color_set_la
,   gb_pixmap_argb1555_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_argb1555 =
{   
    "argb1555"
,   16
,   2
,   GB_PIXFMT_ARGB1555 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb1555_pixel
,   gb_pixmap_argb1555_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_argb1555_pixel_set_ba
,   gb_pixmap_argb1555_pixel_cpy_ba
,   gb_pixmap_argb1555_color_get_b
,   gb_pixmap_argb1555_color_set_ba
,   gb_pixmap_argb1555_pixels_fill_ba
};

#endif

