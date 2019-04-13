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
 * @file        quad.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "quad.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_quad_make_line_impl(gb_point_t const points[3], tb_size_t count, gb_quad_line_func_t func, tb_cpointer_t priv)
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
    else func((gb_point_ref_t)&points[2], priv);
}
static tb_void_t gb_quad_chop_xy_at(gb_float_t const* xy, gb_float_t* output, gb_float_t factor) 
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
static gb_float_t gb_quad_find_max_curvature(gb_point_t const points[3])
{
    /* parameter: f = factor
     *
     * function of the x-coordinate:
     * X(f) = x0 * (1 - f) ^ 2 + 2 * x1 * f * (1 - f) + x2 * f ^ 2
     * => X'(f) = 2 * (x1 - x0) + 2 * (x0 - 2 * x1 + x2) * f
     * => X''(f) = 2 * (x0 - 2 * x1 + x2)
     *
     * Ax = 2 * (x1 - x0)
     * Bx = 2 * (x0 - 2 * x1 + x2)
     *
     * => X'(f) = Ax + Bx * f
     * => X''(f) = Bx
     *
     * function of the y-coordinate:
     * Y(f) = y0 * (1 - f) ^ 2 + 2 * y1 * f * (1 - f) + y2 * f ^ 2
     * => Y'(f) = 2 * (y1 - y0) + 2 * (y0 - 2 * y1 + y2) * f
     * => Y''(f) = 2 * (y0 - 2 * y1 + y2)
     *
     * Ay = 2 * (y1 - y0)
     * By = 2 * (y0 - 2 * y1 + y2)
     *
     * => Y'(f) = Ay + By * f
     * => Y''(f) = By
     *
     * curvature:
     *          |X'(f)Y''(f) - X''(f)Y'(f)|
     * K(f) = -------------------------------
     *           (X'(f)^2 + Y'(f)^2)^(3/2)
     *
     *          |(Ax + Bx * f) * By - Bx * (Ay + By * f)|
     *      = ---------------------------------------------
     *          ((Ax + Bx * f)^2 + (Ay + By * f)^2)^(3/2)
     *
     *          |Ax * By + Bx * By * f - Bx * Ay - Bx * By * f|
     *      = ------------------------------------------------------------------------------------
     *          (Ax^2 + 2 * Ax * Bx * f + Bx^2 * f^2 + Ay^2 + 2 * Ay * By * f + By^2 * f^2)^(3/2)
     *
     *          |Ax * By - Bx * Ay|
     *      = ------------------------------------------------------------------------------------
     *          (Ax^2 + Ay^2 + 2 * f * (Ax * Bx + Ay * By) + (Bx^2 + By^2) * f^2)^(3/2)
     *
     *          |Ax * By - Bx * Ay|
     *      = -----------------------
     *            (F(f))^(3/2)
     *
     * F(f) = Ax^2 + Ay^2 + 2 * f * (Ax * Bx + Ay * By) + (Bx^2 + By^2) * f^2
     * => F'(f) = 2 * (Ax * Bx + Ay * By) + 2 * f * (Bx^2 + By^2)
     *
     * max(K(f)) => min(F(f)) => F'(f) == 0
     * => 2 * (Ax * Bx + Ay * By) + 2 * f * (Bx^2 + By^2) = 0
     * => f = -(Ax * Bx + Ay * By) / (Bx^2 + By^2)
     *
     * so
     *
     * the curvature is maximum if f == -(Ax * Bx + Ay * By) / (Bx^2 + By^2)
     */
    gb_float_t ax = points[1].x - points[0].x;
    gb_float_t ay = points[1].y - points[0].y;
    gb_float_t bx = points[0].x - points[1].x - points[1].x + points[2].x;
    gb_float_t by = points[0].y - points[1].y - points[1].y + points[2].y;

    /* compute the factor of the max curvature
     *
     * -(Ax * Bx + Ay * By) / (Bx^2 + By^2)
     */
    gb_float_t factor = 0;
#ifdef GB_CONFIG_FLOAT_FIXED
    tb_hong_t numer = -((tb_hong_t)ax * bx + (tb_hong_t)ay * by);
    tb_hong_t denom = ((tb_hong_t)bx * bx + (tb_hong_t)by * by);
    if (numer < 0)
    {
        numer = -numer;
        denom = -denom;
    }
    if (denom && numer < denom) factor = (gb_float_t)((numer / denom) << 16);
#else
    gb_float_unit_divide(-(ax * bx + ay * by), bx * bx + by * by, &factor);
#endif
   
    // invalid factor?
    if (factor < 0 || factor >= GB_ONE)
    {
        // clear it
        factor = 0;

        // failed
        tb_assert(0);
    }
 
    // ok
    return factor;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_quad_near_distance(gb_point_t const points[3])
{
    // check
    tb_assert(points);
 
    /* compute the delat x and y of the distance(p1, center(p0, p2))
     *
     *                  p1
     *                  .
     *                .  .
     *              .  .  .
     *            .   .    .
     *          .    .      .
     *        .     .        .
     *      .      .distance  .
     *    .       .            .
     * p0         x            p2
     *
     */
    gb_float_t dx = gb_avg(points[0].x, points[2].x) - points[1].x;
    gb_float_t dy = gb_avg(points[0].y, points[2].y) - points[1].y;
    dx = gb_abs(dx);
    dy = gb_abs(dy);

    // compute the more approximate distance
    return (dx > dy)? (dx + gb_half(dy)) : (dy + gb_half(dx));
}
tb_size_t gb_quad_divide_line_count(gb_point_t const points[3])
{
    // check
    tb_assert(points);

    // compute the approximate distance
    gb_float_t distance = gb_quad_near_distance(points);
    tb_assert(distance >= 0);

    // get the integer distance
    tb_size_t idistance = gb_ceil(distance);

    // compute the divided count
    tb_size_t count = (tb_ilog2i(idistance) >> 1) + 1;

    // limit the count
    if (count > GB_QUAD_DIVIDED_MAXN) count = GB_QUAD_DIVIDED_MAXN;

    // ok
    return count;
}
tb_void_t gb_quad_chop_at(gb_point_t const points[3], gb_point_t output[5], gb_float_t factor)
{
    // check
    tb_assert(points && output && gb_float_in_unit_range(factor));

    // chop x-coordinates at the factor
    gb_quad_chop_xy_at(&points[0].x, &output[0].x, factor);

    // chop y-coordinates at the factor
    gb_quad_chop_xy_at(&points[0].y, &output[0].y, factor);
}
tb_void_t gb_quad_chop_at_half(gb_point_t const points[3], gb_point_t output[5])
{
    // check
    tb_assert(points && output);

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
tb_size_t gb_quad_chop_at_max_curvature(gb_point_t const points[3], gb_point_t output[5])
{
    // check
    tb_assert(points);

    // find the factor of the max curvature
    gb_float_t factor = gb_quad_find_max_curvature(points);

    // done
    tb_size_t count = 2;
    if (output)
    {
        // chop it
        if (factor != 0) gb_quad_chop_at(points, output, factor);
        // using the whole curve
        else
        {
            tb_memcpy(output, points, 3 * sizeof(gb_point_t));
            count = 1;
        }
    }
    else if (0 == factor) count = 1;

    // the sub-curve count
    return count;
}
tb_void_t gb_quad_make_line(gb_point_t const points[3], gb_quad_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert(func && points);

    // compute the divided count first
    tb_size_t count = gb_quad_divide_line_count(points);

    // make line
    gb_quad_make_line_impl(points, count, func, priv);
}

