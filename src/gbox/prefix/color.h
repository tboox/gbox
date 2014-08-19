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
 * @file        color.h
 *
 */
#ifndef GB_PREFIX_COLOR_H
#define GB_PREFIX_COLOR_H

// c plus plus
#ifdef __cplusplus
extern "C" {
#endif

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* ///////////////////////////////////////////////////////////////////////
 * macros
 */

// the color contants
#define GB_COLOR_BLACK                  gb_color_from_index(0)
#define GB_COLOR_BLUE                   gb_color_from_index(1)
#define GB_COLOR_BROWN                  gb_color_from_index(2)
#define GB_COLOR_DARKBLUE               gb_color_from_index(3)
#define GB_COLOR_GOLD                   gb_color_from_index(4)
#define GB_COLOR_GRAY                   gb_color_from_index(5)
#define GB_COLOR_GREEN                  gb_color_from_index(6)
#define GB_COLOR_GREY                   gb_color_from_index(7)
#define GB_COLOR_LIGHTBLUE              gb_color_from_index(8)
#define GB_COLOR_LIGHTGRAY              gb_color_from_index(9)
#define GB_COLOR_LIGHTGREY              gb_color_from_index(10)
#define GB_COLOR_LIGHTPINK              gb_color_from_index(11)
#define GB_COLOR_LIGHTYELLOW            gb_color_from_index(12)
#define GB_COLOR_NAVY                   gb_color_from_index(13)
#define GB_COLOR_ORANGE                 gb_color_from_index(14)
#define GB_COLOR_PINK                   gb_color_from_index(15)
#define GB_COLOR_PURPLE                 gb_color_from_index(16)
#define GB_COLOR_RED                    gb_color_from_index(17)
#define GB_COLOR_SNOW                   gb_color_from_index(18)
#define GB_COLOR_TOMATO                 gb_color_from_index(19)
#define GB_COLOR_YELLOW                 gb_color_from_index(20)
#define GB_COLOR_WHEAT                  gb_color_from_index(21)
#define GB_COLOR_WHITE                  gb_color_from_index(22)
#define GB_COLOR_DEFAULT                GB_COLOR_BLACK

/* ////////////////////////////////////////////////////////////////////////
 * inline
 */

/*! make color
 *
 * @param a                     the alpha
 * @param r                     the red
 * @param g                     the green
 * @param b                     the blue
 *
 * @return                      the color
 */
static __tb_inline__ gb_color_t gb_color_make(tb_byte_t a, tb_byte_t r, tb_byte_t g, tb_byte_t b)
{
    gb_color_t c;

    c.a = a;
    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}

/*! convert color to the pixel
 *
 * @param color                 the color
 *
 * @return                      the pixel value
 */
static __tb_inline__ tb_uint32_t gb_color_pixel(gb_color_t color)
{
    union __gb_c2p_t
    {
        gb_color_t  c;
        gb_pixel_t p;

    }c2p;
    c2p.c = color;
    return c2p.p;
}

/*! convert pixel to the color
 *
 * @param pixel                 the pixel value
 *
 * @return                      the color
 */
static __tb_inline__ gb_color_t gb_pixel_color(gb_pixel_t pixel)
{
    union __gb_p2c_t
    {
        gb_color_t  c;
        gb_pixel_t p;

    }p2c;
    p2c.p = pixel;
    return p2c.c;
}

/* ////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! get color from the name
 *
 * @param name     the color name
 *
 * @return          the color
 */
gb_color_t const    gb_color_from_name(tb_char_t const* name);

/*! get color from the index
 *
 * @param index     the color index
 *
 * @return          the color
 */
gb_color_t const    gb_color_from_index(tb_size_t index);

__tb_extern_c_leave__
// c plus plus
#ifdef __cplusplus
}
#endif


#endif


