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
 * @file        rgb32.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGB32_H
#define GB_CORE_PIXMAP_RGB32_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

static __tb_inline__ tb_uint32_t gb_pixmap_rgb32_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    tb_uint32_t hs = (s >> 8) & 0x00ff00ff;
    tb_uint32_t hd = (d >> 8) & 0x00ff00ff;
    tb_uint32_t ls = s & 0x00ff00ff;
    tb_uint32_t ld = d & 0x00ff00ff;
    hd = (((a * (hs - hd)) >> 8) + hd) & 0x00ff00ff;
    ld = (((a * (ls - ld)) >> 8) + ld) & 0x00ff00ff;
    return (hd << 8) | ld;
}
static __tb_inline__ tb_uint32_t gb_pixmap_rgb32_blend2(tb_uint32_t d, tb_uint32_t hs, tb_uint32_t ls, tb_byte_t a)
{
    tb_uint32_t hd = (d >> 8) & 0x00ff00ff;
    tb_uint32_t ld = d & 0x00ff00ff;
    hd = (((a * (hs - hd)) >> 8) + hd) & 0x00ff00ff;
    ld = (((a * (ls - ld)) >> 8) + ld) & 0x00ff00ff;
    return (hd << 8) | ld;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel(gb_color_t color)
{
    return gb_color_pixel(color);
}
static __tb_inline__ gb_color_t gb_pixmap_rgb32_color(gb_pixel_t pixel)
{
    return gb_pixel_color(pixel);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel_get_l(tb_cpointer_t data)
{
    return tb_bits_get_u32_le(data);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel_get_b(tb_cpointer_t data)
{
    return tb_bits_get_u32_be(data);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_set_lo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_le(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_set_bo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_be(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_cpy_o(tb_pointer_t data, tb_cpointer_t pixel, tb_byte_t alpha)
{
    *((tb_uint32_t*)data) = *((tb_uint32_t*)pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixels_set_lo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u32(data, tb_bits_ne_to_le_u32(pixel), count);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixels_set_bo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u32(data, tb_bits_ne_to_be_u32(pixel), count);
}


#endif

