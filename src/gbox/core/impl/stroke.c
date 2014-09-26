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
 * @file        stroke.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "stroke.h"
#include "../path.h"
#include "../paint.h"
#include "../matrix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_stroke_make_fill_for_lines(gb_path_ref_t path, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    tb_trace_noimpl();
}
tb_void_t gb_stroke_make_fill_for_points(gb_path_ref_t path, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(path && paint && points && count);

    // clear path
    gb_path_clear(path);

    // the width
    gb_float_t width = gb_paint_width(paint);

    // the cap
    tb_size_t cap = gb_paint_cap(paint);

    // make the cap path for points
    switch (cap)
    {
    case GB_PAINT_CAP_ROUND:
        {
            // done
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_circle_t     circle;
            gb_float_t      width_half = gb_rsh(width, 1);
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make circle
                circle = gb_circle_make(point->x, point->y, width_half);

                // add circle to the path
                gb_path_add_circle(path, &circle, GB_PATH_DIRECTION_CW);
            }
        }
        break;
    case GB_PAINT_CAP_BUTT:
    case GB_PAINT_CAP_SQUARE:
    default:
        {
            // done
            gb_rect_t       rect;
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_float_t      width_half = gb_rsh(width, 1);
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make rect
                rect = gb_rect_make(point->x - width_half, point->y - width_half, width, width);

                // add rect to the path
                gb_path_add_rect(path, &rect, GB_PATH_DIRECTION_CW);
            }
        }
        break;
    }
}
tb_void_t gb_stroke_make_fill_for_polygon(gb_path_ref_t path, gb_paint_ref_t paint, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_abort(path && paint && polygon && polygon->points && polygon->counts);

    // clear path
    gb_path_clear(path);

    // done
    gb_point_ref_t  first = tb_null;
    gb_point_ref_t  point = tb_null;
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    tb_size_t       index = 0;
    while (index < count)
    {
        // the point
        point = points++;
        
        // first point?
        if (!index) 
        {
            gb_path_move_to(path, point);
            first = point;
        }
        else gb_path_line_to(path, point);

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // close path
            if (first && first->x == point->x && first->y == point->y) gb_path_clos(path);

            // next
            count = *counts++;
            index = 0;
        }
    }
}
