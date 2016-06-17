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
 * @file        bounds.h
 * @ingroup     core
 */
#ifndef GB_CORE_IMPL_BOUNDS_H
#define GB_CORE_IMPL_BOUNDS_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* make bounds from the points
 *
 * @param bounds                the bounds
 * @param points                the points 
 * @param count                 the points count
 */
static __tb_inline__ tb_void_t  gb_bounds_make(gb_rect_ref_t bounds, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert(bounds && points && count);

    // the point
    gb_point_ref_t point = points++; count--;

    // done
    gb_float_t x0 = point->x;
    gb_float_t y0 = point->y;
    gb_float_t x1 = x0;
    gb_float_t y1 = y0;
    gb_float_t x;
    gb_float_t y;
    while (count--)
    {
        // the point
        x = points->x;
        y = points->y;

        // make minimum and maximum point
        if (x < x0) x0 = x;
        if (y < y0) y0 = y;
        if (x > x1) x1 = x;
        if (y > y1) y1 = y;

        // next point
        points++;
    }

    // make bounds
    bounds->x = x0;
    bounds->y = y0;
    bounds->w = x1 - x0;
    bounds->h = y1 - y0;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


