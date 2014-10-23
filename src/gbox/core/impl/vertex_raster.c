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
 * @file        vertex_raster.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "vertex_raster"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "vertex_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the vertex raster edge type
typedef struct __gb_vertex_raster_edge_t
{
    /* the winding for rule
     *
     *   . <= -1
     *     .
     *       . 
     *         .
     *            .  
     *              .
     *            => 1
     *
     * 1:  top => bottom
     * -1: bottom => top
     */
    tb_int8_t       winding     : 2;

    // the index of next edge at the edge pool 
    tb_uint16_t     next;

    // the bottom y-coordinate
    tb_int16_t      y_bottom;

    // the x-coordinate of the active edge
    tb_fixed_t      x;

    // the slope of the edge: dx / dy 
    tb_fixed_t      slope;

}gb_vertex_raster_edge_t, *gb_vertex_raster_edge_ref_t;

// the polygon vertex raster impl type
typedef struct __gb_vertex_raster_impl_t
{
}gb_vertex_raster_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_vertex_raster_ref_t gb_vertex_raster_init()
{
    // init it
    return (gb_vertex_raster_ref_t)tb_malloc0_type(gb_vertex_raster_impl_t);
}
tb_void_t gb_vertex_raster_exit(gb_vertex_raster_ref_t raster)
{
    // check
    gb_vertex_raster_impl_t* impl = (gb_vertex_raster_impl_t*)raster;
    tb_assert_and_check_return(impl);

    // exit it
    tb_free(impl);
}
tb_void_t gb_vertex_raster_done(gb_vertex_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_vertex_raster_func_t func, tb_cpointer_t priv)
{
    // check
    gb_vertex_raster_impl_t* impl = (gb_vertex_raster_impl_t*)raster;
    tb_assert_abort(impl && polygon && func);

}

