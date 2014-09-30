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
 * @file        triangle.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "triangle.h"
#include "point.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_triangle_make(gb_triangle_ref_t triangle, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // check
    tb_assert_abort(triangle);

    // make it
    triangle->p0.x = x0;
    triangle->p0.y = y0;
    triangle->p1.x = x1;
    triangle->p1.y = y1;
    triangle->p2.x = x2;
    triangle->p2.y = y2;
}
tb_void_t gb_triangle_imake(gb_triangle_ref_t triangle, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    gb_triangle_make(triangle, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(x1), gb_long_to_float(y1), gb_long_to_float(x2), gb_long_to_float(y2));
}
tb_void_t gb_triangle_apply(gb_triangle_ref_t triangle, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_abort(triangle);
    
    // apply it
	gb_point_apply(&triangle->p0, matrix);
	gb_point_apply(&triangle->p1, matrix);
	gb_point_apply(&triangle->p2, matrix);
}
tb_void_t gb_triangle_apply2(gb_triangle_ref_t triangle, gb_triangle_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_abort(triangle && applied);
    
    // apply it
	gb_point_apply2(&triangle->p0, &applied->p0, matrix);
	gb_point_apply2(&triangle->p1, &applied->p1, matrix);
	gb_point_apply2(&triangle->p2, &applied->p2, matrix);
}
