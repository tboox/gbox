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
 * @file        polygon_raster.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_POLYGON_RASTER_H
#define GB_CORE_IMPL_POLYGON_RASTER_H

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

// the polygon raster rule enum
typedef enum __gb_polygon_raster_rule_e
{
    GB_POLYGON_RASTER_RULE_ODD      = GB_PAINT_FILL_RULE_ODD     //< odd 
,   GB_POLYGON_RASTER_RULE_NONZERO  = GB_PAINT_FILL_RULE_NONZERO //< non-zero 

}gb_polygon_raster_rule_e;

// the polygon raster edge type
typedef struct __gb_polygon_raster_edge_t
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
    tb_int8_t       winding : 2;

    // the index of next edge at the edge pool 
    tb_uint16_t     next;

    // the y value at the bottom of edge
    tb_int16_t      bottom_y;

    // the x value of the active edge
    tb_fixed_t      x;

    // the slope of the edge: dx / dy 
    tb_fixed_t      slope;

}gb_polygon_raster_edge_t, *gb_polygon_raster_edge_ref_t;

// the polygon raster ref type
typedef struct{}*       gb_polygon_raster_ref_t;

/* the polygon raster func type
 *
 * @param yb            the start y-coordinate
 * @param ye            the end y-coordinate 
 * @param edge_lsh      the left-hand edge
 * @param edge_rsh      the right-hand edge
 * @param priv          the private data
 */
typedef tb_void_t       (*gb_polygon_raster_func_t)(tb_long_t yb, tb_long_t ye, gb_polygon_raster_edge_ref_t edge_lsh, gb_polygon_raster_edge_ref_t edge_rsh, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init raster
 *
 * @return              the raster
 */
gb_polygon_raster_ref_t gb_polygon_raster_init(tb_noarg_t);

/* exit raster
 *
 * @param raster        the raster
 */
tb_void_t               gb_polygon_raster_exit(gb_polygon_raster_ref_t raster);

/* done raster
 *
 * @param raster        the raster
 * @param polygon       the polygon
 * @param bounds        the bounds
 * @param rule          the raster rule
 * @param func          the raster func
 * @param priv          the private data
 */
tb_void_t               gb_polygon_raster_done(gb_polygon_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_polygon_raster_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


