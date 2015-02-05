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
gb_float_t gb_tessellator_distance_h(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t upper, gb_mesh_vertex_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_tessellator_vertex_le_h(upper, center));
    tb_assert_abort(gb_tessellator_vertex_le_h(center, lower));

    // the points
    gb_point_ref_t pc = gb_tessellator_vertex_point(center);
    gb_point_ref_t pu = gb_tessellator_vertex_point(upper);
    gb_point_ref_t pl = gb_tessellator_vertex_point(lower);
    tb_assert_abort(pc && pu && pl);

    // compute the upper and lower y-distances
    gb_float_t yu = pc->y - pu->y;
    gb_float_t yl = pl->y - pc->y;
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
            return (pc->x - pu->x) + gb_mul(pu->x - pl->x, factor);
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
            return (pc->x - pl->x) + gb_mul(pl->x - pu->x, factor);
        }
    }

    // horizontal edge? no distance
    return 0;
}
gb_float_t gb_tessellator_distance_v(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t left, gb_mesh_vertex_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assert_abort(gb_tessellator_vertex_le_v(left, center));
    tb_assert_abort(gb_tessellator_vertex_le_v(center, right));

    // the points
    gb_point_ref_t pc = gb_tessellator_vertex_point(center);
    gb_point_ref_t pl = gb_tessellator_vertex_point(left);
    gb_point_ref_t pr = gb_tessellator_vertex_point(right);
    tb_assert_abort(pc && pl && pr);

    // compute the left and right x-distances
    gb_float_t xl = pc->x - pl->x;
    gb_float_t xr = pr->x - pc->x;
    tb_assert_abort(!gb_lz(xl) && !gb_lz(xr));

    // edge(left, right) is not vertical?
    if (gb_bz(xl + xr))
    {
        // center.x is close to left.x?
        if (xl < xr)
        {
            // compute distance
            gb_float_t factor = gb_div(xl, xl + xr);
            return (pc->y - pl->y) + gb_mul(pl->y - pr->y, factor);
        } 
        // center.x is close to right.x?
        else 
        {
            // compute distance
            gb_float_t factor = gb_div(xr, xl + xr);
            return (pc->y - pr->y) + gb_mul(pr->y - pl->y, factor);
        }
    }

    // vertical edge? no distance
    return 0;
}
tb_long_t gb_tessellator_position_h(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t upper, gb_mesh_vertex_ref_t lower)
{
    // check
    tb_assert_abort(center && upper && lower);

    // must be upper <= center <= lower
    tb_assert_abort(gb_tessellator_vertex_le_h(upper, center));
    tb_assert_abort(gb_tessellator_vertex_le_h(center, lower));

    // the points
    gb_point_ref_t pc = gb_tessellator_vertex_point(center);
    gb_point_ref_t pu = gb_tessellator_vertex_point(upper);
    gb_point_ref_t pl = gb_tessellator_vertex_point(lower);
    tb_assert_abort(pc && pu && pl);

    // compute the upper and lower y-distances
    gb_float_t yu = pc->y - pu->y;
    gb_float_t yl = pl->y - pc->y;
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
#ifdef GB_CONFIG_FLOAT_FIXED
        return (tb_long_t)((tb_hong_t)(pc->x - pl->x) * yu + (tb_hong_t)(pc->x - pu->x) * yl);
#else
        return gb_sign((pc->x - pl->x) * yu + (pc->x - pu->x) * yl);
#endif
    }

    // horizontal edge
    return 0;
}
tb_long_t gb_tessellator_position_v(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t left, gb_mesh_vertex_ref_t right)
{
    // check
    tb_assert_abort(center && left && right);

    // must be left <= center <= right
    tb_assert_abort(gb_tessellator_vertex_le_v(left, center));
    tb_assert_abort(gb_tessellator_vertex_le_v(center, right));

    // the points
    gb_point_ref_t pc = gb_tessellator_vertex_point(center);
    gb_point_ref_t pl = gb_tessellator_vertex_point(left);
    gb_point_ref_t pr = gb_tessellator_vertex_point(right);
    tb_assert_abort(pc && pl && pr);

    // compute the left and right x-distances
    gb_float_t xl = pc->x - pl->x;
    gb_float_t xr = pr->x - pc->x;
    tb_assert_abort(!gb_lz(xl) && !gb_lz(xr));

    // edge(left, right) is not vertical?
    if (gb_bz(xl + xr))
    {
        // compute the position
#ifdef GB_CONFIG_FLOAT_FIXED
        return (tb_long_t)((tb_hong_t)(pc->y - pr->y) * xl + (tb_hong_t)(pc->y - pl->y) * xr);
#else
        return gb_sign((pc->y - pr->y) * xl + (pc->y - pl->y) * xr);
#endif
    }

    // vertical edge
    return 0;
}



