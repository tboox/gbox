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

// calculate interpolation: (a * x + b * y) / (a + b) for intersection
static __tb_inline__ gb_float_t gb_segment_intersection_interpolate(gb_float_t a, gb_float_t x, gb_float_t b, gb_float_t y)
{
    // a and b may be slightly negative
    if (a < 0) a = 0;
    if (b < 0) b = 0;

    // the results are very accurate even when a and b are very large
    if (a >= b)
    {
        // calculate the factor
        gb_float_t factor = gb_div(b, a + b);

        /* calculate interpolation: x + (y - x) * (b / (a + b))
         *
         * we only return x + y / 2 if a and b are zero
         */
        return (0 == a)? gb_avg(x, y) : (x + gb_mul(y - x, factor));
    }
    else 
    {
        // calculate the factor
        gb_float_t factor = gb_div(a, a + b);

        // calculate interpolation: y + (x - y) * (a / (a + b))
        return (y + gb_mul(x - y, factor));
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
static tb_void_t gb_segment_intersection_x(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
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
    if (gb_point_in_right(org2, dst1))
    {
        // no intersection
        tb_assertf_abort(0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);
    }
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

        // no intersection? dy1 * dy2 < 0?
        tb_assertf_abort(dy1 > -GB_NEAR0 && dy2 > -GB_NEAR0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);

        /* calculate the x-coordinate of the intersection
         *
         * result.x = (org2.x * dy2 + dst1.x * dy1) / (dy1 + dy2)
         */
        result->x = gb_segment_intersection_interpolate(org2->x, dy2, dst1->x, dy1);
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
        /* calculate the vertical distance: dy1 and dy2
         *
         * TODO optimization
         */
        gb_float_t dy1 = gb_point_to_segment_distance_v(org2, org1, dst1);
        gb_float_t dy2 = -gb_point_to_segment_distance_v(dst2, org1, dst1);

        // ensure: (dy1 + dy2) > 0
        if (dy1 + dy2 < 0)
        {
            dy1 = -dy1;
            dy2 = -dy2;
        }

        // no intersection? dy1 * dy2 < 0?
        tb_assertf_abort(dy1 > -GB_NEAR0 && dy2 > -GB_NEAR0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);

        /* calculate the x-coordinate of the intersection
         *
         * result.x = (org2.x * dy2 + dst2.x * dy1) / (dy1 + dy2)
         */
        result->x = gb_segment_intersection_interpolate(org2->x, dy2, dst2->x, dy1);
    }
}
static tb_void_t gb_segment_intersection_y(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
{
    // check
    tb_assert_abort(org1 && dst1 && org2 && dst2);

    // sort edges in the order: org1.y <= org2.y <= (dst1/dst2).y
    if (gb_point_in_bottom(org1, dst1)) { tb_swap(gb_point_ref_t, org1, dst1); }
    if (gb_point_in_bottom(org2, dst2)) { tb_swap(gb_point_ref_t, org2, dst2); }
    if (gb_point_in_bottom(org1, org2)) { tb_swap(gb_point_ref_t, org1, org2); 
                                          tb_swap(gb_point_ref_t, dst1, dst2); }

    // no intersection?
    if (gb_point_in_bottom(org2, dst1))
    {
        // no intersection
        tb_assertf_abort(0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);
    }
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

        // no intersection? dx1 * dx2 < 0?
        tb_assertf_abort(dx1 > -GB_NEAR0 && dx2 > -GB_NEAR0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);

        /* calculate the y-coordinate of the intersection
         *
         * result.y = (org2.y * dx2 + dst1.y * dx1) / (dx1 + dx2)
         */
        result->y = gb_segment_intersection_interpolate(org2->y, dx2, dst1->y, dx1);
    }
    // two points are projected onto the same line segment
    else
    {
        // calculate the horizontal distance: dx1 and dx2
        gb_float_t dx1 = gb_point_to_segment_distance_h(org2, org1, dst1);
        gb_float_t dx2 = -gb_point_to_segment_distance_h(dst2, org1, dst1);

        // ensure: (dx1 + dx2) > 0
        if (dx1 + dx2 < 0)
        {
            dx1 = -dx1;
            dx2 = -dx2;
        }

        // no intersection? dx1 * dx2 < 0?
        tb_assertf_abort(dx1 > -GB_NEAR0 && dx2 > -GB_NEAR0, "no intersection for: (%{point}, %{point}) and (%{point}, %{point})", org1, dst1, org2, dst2);

        /* calculate the y-coordinate of the intersection
         *
         * result.y = (org2.y * dx2 + dst2.y * dx1) / (dx1 + dx2)
         */
        result->y = gb_segment_intersection_interpolate(org2->y, dx2, dst2->y, dx1);
    }
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
    tb_assert_abort(gb_point_in_top_or_horizontal(upper, center));
    tb_assert_abort(gb_point_in_top_or_horizontal(center, lower));

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
            gb_float_t factor = gb_div(yu, yu + yl);
            return (center->x - upper->x) + gb_mul(upper->x - lower->x, factor);
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
            gb_float_t factor = gb_div(yl, yu + yl);
            return (center->x - lower->x) + gb_mul(lower->x - upper->x, factor);
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
    tb_assert_abort(gb_point_in_left_or_vertical(left, center));
    tb_assert_abort(gb_point_in_left_or_vertical(center, right));

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
            gb_float_t factor = gb_div(xl, xl + xr);
            return (center->y - left->y) + gb_mul(left->y - right->y, factor);
        } 
        // center.x is close to right.x?
        else 
        {
            // compute distance
            gb_float_t factor = gb_div(xr, xl + xr);
            return (center->y - right->y) + gb_mul(right->y - left->y, factor);
        }
    }

    // vertical edge? no distance
    return 0;
}
tb_long_t gb_point_to_segment_position_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_point_in_top_or_horizontal(upper, center));
    tb_assert_abort(gb_point_in_top_or_horizontal(center, lower));

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
        return (tb_long_t)((tb_hong_t)(center->x - lower->x) * yu + (tb_hong_t)(center->x - upper->x) * yl);
