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
 * @file        polygon_raster.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "polygon_raster"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "polygon_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_polygon_raster_init(gb_polygon_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(raster && polygon && polygon->points && polygon->counts && bounds);

    // empty polygon?
    tb_check_return_val(gb_nz(bounds->w) && gb_nz(bounds->h), tb_false);

    // init top and bottom
    raster->top     = gb_float_to_long(bounds->y);
    raster->bottom  = gb_float_to_long(bounds->y + bounds->h);

    // init the edge table
    gb_point_t      pb;
    gb_point_t      pe;
    tb_uint16_t     index = 0;
    tb_uint16_t     edge_index = 0;
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    while (index < count)
    {
        // the point
        pe = *points++;

        // edge?
        if (index) 
        {
            // update the edge index
            edge_index++;

            // check
            tb_assert_abort(edge_index < tb_arrayn(raster->edge_pool));

            // make a new edge from the edge pool
            gb_polygon_raster_edge_ref_t edge = &raster->edge_pool[edge_index];

            // sort the points of the edge
            gb_point_ref_t top = &pb;
            gb_point_ref_t bottom = &pe;
            if (top->y < bottom->y)
            {
                top = &pe;
                bottom = &pb;
            }

            // the top and bottom coordinates
//            tb_long_t top_x     = gb_float_to_long(top->x);
            tb_long_t top_y     = gb_float_to_long(top->y);
//            tb_long_t bottom_x  = gb_float_to_long(bottom->x);
            tb_long_t bottom_y  = gb_float_to_long(bottom->y);
            tb_assert_abort(bottom_y >= top_y && top_y >= raster->bottom);

            /* insert edge to the head of the edge table
             *
             * table[index]: => edge => edge => .. => 0
             *              |
             *            insert
             */
            edge->next = raster->edge_table[top_y - raster->bottom];
            raster->edge_table[top_y - raster->bottom] = edge_index;
        }

        // save the previous point
        pb = pe;
        
        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }

    // ok
    return tb_true;
}
tb_void_t gb_polygon_raster_exit(gb_polygon_raster_ref_t raster)
{
}
tb_void_t gb_polygon_raster_done(gb_polygon_raster_ref_t raster, gb_polygon_raster_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(raster && func);

}

