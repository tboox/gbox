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
 * @file        color.h
 * @ingroup     core
 */
#ifndef GB_CORE_DEVICE_SKIA_COLOR_H
#define GB_CORE_DEVICE_SKIA_COLOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */
static __tb_inline__ gb_color_t gb_skia_color_to_gb(SkColor color)
{
    union __gb_p2c_t
    {
        gb_color_t c;
        gb_pixel_t p;

    }p2c;
    p2c.p = color;
    return p2c.c;
}
static __tb_inline__ SkColor gb_skia_color_to_sk(gb_color_t color)
{
    union __gb_c2p_t
    {
        gb_color_t c;
        gb_pixel_t p;

    }c2p;
    c2p.c = color;
    return static_cast<SkColor>(c2p.p);
}

#endif