#else
        return gb_sign_to_long((center->x - lower->x) * yu + (center->x - upper->x) * yl);
#endif
    }

    // horizontal edge
    return 0;
}
tb_long_t gb_point_to_segment_position_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assert_abort(gb_point_in_left_or_vertical(left, center));
    tb_assert_abort(gb_point_in_left_or_vertical(center, right));

    // compute the left and right x-distances
    gb_float_t xl = center->x - left->x;
    gb_float_t xr = right->x - center->x;
    tb_assert_abort(xl >= 0 && xr >= 0);

    // edge(left, right) is not vertical?
    if (xl + xr > 0)
    {
        // compute the position
#ifdef GB_CONFIG_FLOAT_FIXED
        tb_hong_t position = ((tb_hong_t)(center->y - right->y) * xl + (tb_hong_t)(center->y - left->y) * xr);
        return position < 0? -1 : position > 0;
#else
        return gb_sign_to_long((center->y - right->y) * xl + (center->y - left->y) * xr);
#endif
    }

    // vertical edge
    return 0;
}
tb_void_t gb_segment_intersection(gb_point_ref_t org1, gb_point_ref_t dst1, gb_point_ref_t org2, gb_point_ref_t dst2, gb_point_ref_t result)
{
    // check
    tb_assert_abort(org1 && dst1 && org2 && dst2 && result);

    /* calculate the intersection
     *
     * this is certainly not the most efficient way to find the intersection of two line segments,
     * but it is very numerically stable.
     */
    gb_segment_intersection_x(org1, dst1, org2, dst2, result);
    gb_segment_intersection_y(org1, dst1, org2, dst2, result);
}
