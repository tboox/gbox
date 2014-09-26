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
 * @file        stroker.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_STROKER_H
#define GB_CORE_IMPL_STROKER_H

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

// the stroker type
typedef struct{}*           gb_stroker_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init stroker
 *
 * @return                  the stroker
 */
gb_stroker_ref_t            gb_stroker_init(tb_noarg_t);

/* exit stroker
 * 
 * @param stroker           the stroker
 */
tb_void_t                   gb_stroker_exit(gb_stroker_ref_t stroker);

/* clear stroker
 * 
 * @param stroker           the stroker
 */
tb_void_t                   gb_stroker_clear(gb_stroker_ref_t stroker);

/* apply paint to stroker
 * 
 * @param stroker           the stroker
 * @param paint             the paint
 */
tb_void_t                   gb_stroker_apply_paint(gb_stroker_ref_t stroker, gb_paint_ref_t paint);

/* close 
 * 
 * @param stroker           the stroker
 */
tb_void_t                   gb_stroker_clos(gb_stroker_ref_t stroker);

/*! move to the point
 *
 * @param stroker           the stroker
 * @param point             the point
 */
tb_void_t                   gb_stroker_move_to(gb_stroker_ref_t stroker, gb_point_ref_t point);

/*! line to the point
 *
 * @param stroker           the stroker
 * @param point             the point
 */
tb_void_t                   gb_stroker_line_to(gb_stroker_ref_t stroker, gb_point_ref_t point);

/*! quad to the ctrl, point
 *
 * @param stroker           the stroker
 * @param ctrl              the control point
 * @param point             the point
 */
tb_void_t                   gb_stroker_quad_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl, gb_point_ref_t point);

/*! cube to the ctrl0, ctrl1, point
 *
 * @param stroker           the stroker
 * @param ctrl0             the control0 point
 * @param ctrl1             the control1 point
 * @param point             the point
 */
tb_void_t                   gb_stroker_cube_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point);

/* add path 
 * 
 * @param stroker           the stroker
 * @param path              the path 
 */
tb_void_t                   gb_stroker_add_path(gb_stroker_ref_t stroker, gb_path_ref_t path);

/* add lines 
 * 
 * @param stroker           the stroker
 * @param points            the points 
 * @param count             the points count 
 */
tb_void_t                   gb_stroker_add_lines(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count);

/* add points
 * 
 * @param stroker           the stroker
 * @param points            the points 
 * @param count             the points count 
 */
tb_void_t                   gb_stroker_add_points(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count);

/* add polygon 
 * 
 * @param stroker           the stroker
 * @param polygon           the polygon 
 */
tb_void_t                   gb_stroker_add_polygon(gb_stroker_ref_t stroker, gb_polygon_ref_t polygon);

/* done stroker and get the stroked path 
 * 
 * @param stroker           the stroker
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done(gb_stroker_ref_t stroker);

/* done path to stroker and get the stroked path 
 * 
 * @param stroker           the stroker
 * @param paint             the paint 
 * @param path              the path 
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done_path(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_path_ref_t path);

/* done lines to stroker and get the stroked path 
 * 
 * @param stroker           the stroker
 * @param paint             the paint 
 * @param points            the points 
 * @param count             the points count 
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done_lines(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* done points to stroker and get the stroked path 
 * 
 * @param stroker           the stroker
 * @param paint             the paint 
 * @param points            the points 
 * @param count             the points count 
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done_points(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* done polygon to stroker and get the stroked path 
 * 
 * @param stroker           the stroker
 * @param paint             the paint 
 * @param polygon           the polygon 
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done_polygon(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_polygon_ref_t polygon);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


