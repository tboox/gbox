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
 * @file        polygon_cutter.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_POLYGON_CUTTER_H
#define GB_CORE_IMPL_POLYGON_CUTTER_H

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

// the cutter for the concave polygon
typedef struct{}*       gb_polygon_cutter_ref_t;

/* the polygon cutter func type
 * 
 * @param points        the points of the convex contour
 * @param count         the point count
 * @param priv          the user private data
 */
typedef tb_void_t       (*gb_polygon_cutter_func_t)(gb_point_ref_t points, tb_uint16_t count, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init cutter
 *
 * @return                  the cutter
 */
gb_polygon_cutter_ref_t     gb_polygon_cutter_init(tb_noarg_t);

/* exit cutter
 *
 * @param cutter            the cutter
 */
tb_void_t                   gb_polygon_cutter_exit(gb_polygon_cutter_ref_t cutter);

/* done cutter
 *
 * @param cutter            the cutter
 * @param polygon           the polygon 
 * @param bounds            the bounds of the polygon
 * @param rule              the cutter rule
 * @param func              the cutter func
 * @param priv              the user private data
 */
tb_void_t                   gb_polygon_cutter_done(gb_polygon_cutter_ref_t cutter, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_polygon_cutter_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
