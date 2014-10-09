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
static tb_size_t gb_quad_unit_divide(gb_float_t numer, gb_float_t denom, gb_float_t* result)
{
    // check
    tb_assert_abort(result);

    // negate it
    if (gb_lz(numer)) 
    {
        numer = -numer;
        denom = -denom;
    }

    // must be valid numerator and denominator
    if (gb_ez(denom) || gb_ez(numer) || numer >= denom) 
        return 0;

    // the result: numer / denom
    gb_float_t r = gb_div(numer, denom);

    // must be finite value
    tb_assert_and_check_return_val(gb_isfinite(r), 0);

    // must be in range: [0, 1)
    tb_assert_and_check_return_val(!gb_lz(r) && r < GB_ONE, 0);

    // too smaller? not save result
    tb_check_return_val(gb_nz(r), 0);

    // save result
    *result = r;

    // ok
    return 1;
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
    gb_quad_unit_divide(-(gb_mul(ax, bx) + gb_mul(ay, by)), gb_sqre(bx) + gb_sqre(by), &factor);
    
    // ok
    return factor;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_quad_near_distance(gb_point_t const points[3])
{
    // check
    tb_assert_abort(points);
 
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
    dx = gb_fabs(dx);
    dy = gb_fabs(dy);

    // compute the more approximate distance
    return (dx > dy)? (dx + gb_half(dy)) : (dy + gb_half(dx));
}
tb_size_t gb_quad_divide_line_count(gb_point_t const points[3])
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
tb_void_t gb_quad_chop_at(gb_point_t const points[3], gb_point_t output[5], gb_float_t factor)
{
    // check
    tb_assert_abort(points && output && gb_bz(factor) && factor < GB_ONE);

    // chop x-coordinates at the factor
    gb_quad_chop_xy_at(&points[0].x, &output[0].x, factor);

    // chop y-coordinates at the factor
    gb_quad_chop_xy_at(&points[0].y, &output[0].y, factor);
}
tb_void_t gb_quad_chop_at_half(gb_point_t const points[3], gb_point_t output[5])
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
tb_size_t gb_quad_chop_at_max_curvature(gb_point_t const points[3], gb_point_t output[5])
{
    // check
    tb_assert_abort(points && output);

    // find the factor of the max curvature
    gb_float_t factor = gb_quad_find_max_curvature(points);

    // chop it
    tb_size_t count = 2;
    if (gb_nz(factor)) gb_quad_chop_at(points, output, factor);
    else
    {
        tb_memcpy(output, points, 3 * sizeof(gb_point_t));
        count = 1;
    }

    // the chopped count
    return count;
}
tb_void_t gb_quad_make_line(gb_point_t const points[3], gb_quad_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(func && points);

    // compute the divided count first
    tb_size_t count = gb_quad_divide_line_count(points);

    // make line
    gb_quad_make_line_impl(points, count, func, priv);
}

