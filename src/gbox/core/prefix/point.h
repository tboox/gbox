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
 * @file        point.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_POINT_H
#define GB_CORE_PREFIX_POINT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// points is equal?
#define gb_point_eq(a, b)       ((a)->x == (b)->x && (a)->y == (b)->y)

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! make point
 *
 * @param point     the point
 * @param x         the x
 * @param y         the y
 */
tb_void_t           gb_point_make(gb_point_ref_t point, gb_float_t x, gb_float_t y);

/*! make point with the integer value
 *
 * @param point     the point
 * @param x         the x
 * @param y         the y
 */
tb_void_t           gb_point_imake(gb_point_ref_t point, tb_long_t x, tb_long_t y);

/*! apply matrix to point
 *
 * @param point      the point 
 * @param matrix    the matrix
 */
tb_void_t           gb_point_apply(gb_point_ref_t point, gb_matrix_ref_t matrix);

/*! apply matrix to point
 *
 * @param point      the point 
 * @param applied   the applied point 
 * @param matrix    the matrix
 */
tb_void_t           gb_point_apply2(gb_point_ref_t point, gb_point_ref_t applied, gb_matrix_ref_t matrix);

/*! the distance of the point to the other point
 *
 * @param point     the point
 * @param other     the other point
 */
gb_float_t          gb_point_distance(gb_point_ref_t point, gb_point_ref_t other);

/*! be equal to the other point?
 *
 * @param point     the point
 * @param other     the other point
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_point_near_eq(gb_point_ref_t point, gb_point_ref_t other);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
