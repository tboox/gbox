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
 * @file        line.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "line.h"
#include "point.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_line_make(gb_line_ref_t line, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1)
{
    // check
    tb_assert(line);

    // make it
    line->p0.x = x0;
    line->p0.y = y0;
    line->p1.x = x1;
    line->p1.y = y1;
}
tb_void_t gb_line_imake(gb_line_ref_t line, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1)
{
    gb_line_make(line, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(x1), gb_long_to_float(y1));
}
tb_void_t gb_line_apply(gb_line_ref_t line, gb_matrix_ref_t matrix)
{
    // check
    tb_assert(line);
    
    // apply it
	gb_point_apply(&line->p0, matrix);
	gb_point_apply(&line->p1, matrix);
}
tb_void_t gb_line_apply2(gb_line_ref_t line, gb_line_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert(line && applied);
    
    // apply it
	gb_point_apply2(&line->p0, &applied->p0, matrix);
	gb_point_apply2(&line->p1, &applied->p1, matrix);
}
