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
 * @file        stroke.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_STROKE_H
#define GB_CORE_IMPL_STROKE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make the filled path for lines
 *
 * @param path              the filled path
 * @param paint             the paint
 * @param points            the stroked points 
 * @param count             the stroked points count 
 */
tb_void_t                   gb_stroke_make_fill_for_lines(gb_path_ref_t path, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* make the filled path for points
 *
 * @param path              the filled path
 * @param paint             the paint
 * @param points            the stroked points
 * @param count             the stroked points count
 */
tb_void_t                   gb_stroke_make_fill_for_points(gb_path_ref_t path, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* make the filled path for polygon
 *
 * @param path              the filled path
 * @param paint             the paint
 * @param polygon           the stroked polygon
 */
tb_void_t                   gb_stroke_make_fill_for_polygon(gb_path_ref_t path, gb_paint_ref_t paint, gb_polygon_ref_t polygon);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


