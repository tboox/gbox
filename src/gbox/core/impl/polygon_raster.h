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
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the polygon raster type
typedef struct __gb_polygon_raster_t
{
    // the bounds
    gb_rect_ref_t       bounds;

    // the polygon 
    gb_polygon_ref_t    polygon;

}gb_polygon_raster_t, *gb_polygon_raster_ref_t;

/* the polygon raster func type
 *
 * @param y             the y-coordinate
 * @param xb            the start x-coordinate
 * @param xe            the end x-coordinate 
 * @param priv          the private data
 */
typedef tb_void_t       (*gb_polygon_raster_func_t)(tb_long_t y, tb_long_t xb, tb_long_t xe, tb_cpointer_t priv);

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


