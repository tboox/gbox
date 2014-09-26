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
 * @file        stroker.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "stroker.h"
#include "../path.h"
#include "../paint.h"
#include "../matrix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the stroker impl type 
typedef struct __gb_stroker_impl_t
{
    // the cap
    tb_size_t               cap;

    // the join
    tb_size_t               join;

    // the radius
    gb_float_t              radius;

    // the outer path and is the stroked path
    gb_path_ref_t           path_outer;

    // the inner path and is the temporary path
    gb_path_ref_t           path_inner;

    // the other command contours path 
    gb_path_ref_t           path_other;

}gb_stroker_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_stroker_ref_t gb_stroker_init()
{
    // done
    tb_bool_t           ok = tb_false;
    gb_stroker_impl_t*  impl = tb_null;
    do
    {
        // make stroker
        impl = tb_malloc0_type(gb_stroker_impl_t);
        tb_assert_and_check_break(impl);

        // init stroker
        impl->cap       = GB_PAINT_CAP_BUTT;
        impl->join      = GB_PAINT_JOIN_MITER;
        impl->radius    = 0;

        // init the outer path
        impl->path_outer = gb_path_init();
        tb_assert_and_check_break(impl->path_outer);
    
        // init the inner path
        impl->path_inner = gb_path_init();
        tb_assert_and_check_break(impl->path_inner);
   
        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_stroker_exit((gb_stroker_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_stroker_ref_t)impl;
}
tb_void_t gb_stroker_exit(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl);

    // exit the other path
    if (impl->path_other) gb_path_exit(impl->path_other);
    impl->path_other = tb_null;

    // exit the inner path
    if (impl->path_inner) gb_path_exit(impl->path_inner);
    impl->path_inner = tb_null;

    // exit the outer path
    if (impl->path_outer) gb_path_exit(impl->path_outer);
    impl->path_outer = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_stroker_clear(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl);

    // clear it
    impl->cap       = GB_PAINT_CAP_BUTT;
    impl->join      = GB_PAINT_JOIN_MITER;
    impl->radius    = 0;

    // clear the other path
    if (impl->path_other) gb_path_clear(impl->path_other);

    // clear the inner path
    if (impl->path_inner) gb_path_clear(impl->path_inner);

    // clear the outer path
    if (impl->path_outer) gb_path_clear(impl->path_outer);
}
tb_void_t gb_stroker_apply_paint(gb_stroker_ref_t stroker, gb_paint_ref_t paint)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && paint);

    // the width
    gb_float_t width = gb_paint_width(paint);

    // set the cap
    impl->cap = gb_paint_cap(paint);

    // set the join
    impl->join = gb_paint_join(paint);

    // set the radius
    impl->radius = gb_rsh(width, 1);
}
tb_void_t gb_stroker_clos(gb_stroker_ref_t stroker)
{
}
tb_void_t gb_stroker_move_to(gb_stroker_ref_t stroker, gb_point_ref_t point)
{
}
tb_void_t gb_stroker_line_to(gb_stroker_ref_t stroker, gb_point_ref_t point)
{
}
tb_void_t gb_stroker_quad_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl, gb_point_ref_t point)
{
}
tb_void_t gb_stroker_cube_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point)
{
}
tb_void_t gb_stroker_add_path(gb_stroker_ref_t stroker, gb_path_ref_t path)
{
    // done
    tb_for_all_if (gb_path_item_ref_t, item, path, item)
    {
        switch (item->code)
        {
        case GB_PATH_CODE_MOVE:
            gb_stroker_move_to(stroker, &item->point);
            break;
        case GB_PATH_CODE_LINE:
            gb_stroker_line_to(stroker, &item->point);
            break;
        case GB_PATH_CODE_QUAD:
            gb_stroker_quad_to(stroker, &item->ctrls[0], &item->point);
            break;
        case GB_PATH_CODE_CUBE:
            gb_stroker_cube_to(stroker, &item->ctrls[0], &item->ctrls[1], &item->point);
            break;
        case GB_PATH_CODE_CLOS:
            gb_stroker_clos(stroker);
            break;
        default:
            // trace
            tb_trace_e("invalid code: %lu", item->code);
            break;
        }
    }
}
tb_void_t gb_stroker_add_lines(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count && !(count & 0x1));

    // done
    tb_size_t index;
    for (index = 0; index < count; index += 2)
    {
        gb_stroker_move_to(stroker, points + index);
        gb_stroker_line_to(stroker, points + index + 1);
    }
}
tb_void_t gb_stroker_add_points(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_abort(impl && points && count);

    // make the other path if not exists
    if (!impl->path_other) impl->path_other = gb_path_init();
    tb_assert_and_check_return(impl->path_other);

    // make the stroked path
    switch (impl->cap)
    {
    case GB_PAINT_CAP_ROUND:
        {
            // done
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_circle_t     circle;
            gb_float_t      radius = impl->radius;
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make circle
                circle = gb_circle_make(point->x, point->y, radius);

                // add circle to the path
                gb_path_add_circle(impl->path_other, &circle, GB_PATH_DIRECTION_CW);
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
            gb_float_t      radius = impl->radius;
            gb_float_t      width = gb_lsh(radius, 1);
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make rect
                rect = gb_rect_make(point->x - radius, point->y - radius, width, width);

                // add rect to the path
                gb_path_add_rect(impl->path_other, &rect, GB_PATH_DIRECTION_CW);
            }
        }
        break;
    }
}
tb_void_t gb_stroker_add_polygon(gb_stroker_ref_t stroker, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_and_check_return(polygon && polygon->points && polygon->counts);

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
            gb_stroker_move_to(stroker, point);
            first = point;
        }
        else gb_stroker_line_to(stroker, point);

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // close path
            if (first && first->x == point->x && first->y == point->y) gb_stroker_clos(stroker);

            // next
            count = *counts++;
            index = 0;
        }
    }
}
gb_path_ref_t gb_stroker_done(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO merge the inner path to the outer path
    // ...

    // exists the other path? merge it
    if (impl->path_other && !gb_path_null(impl->path_other))
    {
        // add the other path
        gb_path_add_path(impl->path_outer, impl->path_other);

        // clear the other path
        gb_path_clear(impl->path_other);
    }

    // the stroked path
    return impl->path_outer;
}
gb_path_ref_t gb_stroker_done_path(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_path_ref_t path)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add path to the stroker
    gb_stroker_add_path(stroker, path);

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_lines(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add lines to the stroker
    gb_stroker_add_lines(stroker, points, count);

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_points(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add points to the stroker
    gb_stroker_add_points(stroker, points, count);

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_polygon(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_polygon_ref_t polygon)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add polygon to the stroker
    gb_stroker_add_polygon(stroker, polygon);

    // done the stroker
    return gb_stroker_done(stroker);
}
