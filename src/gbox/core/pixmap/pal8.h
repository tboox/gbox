/*!The Graphic 2D Box Library
 * 
 * GBox2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox2; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author      ruki
 * @file        pal8.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_PAL8_H
#define GB_CORE_PIXMAP_PAL8_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

static gb_pixel_t gb_pixmap_pal8_pixel(gb_color_t color)
{
    tb_trace_noimpl();
    return 0;
}
static gb_color_t gb_pixmap_pal8_color(gb_pixel_t pixel)
{
    tb_trace_noimpl();
    return GB_COLOR_DEFAULT;
}
static gb_pixel_t gb_pixmap_pal8_pixel_get(tb_cpointer_t data)
{
    return ((tb_byte_t*)data)[0];
}
static tb_void_t gb_pixmap_pal8_pixel_set_o(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    ((tb_byte_t*)data)[0] = (tb_byte_t)(pixel & 0xff);
}
static tb_void_t gb_pixmap_pal8_pixel_set_a(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    ((tb_byte_t*)data)[0] = (tb_byte_t)(pixel & 0xff);
}
static tb_void_t gb_pixmap_pal8_pixel_cpy_o(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    ((tb_byte_t*)data)[0] = ((tb_byte_t*)source)[0];
}
static tb_void_t gb_pixmap_pal8_pixel_cpy_a(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    ((tb_byte_t*)data)[0] = ((tb_byte_t*)source)[0];
}
static gb_color_t gb_pixmap_pal8_color_get(tb_cpointer_t data)
{
    tb_trace_noimpl();
    return GB_COLOR_DEFAULT;
}
static tb_void_t gb_pixmap_pal8_color_set_o(tb_pointer_t data, gb_color_t color)
{
    tb_trace_noimpl();
}
static tb_void_t gb_pixmap_pal8_color_set_a(tb_pointer_t data, gb_color_t color)
{
    tb_trace_noimpl();
}
static tb_void_t gb_pixmap_pal8_pixels_fill_o(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset(data, (tb_byte_t)pixel, count);
}
static tb_void_t gb_pixmap_pal8_pixels_fill_a(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset(data, (tb_byte_t)pixel, count);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_o_pal8 =
{   
    "pal8"
,   8
,   1
,   GB_PIXFMT_PAL8
,   gb_pixmap_pal8_pixel
,   gb_pixmap_pal8_color
,   gb_pixmap_pal8_pixel_get
,   gb_pixmap_pal8_pixel_set_o
,   gb_pixmap_pal8_pixel_cpy_o
,   gb_pixmap_pal8_color_get
,   gb_pixmap_pal8_color_set_o
,   gb_pixmap_pal8_pixels_fill_o
};

static gb_pixmap_t const g_pixmap_a_pal8 =
{   
    "pal8"
,   8
,   1
,   GB_PIXFMT_PAL8
,   gb_pixmap_pal8_pixel
,   gb_pixmap_pal8_color
,   gb_pixmap_pal8_pixel_get
,   gb_pixmap_pal8_pixel_set_a
,   gb_pixmap_pal8_pixel_cpy_a
,   gb_pixmap_pal8_color_get
,   gb_pixmap_pal8_color_set_a
,   gb_pixmap_pal8_pixels_fill_a
};

#endif

