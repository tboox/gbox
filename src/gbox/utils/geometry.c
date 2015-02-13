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
     */
    return ((tb_hong_t)(x0 - x1) * (y2 - y1) - (tb_hong_t)(y0 - y1) * (x2 - x1)) > 0;
}
gb_float_t gb_point_to_segment_distance_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_point_in_top_or_eq(upper, center));
    tb_assert_abort(gb_point_in_top_or_eq(center, lower));

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
    tb_assert_abort(gb_point_in_left_or_eq(left, center));
    tb_assert_abort(gb_point_in_left_or_eq(center, right));

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
    tb_assert_abort(gb_point_in_top_or_eq(upper, center));
    tb_assert_abort(gb_point_in_top_or_eq(center, lower));

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
    tb_assert_abort(gb_point_in_left_or_eq(left, center));
    tb_assert_abort(gb_point_in_left_or_eq(center, right));

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

