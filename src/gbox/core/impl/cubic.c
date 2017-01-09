/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        cubic.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "cubic.h"
#include "float.h"

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
static tb_void_t gb_cubic_chop_xy_at(gb_float_t const* xy, gb_float_t* output, gb_float_t factor) 
{
    // compute the interpolation of p0 => p1
    gb_float_t xy01 = gb_interp(xy[0], xy[2], factor);

    // compute the interpolation of p1 => p2
    gb_float_t xy12 = gb_interp(xy[2], xy[4], factor);

    // compute the interpolation of p2 => p3
    gb_float_t xy23 = gb_interp(xy[4], xy[6], factor);

    // compute the interpolation of (p0, p1) => (p1, p2)
    gb_float_t xy012 = gb_interp(xy01, xy12, factor);
   
    // compute the interpolation of (p1, p2) => (p2, p3)
    gb_float_t xy123 = gb_interp(xy12, xy23, factor);

    // make output
    output[0]   = xy[0];
    output[2]   = xy01;
    output[4]   = xy012;
    output[6]   = gb_interp(xy012, xy12, factor);
    output[8]   = xy123;
    output[10]  = xy23;
    output[12]  = xy[6];
}
static tb_size_t gb_cubic_find_max_curvature(gb_point_t const points[4], gb_float_t factors[3])
{
    // TODO
    tb_trace_noimpl();
    return 0;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_cubic_near_distance(gb_point_t const points[4])
{
    // check
    tb_assert(points);

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

    dx1 = gb_abs(dx1);
    dy1 = gb_abs(dy1);
    dx2 = gb_abs(dx2);
    dy2 = gb_abs(dy2);

    // compute the more approximate distance
    gb_float_t d1 = (dx1 > dy1)? (dx1 + gb_half(dy1)) : (dy1 + gb_half(dx1));
    gb_float_t d2 = (dx2 > dy2)? (dx2 + gb_half(dy2)) : (dy2 + gb_half(dx2));

    // using the maximum value
    return tb_max(d1, d2);
}
tb_size_t gb_cubic_divide_line_count(gb_point_t const points[4])
{
    // check
    tb_assert(points);

    // compute the approximate distance
    gb_float_t distance = gb_cubic_near_distance(points);
    tb_assert(distance >= 0);

    // get the integer distance
    tb_size_t idistance = gb_ceil(distance);

    // compute the divided count
    tb_size_t count = (tb_ilog2i(idistance) >> 1) + 1;

    // limit the count
    if (count > GB_CUBIC_DIVIDED_MAXN) count = GB_CUBIC_DIVIDED_MAXN;

    // ok
    return count;
}
tb_void_t gb_cubic_chop_at(gb_point_t const points[4], gb_point_t output[7], gb_float_t factor)
{
    // check
    tb_assert(points && output && gb_float_in_unit_range(factor));

    // chop x-coordinates at the factor
    gb_cubic_chop_xy_at(&points[0].x, &output[0].x, factor);

    // chop y-coordinates at the factor
    gb_cubic_chop_xy_at(&points[0].y, &output[0].y, factor);
}
tb_void_t gb_cubic_chop_at2(gb_point_t const points[4], gb_point_ref_t output, tb_size_t maxn, gb_float_t const* factors, tb_size_t count)
{
    // check
    tb_assert(points && output && maxn >= 7 && factors && count);
    
    // done
    tb_size_t   index;
    gb_point_t  buffer[4];
    gb_float_t  factor = factors[0];
    for (index = 0; index < count; index++) 
    {
        // check
        tb_assert(maxn >= 7);

        // chop it
        gb_cubic_chop_at(points, output, factor);

        // end?
        tb_check_break(index != count - 1);

        // update to the next sub-cubic curve output
        output  += 3;
        maxn    -= 3;

        // update the new start points
        tb_memcpy(buffer, output, sizeof(buffer));
        points = buffer;

        // check
        tb_assert(gb_float_in_unit_range(factors[index]));
        tb_assert(gb_float_in_unit_range(factors[index + 1]));
        tb_assert(factors[index + 1] > factors[index]);

        // compute the next factor for the new range: (factor[index], 1) => (0, 1)
        if (!gb_float_unit_divide(factors[index + 1] - factors[index], GB_ONE - factors[index], &factor))
        {
            // failed? only make a degenerate cubic
            output[4] = output[5] = output[6] = points[3];
            break;
        }
    }
}
tb_void_t gb_cubic_chop_at_half(gb_point_t const points[4], gb_point_t output[7])
{
    // check
    tb_assert(points && output);

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
tb_size_t gb_cubic_chop_at_max_curvature(gb_point_t const points[4], gb_point_t output[13])
{
    // check
    tb_assert(points);

    // find the factors of the max curvature
    gb_float_t  factors[3];
    tb_size_t   factors_count = gb_cubic_find_max_curvature(points, factors);
    tb_assert(factors_count <= 3);

    // done
    if (output)
    {
        // chop it
        if (factors_count) gb_cubic_chop_at2(points, output, 13, factors, factors_count);
        // using the whole curve
        else tb_memcpy(output, points, sizeof(gb_point_t) << 2);
    }

    // the sub-curve count
    return factors_count + 1;
}
tb_void_t gb_cubic_make_line(gb_point_t const points[4], gb_cubic_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert(func && points);

    // compute the divided count first
    tb_size_t count = gb_cubic_divide_line_count(points);

    // make line
    gb_cubic_make_line_impl(points, count, func, priv);
}

