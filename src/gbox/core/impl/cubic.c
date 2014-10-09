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
 * @file        cubic.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "cubic.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_cubic_make_line_impl(gb_point_t const points[4], tb_size_t count, gb_cubic_line_func_t func, tb_cpointer_t priv)
{
    /* divide it
     * 
     *          p1                 p2
     *          . . . . . . . . . . .
     *         .                     .
     *        .    o2 . . . . . o4    .
     *       .           o3            .
     *   o1 .             |             . o5
     *     .            chop             .
     *    .                               .
     *   .                                 .
     *  .                                   .
     * .                                     .
     * p0, o0                                p3, o6
     *
     */
    if (count)
    {
        // chop the cubic at half
        gb_point_t output[7];
        gb_cubic_chop_at_half(points, output);

        // make line for cubic(o0, o1, o2, o3)
        gb_cubic_make_line_impl(output, count - 1, func, priv);

        // make line to cubic(o3, o4, o5, o6)
        gb_cubic_make_line_impl(output + 3, count - 1, func, priv);
    }
    else func((gb_point_ref_t)&points[3], priv);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_cubic_near_distance(gb_point_t const points[4])
{
    // check
    tb_assert_abort(points);

    /* compute the delat x and y 
     *
     *      p1                 p2
     *      . . . . . . . . . . .
     *     . .                 . .
     *    .   .               .   .
     *   .      .           .      .
     *  .     d1  .       .  d2     .
     * .            .   .            .
     * p0             .              p3
     *
     * distance = max(d1 + d2)
     */
    gb_float_t x03 = gb_avg(points[0].x, points[3].x);
    gb_float_t y03 = gb_avg(points[0].y, points[3].y);
    gb_float_t dx1 = x03 - points[1].x;
    gb_float_t dy1 = y03 - points[1].y;
    gb_float_t dx2 = x03 - points[2].x;
    gb_float_t dy2 = y03 - points[2].y;

    dx1 = gb_fabs(dx1);
    dy1 = gb_fabs(dy1);
    dx2 = gb_fabs(dx2);
    dy2 = gb_fabs(dy2);

    // compute the more approximate distance
    gb_float_t d1 = (dx1 > dy1)? (dx1 + gb_half(dy1)) : (dy1 + gb_half(dx1));
    gb_float_t d2 = (dx2 > dy2)? (dx2 + gb_half(dy2)) : (dy2 + gb_half(dx2));

    // using the maximum value
    return tb_max(d1, d2);
}
tb_size_t gb_cubic_divide_line_count(gb_point_t const points[4])
{
    // check
    tb_assert_abort(points);

    // compute the approximate distance
    gb_float_t distance = gb_cubic_near_distance(points);
    tb_assert_abort(!gb_lz(distance));

    // get the integer distance
    tb_size_t idistance = gb_ceil(distance);

    // compute the divided count
    tb_size_t count = (tb_ilog2i(idistance) >> 1) + 1;

    // limit the count
    if (count > GB_CUBIC_DIVIDED_MAXN) count = GB_CUBIC_DIVIDED_MAXN;

    // ok
    return count;
}
tb_void_t gb_cubic_chop_at_half(gb_point_t const points[4], gb_point_t output[7])
{
    // check
    tb_assert_abort(points && output);

    /* compute the chopped points
     *
     *          p1                 p2
     *          . . . . . . . . . . .
     *         .                     .
     *        .    o2 . . . . . o4    .
     *       .           o3            .
     *   o1 .             |             . o5
     *     .            chop             .
     *    .                               .
     *   .                                 .
     *  .                                   .
     * .                                     .
     * p0, o0                                p3, o6
     *
     * (p0, p1, p2, p3) => (o0, o1, o2, o3) + (o3, o4, o5, o6)
     */
    gb_float_t x01 = gb_avg(points[0].x, points[1].x);
    gb_float_t y01 = gb_avg(points[0].y, points[1].y);
    gb_float_t x12 = gb_avg(points[1].x, points[2].x);
    gb_float_t y12 = gb_avg(points[1].y, points[2].y);
    gb_float_t x23 = gb_avg(points[2].x, points[3].x);
    gb_float_t y23 = gb_avg(points[2].y, points[3].y);

    gb_float_t x012 = gb_avg(x01, x12);
    gb_float_t y012 = gb_avg(y01, y12);
    gb_float_t x123 = gb_avg(x12, x23);
    gb_float_t y123 = gb_avg(y12, y23);

    // make output
    output[0] = points[0];
    gb_point_make(&output[1], x01, y01);
    gb_point_make(&output[2], x012, y012);
    gb_point_make(&output[3], gb_avg(x012, x123), gb_avg(y012, y123));
    gb_point_make(&output[4], x123, y123);
    gb_point_make(&output[5], x23, y23);
    output[6] = points[3];
}
tb_void_t gb_cubic_make_line(gb_point_t const points[4], gb_cubic_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(func && points);

    // compute the divided count first
    tb_size_t count = gb_cubic_divide_line_count(points);

    // make line
    gb_cubic_make_line_impl(points, count, func, priv);
}

