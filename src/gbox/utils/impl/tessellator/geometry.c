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

    // upper <= center <= lower
    tb_assert_abort(gb_tessellator_vertex_le_h(upper, center));
    tb_assert_abort(gb_tessellator_vertex_le_h(center, lower));

    // compute the upper and lower XXX
    gb_float_t yu = center->y - upper->y;
    gb_float_t yl = lower->y - center->y;
    tb_assert_abort(!gb_lz(yu) && !gb_lz(yl));

    if (yu + yl > 0)
    {
        if (yu < yl)
        {
            /*
             *              upper             
             *                .                
             *               .  
             *              .
             *             . -------------------- . 
             *            .                     center
             *           .
             *          .          
             *         .                          
             *        .                     
             *       .                       
             *       lower                     
             *
             */
            return (center->x - upper->x) + (upper->x - lower->x) * (yu / (yu + yl));
        } 
        else 
        {
            /*
             *              upper             
             *                .                
             *               .  
             *              .
             *             .
             *            .
             *           .          
             *          . -------------------- . 
             *         .                     center     
             *        .                     
             *       .                       
             *       lower                     
             *
             */
            return (center->x - lower->x) + (lower->x - upper->x) * (yl / (yu + yl));
        }
    }

    return 0;
}
tb_long_t gb_tessellator_position_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower)
{
    return 0;
}
gb_float_t gb_tessellator_distance_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    return 0;
}
tb_long_t gb_tessellator_position_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right)
{
    return 0;
}



