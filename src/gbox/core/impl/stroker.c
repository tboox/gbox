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

    // the miter limit
    gb_float_t              miter;

    // the outer path and is the stroked path
    gb_path_ref_t           path_outer;

    // the inner path and is the temporary path
    gb_path_ref_t           path_inner;

    // the other command contours path 
    gb_path_ref_t           path_other;

}gb_stroker_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_stroker_add_hint(gb_stroker_ref_t stroker, gb_shape_ref_t hint)
{
    // check
    tb_check_return_val(hint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    switch (hint->type)
    {
    case GB_SHAPE_TYPE_RECT:
        {
            gb_stroker_add_rect(stroker, &hint->u.rect);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_LINE:
        {
            gb_point_t points[2];
            points[0] = hint->u.line.p0;
            points[1] = hint->u.line.p1;
            gb_stroker_add_lines(stroker, points, tb_arrayn(points));
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_CIRCLE:
        {
            gb_stroker_add_circle(stroker, &hint->u.circle);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_ELLIPSE:
        {
            gb_stroker_add_ellipse(stroker, &hint->u.ellipse);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_ARC:
        {
            gb_stroker_add_arc(stroker, &hint->u.arc);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_POINT:
        {
            gb_stroker_add_lines(stroker, &hint->u.point, 1);
            ok = tb_true;
        }
        break;
    default:
        break;
    }

    // ok?
    return ok;
}

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
        impl->cap       = GB_PAINT_STROKE_CAP_BUTT;
        impl->join      = GB_PAINT_STROKE_JOIN_MITER;
        impl->radius    = 0;

        // init the outer path
        impl->path_outer = gb_path_init();
        tb_assert_and_check_break(impl->path_outer);
    
        // init the inner path
        impl->path_inner = gb_path_init();
        tb_assert_and_check_break(impl->path_inner);
   
        // init the other path
        impl->path_other = gb_path_init();
        tb_assert_and_check_break(impl->path_other);

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
    impl->cap       = GB_PAINT_STROKE_CAP_BUTT;
    impl->join      = GB_PAINT_STROKE_JOIN_MITER;
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
    gb_float_t width = gb_paint_stroke_width(paint);
    tb_assert_abort(!gb_lz(width));

    // set the cap
    impl->cap = gb_paint_stroke_cap(paint);

    // set the join
    impl->join = gb_paint_stroke_join(paint);

    // set the radius
    impl->radius = gb_rsh(width, 1);

    // set the miter limit
    impl->miter = gb_paint_stroke_miter(paint);
    tb_assert_abort(impl->miter > GB_ONE);
}
tb_void_t gb_stroker_clos(gb_stroker_ref_t stroker)
{
}
tb_void_t gb_stroker_move_to(gb_stroker_ref_t stroker, gb_point_ref_t point)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_inner && impl->path_outer && point);

    // done
    switch (impl->cap)
    {
    case GB_PAINT_STROKE_CAP_BUTT:
        {
        }
        break;
    case GB_PAINT_STROKE_CAP_ROUND:
        {
        }
        break;
    case GB_PAINT_STROKE_CAP_SQUARE:
        {
        }
        break;
    default:
        break;
    }
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
tb_void_t gb_stroker_add_arc(gb_stroker_ref_t stroker, gb_arc_ref_t arc)
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
tb_void_t gb_stroker_add_rect(gb_stroker_ref_t stroker, gb_rect_ref_t rect)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_other && rect);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // the width
    gb_float_t width = gb_lsh(radius, 1);

    // init the inner rect
    gb_rect_t rect_inner = *rect;
    if (rect_inner.w > width && rect_inner.h > width)
    {
        // make the inner rect
        gb_rect_deflate(&rect_inner, radius, radius);

        // add the inner rect to the other path
        gb_path_add_rect(impl->path_other, &rect_inner, GB_PATH_DIRECTION_CW);
    }

    // init the outer rect
    gb_rect_t rect_outer = *rect;

    // make the outer rect
    gb_rect_inflate(&rect_outer, radius, radius);

    // the join
    tb_size_t join = impl->join;

    /* add the outer rect to the other path
     *
     * ------------------------------ miter join
     *                    .        . |
     *                      .    L   |
     *            bevel join  .      |
     *                        | .    |  
     *                        |   .  |
     * -----------------|     |      |
     *                  |        R   |
     *                  |            |
     *                  |            |
     *                  |            |
     *                  |            |
     *                  | W = R * 2  |
     * 
     * W: width
     * R: radius
     * miter_limit = L / R > 1
     */
    switch (join)
    {
    case GB_PAINT_STROKE_JOIN_MITER:
        {
            // TODO limit miter
            // ...

            // add miter rect
            gb_path_add_rect(impl->path_other, &rect_outer, GB_PATH_DIRECTION_CCW);
        }
        break;
    case GB_PAINT_STROKE_JOIN_BEVEL:
        {
            // the bounds
            gb_float_t x = rect_outer.x;
            gb_float_t y = rect_outer.y;
            gb_float_t w = rect_outer.w;
            gb_float_t h = rect_outer.h;

            // add bevel rect by counter-clockwise
            gb_path_move2_to(impl->path_other, x,               y + radius);
            gb_path_line2_to(impl->path_other, x,               y + h - radius);
            gb_path_line2_to(impl->path_other, x + radius,      y + h);
            gb_path_line2_to(impl->path_other, x + w - radius,  y + h);
            gb_path_line2_to(impl->path_other, x + w,           y + h - radius);
            gb_path_line2_to(impl->path_other, x + w,           y + radius);
            gb_path_line2_to(impl->path_other, x + w - radius,  y);
            gb_path_line2_to(impl->path_other, x + radius,      y);
            gb_path_clos(impl->path_other);
        }
        break;
    case GB_PAINT_STROKE_JOIN_ROUND:
        {
            // add round rect
            gb_path_add_round_rect2(impl->path_other, &rect_outer, radius, radius, GB_PATH_DIRECTION_CCW);
        }
        break;
    default:
        tb_trace_e("unknown join: %lu", impl->join);
        break;
    }
}
tb_void_t gb_stroker_add_circle(gb_stroker_ref_t stroker, gb_circle_ref_t circle)
{
    // check
    tb_assert_and_check_return(circle);

    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_make(circle->c.x, circle->c.y, circle->r, circle->r);

    // add ellipse
    gb_stroker_add_ellipse(stroker, &ellipse);
}
tb_void_t gb_stroker_add_ellipse(gb_stroker_ref_t stroker, gb_ellipse_ref_t ellipse)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_other && ellipse);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // init the inner ellipse
    gb_ellipse_t ellipse_inner = *ellipse;
    if (ellipse_inner.rx > radius && ellipse_inner.ry > radius)
    {
        // make the inner ellipse
        ellipse_inner.rx -= radius;
        ellipse_inner.ry -= radius;

        // add the inner ellipse to the other path
        gb_path_add_ellipse(impl->path_other, &ellipse_inner, GB_PATH_DIRECTION_CW);
    }

    // init the outer ellipse
    gb_ellipse_t ellipse_outer = *ellipse;

    // make the outer ellipse
    ellipse_outer.rx += radius;
    ellipse_outer.ry += radius;

    // add the inner and outer ellipse to the other path
    gb_path_add_ellipse(impl->path_other, &ellipse_outer, GB_PATH_DIRECTION_CCW);
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
    tb_assert_and_check_return(impl && impl->path_other && points && count);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // make the stroked path
    switch (impl->cap)
    {
    case GB_PAINT_STROKE_CAP_ROUND:
        {
            // done
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_circle_t     circle;
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make circle
                circle = gb_circle_make(point->x, point->y, radius);

                // add circle to the other path
                gb_path_add_circle(impl->path_other, &circle, GB_PATH_DIRECTION_CW);
            }
        }
        break;
    case GB_PAINT_STROKE_CAP_BUTT:
    case GB_PAINT_STROKE_CAP_SQUARE:
    default:
        {
            // done
            gb_rect_t       rect;
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_float_t      width = gb_lsh(radius, 1);
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make rect
                rect = gb_rect_make(point->x - radius, point->y - radius, width, width);

                // add rect to the other path
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

    // attempt to add hint first
    if (!gb_stroker_add_hint(stroker, gb_path_hint(path)))
    {
        // add path to the stroker
        gb_stroker_add_path(stroker, path);
    }

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
gb_path_ref_t gb_stroker_done_polygon(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_polygon_ref_t polygon, gb_shape_ref_t hint)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // attempt to add hint first
    if (!gb_stroker_add_hint(stroker, hint))
    {
        // add polygon to the stroker
        gb_stroker_add_polygon(stroker, polygon);
    }

    // done the stroker
    return gb_stroker_done(stroker);
}
