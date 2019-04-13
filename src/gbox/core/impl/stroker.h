/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
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
 * macros
 */

// the default miter limit
#define GB_STROKER_DEFAULT_MITER                gb_long_to_float(4)

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

/*! cubic to the ctrl0, ctrl1, point
 *
 * @param stroker           the stroker
 * @param ctrl0             the control0 point
 * @param ctrl1             the control1 point
 * @param point             the point
 */
tb_void_t                   gb_stroker_cubic_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point);

/* add path 
 * 
 * @param stroker           the stroker
 * @param path              the path 
 */
tb_void_t                   gb_stroker_add_path(gb_stroker_ref_t stroker, gb_path_ref_t path);

/* add rect 
 * 
 * @param stroker           the stroker
 * @param rect              the rect 
 */
tb_void_t                   gb_stroker_add_rect(gb_stroker_ref_t stroker, gb_rect_ref_t rect);

/* add circle 
 * 
 * @param stroker           the stroker
 * @param circle              the circle 
 */
tb_void_t                   gb_stroker_add_circle(gb_stroker_ref_t stroker, gb_circle_ref_t circle);

/* add ellipse 
 * 
 * @param stroker           the stroker
 * @param ellipse           the ellipse 
 */
tb_void_t                   gb_stroker_add_ellipse(gb_stroker_ref_t stroker, gb_ellipse_ref_t ellipse);

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
 * @param convex            is convex?
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done(gb_stroker_ref_t stroker, tb_bool_t convex);

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
 * @param hint              the hint shape
 *
 * @return                  the stroked path
 */
gb_path_ref_t               gb_stroker_done_polygon(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_polygon_ref_t polygon, gb_shape_ref_t hint);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


