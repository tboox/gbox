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
#ifndef GB_CORE_IMPL_GEOMETRY_H
#define GB_CORE_IMPL_GEOMETRY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the polygon edges maxn
#ifdef __gb_small__
#   define GB_POLYGON_RASTER_EDGES_MAXN     (2048)
#else
#   define GB_POLYGON_RASTER_EDGES_MAXN     (2048 << 1)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the polygon raster edge type
typedef struct __gb_polygon_raster_edge_t
{
#if 0
    // the y value at the bottom of edge
    tb_int16_t      btm;

    // the x value at the top of edge
    tb_int16_t      x;

    // the direction of y, top => btm: 1 else 0
    tb_int16_t      yd:     1;

    // for bresenham algorithm
    tb_int16_t      xinc:   15;
    tb_int16_t      dx2;
    tb_int16_t      dy2;
    tb_int16_t      error;
    tb_int16_t      line;
#endif

    // the index of next edge at the edge pool 
    tb_uint16_t     next;

}gb_polygon_raster_edge_t, *gb_polygon_raster_edge_ref_t;

/* the polygon raster type
 *
 * 1. make the edge table    
 *     (y)
 *      0 ----------------> . 
 *      1                 .   .
 *      2               .       . e2
 *      3          e1 .           .
 *      4 ------------------------> . 
 *      5         .               .
 *      6       .               .
 *      7 --> .               . e3
 *      8       .           .
 *      9      e4 .       .
 *      10          .   .
 *      11            .
 *
 * edge_table[0]: e1 e2
 * edge_table[4]: e3
 * edge_table[7]: e4
 *
 * 2. scanning the edge table  
 *     (y)
 *      0                   . 
 *      1                 . - .
 *      2               . ----- . e2
 *      3          e1 . --------- .
 *      4           .               . 
 *      5         .               .
 *      6       .               .
 *      7     .               . e3
 *      8       .           .
 *      9      e4 .       .
 *      10          .   .
 *      11            .
 *
 * active_edge_table: e1 e2
 *
 * 3. scanning the edge table  
 *     (y)
 *      0                   . 
 *      1                 .   .
 *      2               .       . e2
 *      3          e1 .           .
 *      4           . ------------- . 
 *      5         . ------------- .
 *      6       . ------------- .
 *      7     .               . e3
 *      8       .           .
 *      9      e4 .       .
 *      10          .   .
 *      11            .
 *
 * active_edge_table: e1 e3
 *
 * 4. scanning the edge table  
 *     (y)
 *      0                   . 
 *      1                 .   .
 *      2               .       . e2
 *      3          e1 .           .
 *      4           .               . 
 *      5         .               .
 *      6       .               .
 *      7     . ------------- . e3
 *      8       . --------- .
 *      9      e4 . ----- .
 *      10          . - .
 *      11            .
 *
 * active_edge_table: e4 e3
 *
 * active_edge_table: be sorted by x in ascending
 *
 */
typedef struct __gb_polygon_raster_t
{
    // the edge pool, tail: 0, index: > 0
    gb_polygon_raster_edge_t    edge_pool[1 + GB_POLYGON_RASTER_EDGES_MAXN];

    // the edge table
    tb_uint16_t                 edge_table[GB_HEIGHT_MAXN];

    // the top of the polygon bounds
    tb_long_t                   top;

    // the bottom of the polygon bounds
    tb_long_t                   bottom;

}gb_polygon_raster_t, *gb_polygon_raster_ref_t;

/* the polygon raster func type
 *
 * @param y                     the y-coordinate
 * @param xb                    the start x-coordinate
 * @param xe                    the end x-coordinate 
 * @param priv                  the private data
 */
typedef tb_void_t               (*gb_polygon_raster_func_t)(tb_long_t y, tb_long_t xb, tb_long_t xe, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init polygon raster
 *
 * @param raster        the polygon raster
 * @param polygon       the polygon
 * @param bounds        the bounds
 *
 * @return              the polygon raster
 */
tb_bool_t               gb_polygon_raster_init(gb_polygon_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds);

/* exit polygon raster
 *
 * @param raster        the polygon raster
 */
tb_void_t               gb_polygon_raster_exit(gb_polygon_raster_ref_t raster);

/* done polygon raster
 *
 * @param raster        the polygon raster
 * @param func          the raster func
 * @param priv          the private data
 */
tb_void_t               gb_polygon_raster_done(gb_polygon_raster_ref_t raster, gb_polygon_raster_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


