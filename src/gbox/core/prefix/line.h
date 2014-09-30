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
 * @file        line.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_LINE_H
#define GB_CORE_PREFIX_LINE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! make line
 *
 * @param line      the line
 * @param x0        the x0
 * @param y0        the y0
 * @param x1        the x1
 * @param y1        the y1
 */
tb_void_t           gb_line_make(gb_line_ref_t line, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1);

/*! make line with the integer value
 *
 * @param line      the line
 * @param x0        the x0
 * @param y0        the y0
 * @param x1        the x1
 * @param y1        the y1
 */
tb_void_t           gb_line_imake(gb_line_ref_t line, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1);

/*! apply matrix to line
 *
 * @param line      the line 
 * @param matrix    the matrix
 */
tb_void_t           gb_line_apply(gb_line_ref_t line, gb_matrix_ref_t matrix);

/*! apply matrix to line
 *
 * @param line      the line 
 * @param applied   the applied line 
 * @param matrix    the matrix
 */
tb_void_t           gb_line_apply2(gb_line_ref_t line, gb_line_ref_t applied, gb_matrix_ref_t matrix);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
