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
 * @file        geometry.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "geometry"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static gb_double_t gb_point_to_segment_distance_h_cheap(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assertf_abort(gb_point_in_top_or_horizontal(upper, center), "%{point} <=? %{point}", upper, center);
    tb_assertf_abort(gb_point_in_top_or_horizontal(center, lower), "%{point} <=? %{point}", center, lower);

    // compute the upper and lower y-distances
    gb_float_t yu = center->y - upper->y;
    gb_float_t yl = lower->y - center->y;
    tb_assert_abort(yu >= 0 && yl >= 0);

    // edge(upper, lower) is not horizontal?
    if (yu + yl > 0)
    {
        /* compute the position
         * 
         * distance = (center.x - upper.x) + (upper.x - lower.x) * (yu / (yu + yl))
         * => distance * (yu + yl)  = (center.x - upper.x) * (yu + yl) + (upper.x - lower.x) * yu
         *                          = (center.x - upper.x) * yu + (center.x - upper.x) * yl + (upper.x - lower.x) * yu
         *
         *
         *
         * distance = (center.x - lower.x) + (lower.x - upper.x) * (yl / (yu + yl))
         * => distance * (yu + yl)  = (center.x - lower.x) * (yu + yl) + (lower.x - upper.x) * yl
         *                          = (center.x - lower.x) * yl + (center.x - lower.x) * yu + (lower.x - upper.x) * yl
         *
         * so
         *
         * 2 * distance * (yu + yl) = (center.x - upper.x) * yu + (center.x - upper.x) * yl + (upper.x - lower.x) * yu
         *                          + (center.x - lower.x) * yl + (center.x - lower.x) * yu + (lower.x - upper.x) * yl
         *                          = 2 * (center.x - lower.x) * yu + 2 * (center.x - upper.x) * yl
         *
         * => distance * (yu + yl)  = (center.x - lower.x) * yu + (center.x - upper.x) * yl
         * => sign(distance * (yu + yl))
         * => sign(distance)
         * => position
         *
         */
#ifdef GB_CONFIG_FLOAT_FIXED
        return ((tb_hong_t)(center->x - lower->x) * yu + (tb_hong_t)(center->x - upper->x) * yl) >> 16;
#else
        return (tb_double_t)(center->x - lower->x) * yu + (tb_double_t)(center->x - upper->x) * yl;
#endif
    }

    // horizontal edge
    return 0;
}
static gb_double_t gb_point_to_segment_distance_v_cheap(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assertf_abort(gb_point_in_left_or_vertical(left, center), "%{point} <=? %{point}", left, center);
    tb_assertf_abort(gb_point_in_left_or_vertical(center, right), "%{point} <=? %{point}", center, right);

    // compute the left and right x-distances
    gb_float_t xl = center->x - left->x;
    gb_float_t xr = right->x - center->x;
    tb_assert_abort(xl >= 0 && xr >= 0);

    // edge(left, right) is not vertical?
    if (xl + xr > 0)
    {
        // compute the position
#ifdef GB_CONFIG_FLOAT_FIXED
        return ((tb_hong_t)(center->y - right->y) * xl + (tb_hong_t)(center->y - left->y) * xr) >> 16;
#else
        return (tb_double_t)(center->y - right->y) * xl + (tb_double_t)(center->y - left->y) * xr;
#endif
    }

    // vertical edge
    return 0;
}
// calculate interpolation: (x * a + y * b) / (a + b) for intersection
static __tb_inline__ gb_float_t gb_segment_intersection_interpolate(gb_float_t x, gb_double_t a, gb_float_t y, gb_double_t b)
{
    // a and b may be slightly negative
    if (a < 0) a = 0;
    if (b < 0) b = 0;

    // the results are very accurate even when a and b are very large
    if (a >= b)
    {
        // we only return x + y / 2 if a and b are zero
        if (0 == a) return gb_avg(x, y);

#ifdef GB_CONFIG_FLOAT_FIXED
        // calculate the factor
        gb_float_t factor = (gb_float_t)((b << 16) / (a + b));
        tb_assertf_abort(b == (b << 16) >> 16, "the factors are too large: %lld %lld", a, b);

        // calculate the intersection
        return x + gb_mul(y - x, factor);
#else
        // calculate the intersection
        return (gb_float_t)(x + (y - x) * (b / (a + b)));
#endif
    }
    else 
    {
#ifdef GB_CONFIG_FLOAT_FIXED
        // calculate the factor
        gb_float_t factor = (gb_float_t)((a << 16) / (a + b));
        tb_assertf_abort(a == (a << 16) >> 16, "the factors are too large: %lld %lld", a, b);

        // calculate the intersection
        return y + gb_mul(x - y, factor);
#else
        // calculate the intersection
        return (gb_float_t)(y + (x - y) * (a / (a + b)));
#endif
    }
}
/* calculate the x-coordinate of intersection
 * 
 *
 *        org2
 *          . 
 *          |   .
 *      dy1 |       .
 * . . . . . . . . . . * . . . . . . . . . dst1
 * org1          dx    |    .       | dy2 
 *                     |        .   |
 *                     |            .
 *                     |           dst2
 *                     |
 *                  result.x
 *
 * if dy1 + dy2 > 0:
 *    dx / (dst2.x - org2.x) = dy1 / (dy1 + dy2)
 *    dx = dy1 * (dst2.x - org2.x) / (dy1 + dy2)
 *
 * so:
 * result.x = org2.x + dx = org2.x + dy1 * (dst2.x - org2.x) / (dy1 + dy2) 
 *          = (org2.x * (dy1 + dy2) + dy1 * (dst2.x - org2.x)) / (dy1 + dy2)
 *          = (org2.x * dy1 + org2.x * dy2 + dy1 * dst2.x - dy1 * org2.x) / (dy1 + dy2)
 *          = (org2.x * dy2 + dst2.x * dy1) / (dy1 + dy2)
 */
