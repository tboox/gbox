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


