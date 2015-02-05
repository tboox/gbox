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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        geometry.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_float_t gb_tessellator_distance_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_tessellator_vertex_le_h(upper, center));
    tb_assert_abort(gb_tessellator_vertex_le_h(center, lower));

    // compute the upper and lower y-distances
    gb_float_t yu = center->y - upper->y;
    gb_float_t yl = lower->y - center->y;
    tb_assert_abort(!gb_lz(yu) && !gb_lz(yl));

    // edge(upper, lower) is not horizontal?
    if (gb_bz(yu + yl))
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
gb_float_t gb_tessellator_distance_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assert_abort(gb_tessellator_vertex_le_v(left, center));
    tb_assert_abort(gb_tessellator_vertex_le_v(center, right));

    // compute the left and right x-distances
    gb_float_t xl = center->x - left->x;
    gb_float_t xr = right->x - center->x;
    tb_assert_abort(!gb_lz(xl) && !gb_lz(xr));

    // edge(left, right) is not vertical?
    if (gb_bz(xl + xr))
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
tb_long_t gb_tessellator_position_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_tessellator_vertex_le_h(upper, center));
    tb_assert_abort(gb_tessellator_vertex_le_h(center, lower));

    // compute the upper and lower y-distances
    gb_float_t yu = center->y - upper->y;
    gb_float_t yl = lower->y - center->y;
    tb_assert_abort(!gb_lz(yu) && !gb_lz(yl));

    // edge(upper, lower) is not horizontal?
    if (gb_bz(yu + yl))
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
        return gb_sign(gb_mul(center->x - lower->x, yu) + gb_mul(center->x - upper->x, yl));
    }

    // horizontal edge
    return 0;
}
tb_long_t gb_tessellator_position_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assert_abort(gb_tessellator_vertex_le_v(left, center));
    tb_assert_abort(gb_tessellator_vertex_le_v(center, right));

    // compute the left and right x-distances
    gb_float_t xl = center->x - left->x;
    gb_float_t xr = right->x - center->x;
    tb_assert_abort(!gb_lz(xl) && !gb_lz(xr));

    // edge(left, right) is not vertical?
    if (gb_bz(xl + xr))
    {
        // compute the position
        return gb_sign(gb_mul(center->y - right->y, xl) + gb_mul(center->y - left->y, xr));
    }

    // vertical edge
    return 0;
}