static tb_bool_t gb_segment_intersection_x(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
{
    // check
    tb_assert_abort(org1 && dst1 && org2 && dst2);

    /* sort edges in the order: org1.x <= org2.x <= (dst1/dst2).x
     *
     *        org2                                                    dst2
     *         .                                                       .
     *           .                                                   .
     *             .                                               .
     * . . . . . . . . . . . . . . .          or       . . . . . . . . . . . . .
     * org1            .           dst1              org1      .              dst1
     *                   .                                   .
     *                     .                               org2
     *                     dst2
     */
    if (gb_point_in_right(org1, dst1)) { tb_swap(gb_point_ref_t, org1, dst1); }
    if (gb_point_in_right(org2, dst2)) { tb_swap(gb_point_ref_t, org2, dst2); }
    if (gb_point_in_right(org1, org2)) { tb_swap(gb_point_ref_t, org1, org2); 
                                         tb_swap(gb_point_ref_t, dst1, dst2); }

    // check
    tb_assert_abort(org1 != org2);
    tb_assert_abort(dst1 != dst2);

    /* no intersection?
     *                              org2
     *                                . 
     *                                  .
     *                                    .
     * . . . . . . . . . . . . . . .        .
     * org1                       dst1        . 
     *                                          .
     *                                            .
     *                                             dst2
     */
    if (gb_point_in_right(org2, dst1)) return tb_false;
    /* two points are projected onto the different line segment
     * 
     * no intersection:
     *
     *                          org2
     *                           . 
     *                           |  .
     *                       dy1 | |   .
     *                           | |dy2   .
     * . . . . . . . . . . . . . . .         .
     * org1                       dst1          . 
     *                                             .
     *                                                .
     *                                                dst2
     *
     * or 
     *
     *        org2
     *          . 
     *          |   .
     *      dy1 |       .
     * . . . . . . . . . . * . . . . dst1
     * org1          dx    |    .  | dy2 
     *                     |        .
     *                     |            .
     *                     |                .
     *                     |                    .
     *                  result.x               dst2
     */
    else if (gb_point_in_left_or_vertical(dst1, dst2))
    {
        // calculate the vertical distance: dy1 and dy2
        gb_float_t dy1 = gb_point_to_segment_distance_v(org2, org1, dst1);
        gb_float_t dy2 = gb_point_to_segment_distance_v(dst1, org2, dst2);

        // ensure: (dy1 + dy2) > 0
        if (dy1 + dy2 < 0)
        {
            dy1 = -dy1;
            dy2 = -dy2;
        }

        /* no intersection? dy1 * dy2 < 0?
         *
         * we also attempt to calculate it if one segment is very close to another segment
         */
        if (dy1 < -GB_NEAR0 || dy2 < -GB_NEAR0) return tb_false;

        /* calculate the x-coordinate of the intersection
         *
         * result.x = (org2.x * dy2 + dst1.x * dy1) / (dy1 + dy2)
         */
        if (result) result->x = gb_segment_intersection_interpolate(org2->x, dy2, dst1->x, dy1);
    }
    /* two points are projected onto the same line segment
     *
     * no intersection:
     * 
     *        org2
     *          . 
     *          |   .
     *          |       .
     *      dy1 |           .  dst2
     *          |               .
     *          |               | dy2
     * . . . . . . . . . . . . . . . . . . . . dst1
     * org1                    
     *                           
     * or
     *
     *        org2
     *          . 
     *          |   .
     *      dy1 |       .
     * . . . . . . . . . . * . . . . . . . . . dst1
     * org1          dx    |    .       | dy2 
     *                     |        .   |
     *                     |            .
     *                     |           dst2
     *                     |
     *                  result.x
     */
    else
    {
        /* quickly calculate the vertical cheap distance: dy1 and dy2 
         * and the result will be not changed
         *
         * dy1_cheap = dy1 * (dst1.x - org1.x) = dy1 * dt
         * dy2_cheap = dy2 * (dst1.x - org1.x) = dy2 * dt
         *
         * result.x = (org2.x * dy2_cheap + dst2.x * dy1_cheap) / (dy1_cheap + dy2_cheap)
         *          = (org2.x * dy2 * dt + dst2.x * dy1 * dt) / (dy1 * dt + dy2 * dt)
         *          = (org2.x * dy2 + dst2.x * dy1) / (dy1 + dy2)
         */
        gb_double_t dy1 = gb_point_to_segment_distance_v_cheap(org2, org1, dst1);
        gb_double_t dy2 = -gb_point_to_segment_distance_v_cheap(dst2, org1, dst1);

        // ensure: (dy1 + dy2) > 0
        if (dy1 + dy2 < 0)
        {
            dy1 = -dy1;
            dy2 = -dy2;
        }

        /* no intersection? dy1 * dy2 < 0?
         *
         * we also attempt to calculate it if one segment is very close to another segment
         */
        if (dy1 < -GB_NEAR0 || dy2 < -GB_NEAR0) return tb_false;

        /* calculate the x-coordinate of the intersection
         *
         * result.x = (org2.x * dy2 + dst2.x * dy1) / (dy1 + dy2)
         */
        if (result) result->x = gb_segment_intersection_interpolate(org2->x, dy2, dst2->x, dy1);
    }

    // exists intersection
    return tb_true;
}
static tb_long_t gb_segment_intersection_y(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
{
    // check
    tb_assert_abort(org1 && dst1 && org2 && dst2);

    // sort edges in the order: org1.y <= org2.y <= (dst1/dst2).y
    if (gb_point_in_bottom(org1, dst1)) { tb_swap(gb_point_ref_t, org1, dst1); }
    if (gb_point_in_bottom(org2, dst2)) { tb_swap(gb_point_ref_t, org2, dst2); }
    if (gb_point_in_bottom(org1, org2)) { tb_swap(gb_point_ref_t, org1, org2); 
                                          tb_swap(gb_point_ref_t, dst1, dst2); }

    // check
    tb_assert_abort(org1 != org2);
    tb_assert_abort(dst1 != dst2);

    // no intersection?
    if (gb_point_in_bottom(org2, dst1)) return tb_false;
    // two points are projected onto the different line segment
    else if (gb_point_in_top_or_horizontal(dst1, dst2))
    {
        // calculate the horizontal distance: dx1 and dx2
        gb_float_t dx1 = gb_point_to_segment_distance_h(org2, org1, dst1);
        gb_float_t dx2 = gb_point_to_segment_distance_h(dst1, org2, dst2);

        // ensure: (dx1 + dx2) > 0
        if (dx1 + dx2 < 0)
        {
            dx1 = -dx1;
            dx2 = -dx2;
        }

        /* no intersection? dx1 * dx2 < 0?
         *
         * we also attempt to calculate it if one segment is very close to another segment
         */
        if (dx1 < -GB_NEAR0 || dx2 < -GB_NEAR0) return tb_false;

        /* calculate the y-coordinate of the intersection
         *
         * result.y = (org2.y * dx2 + dst1.y * dx1) / (dx1 + dx2)
         */
        if (result) result->y = gb_segment_intersection_interpolate(org2->y, dx2, dst1->y, dx1);
    }
    // two points are projected onto the same line segment
    else
    {
        // quickly calculate the horizontal cheap distance: dx1 and dx2
        gb_double_t dx1 = gb_point_to_segment_distance_h_cheap(org2, org1, dst1);
        gb_double_t dx2 = -gb_point_to_segment_distance_h_cheap(dst2, org1, dst1);

        // ensure: (dx1 + dx2) > 0
        if (dx1 + dx2 < 0)
        {
            dx1 = -dx1;
            dx2 = -dx2;
        }

        /* no intersection? dx1 * dx2 < 0?
         *
         * we also attempt to calculate it if one segment is very close to another segment
         */
        if (dx1 < -GB_NEAR0 || dx2 < -GB_NEAR0) return tb_false;

        /* calculate the y-coordinate of the intersection
         *
         * result.y = (org2.y * dx2 + dst2.y * dx1) / (dx1 + dx2)
         */
        if (result) result->y = gb_segment_intersection_interpolate(org2->y, dx2, dst2->y, dx1);
    }

    // exists intersection
    return tb_true;
}
static tb_bool_t gb_segment_near_parallel(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2)
{
    // the slope factors
    gb_float_t dy1 = org1->y - dst1->y;
    gb_float_t dx1 = org1->x - dst1->x;
    gb_float_t dy2 = org2->y - dst2->y;
    gb_float_t dx2 = org2->x - dst2->x;

    // compute the slope errors
#ifdef GB_CONFIG_FLOAT_FIXED
    gb_float_t dk = (gb_float_t)(((tb_hong_t)dy1 * dx2 - (tb_hong_t)dy2 * dx1) >> 16);
#else
    gb_float_t dk = (gb_float_t)((tb_double_t)dy1 * dx2 - (tb_double_t)dy2 * dx1);
#endif

    // is parallel?
    return gb_near0(dk);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_long_t gb_points_is_ccw(gb_point_ref_t p0, gb_point_ref_t p1, gb_point_ref_t p2)
{
    // check
    tb_assert_abort(p0 && p1 && p2);

    // the coordinates
    tb_fixed_t x0 = gb_float_to_fixed(p0->x);
    tb_fixed_t y0 = gb_float_to_fixed(p0->y);
    tb_fixed_t x1 = gb_float_to_fixed(p1->x);
    tb_fixed_t y1 = gb_float_to_fixed(p1->y);
    tb_fixed_t x2 = gb_float_to_fixed(p2->x);
    tb_fixed_t y2 = gb_float_to_fixed(p2->y);
  
    /* compute the cross value of the vectors (p1, p0) and (p1, p2)
     *
     * cross[(p1, p0), (p1, p2)] > 0
     *
     *
     * the result is not reliable for almost-degenerate situations
     */
    return ((tb_hong_t)(x0 - x1) * (y2 - y1) - (tb_hong_t)(y0 - y1) * (x2 - x1)) > 0;
}
gb_float_t gb_point_to_segment_distance_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assertf_abort(gb_point_in_top_or_horizontal(upper, center), "%{point} <=? %{point}", upper, center);
    tb_assertf_abort(gb_point_in_top_or_horizontal(center, lower), "%{point} <=? %{point}", center, lower);

    // compute the upper and lower y-distances
    gb_float_t yu = center->y - upper->y;
    gb_float_t yl = lower->y - center->y;
    tb_assert_abort(yu >= 0 && yl >= 0);

    // edge(upper, lower) is not horizontal?
    if (yu + yl > 0)
    {
        // center.y is close to upper.y?
        if (yu < yl)
        {
            /*
             *              upper             
             *       .        .                
             *    yu |       .  
             *       |      .        xc > 0
             *       .     . -|------------------ . center
             *       |    . xu > 0                  
             *       |   .
             *    yl |  .          
             *       | .                          
             *       |.                     
             *       .                       
             *       lower                     
             *
             *
             *                                       upper             
             *                                .        .                
             *                             yu |       .  
             *                                |      .   
             * center . ----------------------|---- . --
             *                                |    .  xu > 0               
             *                                |   .
             *                             yl |  .          
             *                                | .                          
             *                                |.                     
             *                                .                       
             *                                lower                   
             *
             *        |---------------------------------|
             *                       xc < 0
             *
             *
             * near horizontal? xu == 0
             *                               . upper                      . center
             *                 .             |----------------------------|
             *   .                                          xc
             * lower
             *
             * xc = center.x - upper.x
             * xu = (upper.x - lower.x) * (yu / (yu + yl))
             *
             * distance = xc + xu
             *          = (center.x - upper.x) + (upper.x - lower.x) * (yu / (yu + yl))
             */
#ifdef GB_CONFIG_FLOAT_FIXED
            return (center->x - upper->x) + (gb_float_t)(((tb_hong_t)yu * (upper->x - lower->x)) / (yu + yl));
#else
            tb_double_t factor = (tb_double_t)(upper->x - lower->x) / (yu + yl);
            return (center->x - upper->x) + (gb_float_t)(yu * factor);
#endif
        } 
        // center.y is close to lower.y?
        else 
        {
            /*
             *              upper             
             *       .        .                
             *       |       .  
             *       |      .
             *    yu |     .
             *       |    .
             *       |< 0.          
             *       .xl. -------------------- . 
             *       | .                     center     
             *    yl |.                     
             *       .                       
             *       lower                     
             *
             *       |-------------------------|
             *                 xc > 0
             *
             *
             * 
             *                                   upper             
             *                            .        .                
             *                            |       .  
             *                            |      .
             *                         yu |     .
             *                            |    .
             *               xc < 0       |<0 .          
             * center .------------------ .xl. 
             *                            | .    
             *                         yl |.                     
             *                            .                       
             *                            lower       
             *
             * 
             *
             * near horizontal? xl == 0
             *                               . upper                     
             *                 .              
             *   .                               .     
             * lower                           center
             *   |-------------------------------|
             *                  xc
             *
             * xc = center.x - lower.x
             * xl = (lower.x - upper.x) * (yl / (yu + yl)) 
             *
             * distance = xc + xl
             *          = (center.x - lower.x) + (lower.x - upper.x) * (yl / (yu + yl))
             */
#ifdef GB_CONFIG_FLOAT_FIXED
            return (center->x - lower->x) + (gb_float_t)((tb_hong_t)yl * (lower->x - upper->x) / (yu + yl));
#else
            tb_double_t factor = (tb_double_t)(lower->x - upper->x) / (yu + yl);
            return (center->x - lower->x) + (gb_float_t)(yl * factor);
#endif
        }
    }

    // horizontal edge? no distance
    return 0;
}
gb_float_t gb_point_to_segment_distance_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assertf_abort(gb_point_in_left_or_vertical(left, center), "%{point} <=? %{point}", left, center);
    tb_assertf_abort(gb_point_in_left_or_vertical(center, right), "%{point} <=? %{point}", center, right);

    // compute the left and right x-distances
    gb_float_t xl = center->x - left->x;
    gb_float_t xr = right->x - center->x;
    tb_assert_abort(xl >= 0 && xr >= 0);

    // edge(left, right) is not vertical?
    if (xl + xr > 0)
    {
        // center.x is close to left.x?
        if (xl < xr)
        {
            // compute distance
#ifdef GB_CONFIG_FLOAT_FIXED
            return (center->y - left->y) + (gb_float_t)(((tb_hong_t)xl * (left->y - right->y)) / (xl + xr));
#else
            tb_double_t factor = (tb_double_t)(left->y - right->y) / (xl + xr);
            return (center->y - left->y) + (gb_float_t)(xl * factor);
#endif
        } 
        // center.x is close to right.x?
        else 
        {
            // compute distance
#ifdef GB_CONFIG_FLOAT_FIXED
            return (center->y - right->y) + (gb_float_t)(((tb_hong_t)xr * (right->y - left->y)) / (xl + xr));
#else
            tb_double_t factor = (tb_double_t)(right->y - left->y) / (xl + xr);
            return (center->y - right->y) + (gb_float_t)(xr * factor);
#endif
        }
    }

    // vertical edge? no distance
    return 0;
}
tb_long_t gb_point_to_segment_position_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // compute the cheap distance quickly
    gb_double_t distance = gb_point_to_segment_distance_h_cheap(center, upper, lower);

    // get the sign of the distance
    return distance < 0? -1 : distance > 0;
}
tb_long_t gb_point_to_segment_position_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // compute the cheap distance quickly
    gb_double_t distance = gb_point_to_segment_distance_v_cheap(center, left, right);

    // get the sign of the distance
    return distance < 0? -1 : distance > 0;
}
tb_long_t gb_segment_intersection(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
{
    // check
    tb_assert_abort(org1 && dst1 && org2 && dst2);

    // near parallel? no intersection
    if (gb_segment_near_parallel(org1, dst1, org2, dst2)) return 0;

    /* calculate the intersection
     *
     * this is certainly not the most efficient way to find the intersection of two line segments,
     * but it is very numerically stable.
     */
    return (    gb_segment_intersection_x(org1, dst1, org2, dst2, result)
            &&  gb_segment_intersection_y(org1, dst1, org2, dst2, result))? 1 : -1;
}
