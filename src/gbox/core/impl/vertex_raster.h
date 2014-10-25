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
 * @file        vertex_raster.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_VERTEX_RASTER_H
#define GB_CORE_IMPL_VERTEX_RASTER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../paint.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the vertex raster rule enum
typedef enum __gb_vertex_raster_rule_e
{
    GB_VERTEX_RASTER_RULE_ODD      = GB_PAINT_FILL_RULE_ODD     //< odd 
,   GB_VERTEX_RASTER_RULE_NONZERO  = GB_PAINT_FILL_RULE_NONZERO //< non-zero 

}gb_vertex_raster_rule_e;

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
    tb_int8_t           winding     : 2;

    /* patch the point? 
     *
     * bottom:          1
     * intersection:    -1
     */
    tb_int8_t           patching    : 2;

    // the index of next edge at the edge pool 
    tb_uint16_t         next;

    // the top y-coordinate
    tb_fixed_t          y_top;

    // the bottom y-coordinate
    tb_fixed_t          y_bottom;

    // the x-coordinate of the active edge
    tb_fixed_t          x;

    // the slope of the edge: dx / dy 
    tb_fixed_t          slope;

}gb_vertex_raster_edge_t, *gb_vertex_raster_edge_ref_t;

// the polygon vertex raster ref type
typedef struct{}*       gb_vertex_raster_ref_t;

/* the vertex raster func type
 *
 * @param y             the y-coordinate
 * @param le            the left edge
 * @param re            the right edge
 * @param priv          the private data
 */
typedef tb_void_t       (*gb_vertex_raster_func_t)(tb_fixed_t y, gb_vertex_raster_edge_ref_t le, gb_vertex_raster_edge_ref_t re, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init raster
 *
 * @return              the raster
 */
gb_vertex_raster_ref_t  gb_vertex_raster_init(tb_noarg_t);

/* exit raster
 *
 * @param raster        the raster
 */
tb_void_t               gb_vertex_raster_exit(gb_vertex_raster_ref_t raster);

/* done raster
 *
 * @param raster        the raster
 * @param polygon       the polygon
 * @param bounds        the bounds
 * @param rule          the raster rule
 * @param func          the raster func
 * @param priv          the private data
 */
tb_void_t               gb_vertex_raster_done(gb_vertex_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_vertex_raster_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


