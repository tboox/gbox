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
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        rgba4444.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGBA4444_H
#define GB_CORE_PIXMAP_RGBA4444_H

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
 * 0000 0000 0000 0000 rrrr gggg bbbb aaaa
 *
 * c | c << 12:
 * 0000 rrrr gggg bbbb rrrr gggg bbbb aaaa
 *
 * 0x0f0f0f0f:
 * 0000 1111 0000 1111 0000 1111 0000 1111
 *
 * d = (c | c << 12) & 0x0f0f0f0f:
 * 0000 rrrr 0000 bbbb 0000 gggg 0000 aaaa
 *
 * (d & 0xffff) | (d >> 12):
 * 0000 0000 0000 0000 rrrr gggg bbbb aaaa
 *
 * (s * a + d * (32 - a)) >> 5 => ((s - d) * a) >> 5 + d
 */
static __tb_inline__ tb_uint16_t gb_pixmap_rgba4444_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    // FIXME: s - d? overflow?
    s = (s | (s << 12)) & 0x0f0f0f0f;
    d = (d | (d << 12)) & 0x0f0f0f0f;
    d = ((((s - d) * a) >> 5) + d) & 0x0f0f0f0f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 12));
}
static __tb_inline__ tb_uint16_t gb_pixmap_rgba4444_blend2(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    d = (d | (d << 12)) & 0x0f0f0f0f;
    d = ((((s - d) * a) >> 5) + d) & 0x0f0f0f0f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 12));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgba4444_pixel(gb_color_t color)
{
    return GB_RGBA_4444(color.r, color.g, color.b, color.a);
}
static __tb_inline__ gb_color_t gb_pixmap_rgba4444_color(gb_pixel_t pixel)
{
    gb_color_t color;
    color.r = GB_RGBA_4444_R(pixel);
    color.g = GB_RGBA_4444_G(pixel);
    color.b = GB_RGBA_4444_B(pixel);
    color.a = GB_RGBA_4444_A(pixel);
    return color;
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixel_set_la(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_le(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixel_set_ba(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_be(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_le(data), tb_bits_get_u16_le(source), alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_be(data), tb_bits_get_u16_be(source), alpha >> 3));
}
static __tb_inline__ gb_color_t gb_pixmap_rgba4444_color_get_l(tb_cpointer_t data)
{
    return gb_pixmap_rgba4444_color(tb_bits_get_u16_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_rgba4444_color_get_b(tb_cpointer_t data)
{
    return gb_pixmap_rgba4444_color(tb_bits_get_u16_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_color_set_lo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba4444_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_color_set_bo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba4444_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_color_set_la(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_le(data), gb_pixmap_rgba4444_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_color_set_ba(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_rgba4444_blend(tb_bits_get_u16_be(data), gb_pixmap_rgba4444_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x0f00f0f0;
    while (p < e)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        tb_bits_set_u16_le(&p[1], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_le(&p[1]), s, alpha));
        tb_bits_set_u16_le(&p[2], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_le(&p[2]), s, alpha));
        tb_bits_set_u16_le(&p[3], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_le(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        p++;
    }
}
static __tb_inline__ tb_void_t gb_pixmap_rgba4444_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x0f00f0f0;
    while (p < e)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        tb_bits_set_u16_be(&p[1], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_be(&p[1]), s, alpha));
        tb_bits_set_u16_be(&p[2], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_be(&p[2]), s, alpha));
        tb_bits_set_u16_be(&p[3], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_be(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_rgba4444_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        p++;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_rgba4444 =
{   
    "rgba4444"
,   16
,   2
,   GB_PIXFMT_RGBA4444 | GB_PIXFMT_LENDIAN
,   gb_pixmap_rgba4444_pixel
,   gb_pixmap_rgba4444_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgb16_pixel_set_lo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_rgba4444_color_get_l
,   gb_pixmap_rgba4444_color_set_lo
,   gb_pixmap_rgb16_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_rgba4444 =
{   
    "rgba4444"
,   16
,   2
,   GB_PIXFMT_RGBA4444 | GB_PIXFMT_BENDIAN
,   gb_pixmap_rgba4444_pixel
,   gb_pixmap_rgba4444_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgb16_pixel_set_bo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_rgba4444_color_get_b
,   gb_pixmap_rgba4444_color_set_bo
,   gb_pixmap_rgb16_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_rgba4444 =
{   
    "rgba4444"
,   16
,   2
,   GB_PIXFMT_RGBA4444 | GB_PIXFMT_LENDIAN
,   gb_pixmap_rgba4444_pixel
,   gb_pixmap_rgba4444_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgba4444_pixel_set_la
,   gb_pixmap_rgba4444_pixel_cpy_la
,   gb_pixmap_rgba4444_color_get_l
,   gb_pixmap_rgba4444_color_set_la
,   gb_pixmap_rgba4444_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_rgba4444 =
{   
    "rgba4444"
,   16
,   2
,   GB_PIXFMT_RGBA4444 | GB_PIXFMT_BENDIAN
,   gb_pixmap_rgba4444_pixel
,   gb_pixmap_rgba4444_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgba4444_pixel_set_ba
,   gb_pixmap_rgba4444_pixel_cpy_ba
,   gb_pixmap_rgba4444_color_get_b
,   gb_pixmap_rgba4444_color_set_ba
,   gb_pixmap_rgba4444_pixels_fill_ba
};


#endif

