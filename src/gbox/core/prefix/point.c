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
 * @file        point.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "point.h"
#include "matrix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_point_make(gb_point_ref_t point, gb_float_t x, gb_float_t y) 
{
    // check
    tb_assert_abort(point);

    // make it
    point->x = x;
    point->y = y;
}
tb_void_t gb_point_imake(gb_point_ref_t point, tb_long_t x, tb_long_t y)
{
    gb_point_make(point, gb_long_to_float(x), gb_long_to_float(y));
}
tb_void_t gb_point_apply(gb_point_ref_t point, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_abort(point && matrix);
    
    // apply it
	gb_float_t x = point->x;
	gb_float_t y = point->y;
	point->x = gb_matrix_apply_x(matrix, x, y);
	point->y = gb_matrix_apply_y(matrix, x, y);
}
tb_void_t gb_point_apply2(gb_point_ref_t point, gb_point_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_abort(point && applied && matrix);
    
    // apply it
	gb_float_t x = point->x;
	gb_float_t y = point->y;
	applied->x = gb_matrix_apply_x(matrix, x, y);
	applied->y = gb_matrix_apply_y(matrix, x, y);
}
