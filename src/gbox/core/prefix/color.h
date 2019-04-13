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
 * @file        color.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_COLOR_H
#define GB_CORE_PREFIX_COLOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
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
#define GB_COLOR_DEFAULT                GB_COLOR_LIGHTGRAY

/* //////////////////////////////////////////////////////////////////////////////////////
 * inline
 */
__tb_extern_c_enter__

/*! make color
 *
 * @param a                         the alpha
 * @param r                         the red
 * @param g                         the green
 * @param b                         the blue
 *
 * @return                          the color
 */
static __tb_inline__ gb_color_t     gb_color_make(tb_byte_t a, tb_byte_t r, tb_byte_t g, tb_byte_t b)
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
 * @param color                     the color
 *
 * @return                          the pixel value
 */
static __tb_inline__ tb_uint32_t    gb_color_pixel(gb_color_t color)
{
    union __gb_c2p_t
    {
        gb_color_t c;
        gb_pixel_t p;

    }c2p;
    c2p.c = color;
    return c2p.p;
}

/*! convert pixel to the color
 *
 * @param pixel                     the pixel value
 *
 * @return                          the color
 */
static __tb_inline__ gb_color_t     gb_pixel_color(gb_pixel_t pixel)
{
    union __gb_p2c_t
    {
        gb_color_t c;
        gb_pixel_t p;

    }p2c;
    p2c.p = pixel;
    return p2c.c;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get color from the name
 *
 * @param name      the color name
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
#endif


