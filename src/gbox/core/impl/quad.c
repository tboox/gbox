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
 * @file        quad.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "quad.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_quad_make_line_impl(gb_point_t points[3], tb_size_t count, gb_quad_line_func_t func, tb_cpointer_t priv)
{
    /* divide it
     * 
     *
     *                  p1
     *                  .
     *                .  .
     *              .     .
     *            .        .
     *       o1 . . . . . . . o3
     *        .      o2      .
     *      .                 .
     *    .                    .
     * p0, o0                p2, o4
     *
     */
    if (count)
    {
        // chop the quad at half
        gb_point_t output[5];
        gb_quad_chop_at_half(points, output);

        // make line for quad(o0, o1, o2)
        gb_quad_make_line_impl(output, count - 1, func, priv);

        // make line to quad(o2, o3, o4)
        gb_quad_make_line_impl(output + 2, count - 1, func, priv);
    }
    else func(points + 2, priv);
}
static tb_void_t gb_quad_chop_x_or_y_at(gb_float_t const* xy, gb_float_t* output, gb_float_t factor) 
{
    // compute the interpolation of p0 => p1
    gb_float_t xy01 = gb_interp(xy[0], xy[2], factor);

    // compute the interpolation of p1 => p2
    gb_float_t xy12 = gb_interp(xy[2], xy[4], factor);

    // make output
    output[0] = xy[0];
    output[2] = xy01;
    output[4] = gb_interp(xy01, xy12, factor);
    output[6] = xy12;
    output[8] = xy[4];
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_quad_near_distance(gb_point_t points[3])
{
    // check
    tb_assert_abort(points);
 
    // compute the delat x and y of the distance(p1, center(p0, p2))
    gb_float_t dx = gb_avg(points[0].x, points[2].x) - points[1].x;
    gb_float_t dy = gb_avg(points[0].y, points[2].y) - points[1].y;
    dx = gb_fabs(dx);
    dy = gb_fabs(dy);

    // compute the more approximate distance
    return (dx > dy)? (dx + gb_half(dy)) : (dy + gb_half(dx));
}
tb_size_t gb_quad_divide_line_count(gb_point_t points[3])
{
    // check
    tb_assert_abort(points);

    // compute the approximate distance
    gb_float_t distance = gb_quad_near_distance(points);
    tb_assert_abort(!gb_lz(distance));

    // get the integer distance
    tb_size_t idistance = gb_ceil(distance);

    // compute the divided count
    tb_size_t count = (tb_ilog2i(idistance) >> 1) + 1;

    // limit the count
    if (count > GB_QUAD_DIVIDED_MAXN) count = GB_QUAD_DIVIDED_MAXN;

    // ok
    return count;
}
tb_void_t gb_quad_chop_at(gb_point_t points[3], gb_point_t output[5], gb_float_t factor)
{
    // check
    tb_assert_abort(points && output && gb_bz(factor) && factor < GB_ONE);

    // chop x-coordinates at the factor
    gb_quad_chop_x_or_y_at(&points[0].x, &output[0].x, factor);

    // chop y-coordinates at the factor
    gb_quad_chop_x_or_y_at(&points[0].y, &output[0].y, factor);
}
tb_void_t gb_quad_chop_at_half(gb_point_t points[3], gb_point_t output[5])
{
    // check
    tb_assert_abort(points && output);

    /* compute the chopped points
     *
     *                  p1
     *                  .
     *                .  .
     *              .     .
     *            .        .
     *       o1 . . . . . . . o3
     *        .      o2      .
     *      .                 .
     *    .                    .
     * p0, o0                p2, o4
     *
     * (p0, p1, p2) => (o0, o1, o2) + (o2, o3, o4)
     */
    gb_float_t x01 = gb_avg(points[0].x, points[1].x);
    gb_float_t y01 = gb_avg(points[0].y, points[1].y);
    gb_float_t x12 = gb_avg(points[1].x, points[2].x);
    gb_float_t y12 = gb_avg(points[1].y, points[2].y);

    // make output
    output[0] = points[0];
    gb_point_make(&output[1], x01, y01);
    gb_point_make(&output[2], gb_avg(x01, x12), gb_avg(y01, y12));
    gb_point_make(&output[3], x12, y12);
    output[4] = points[2];
}
tb_void_t gb_quad_make_line(gb_point_t points[3], gb_quad_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(func && points);

    // compute the divided count first
    tb_size_t count = gb_quad_divide_line_count(points);

    // make line
    gb_quad_make_line_impl(points, count, func, priv);
}

