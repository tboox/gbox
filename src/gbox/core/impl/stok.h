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
 * @file        stok.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_STOK_H
#define GB_CORE_IMPL_STOK_H

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

/* make the filled polygon for lines
 *
 * @param filled_points     the filled polygon points
 * @param filled_counts     the filled polygon counts
 * @param paint             the paint
 * @param points            the stroked points 
 * @param count             the stroked points count 
 *
 * @return                  is convex polygon?
 */
tb_bool_t                   gb_stok_make_fill_for_lines(tb_vector_ref_t filled_points, tb_vector_ref_t filled_counts, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* make the filled polygon for points
 *
 * @param filled_points     the filled polygon points
 * @param filled_counts     the filled polygon counts
 * @param paint             the paint
 * @param points            the stroked points
 * @param count             the stroked points count
 *
 * @return                  is convex polygon?
 */
tb_bool_t                   gb_stok_make_fill_for_points(tb_vector_ref_t filled_points, tb_vector_ref_t filled_counts, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count);

/* make the filled polygon for polygon
 *
 * @param filled_points     the filled polygon points
 * @param filled_counts     the filled polygon counts
 * @param paint             the paint
 * @param polygon           the stroked polygon
 *
 * @return                  is convex polygon?
 */
tb_bool_t                   gb_stok_make_fill_for_polygon(tb_vector_ref_t filled_points, tb_vector_ref_t filled_counts, gb_paint_ref_t paint, gb_polygon_ref_t polygon);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


