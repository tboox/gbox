/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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

