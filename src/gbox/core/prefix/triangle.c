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
 * @file        triangle.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "triangle.h"
#include "point.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_triangle_make(gb_triangle_ref_t triangle, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // check
    tb_assert(triangle);

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
    tb_assert(triangle);
    
    // apply it
	gb_point_apply(&triangle->p0, matrix);
	gb_point_apply(&triangle->p1, matrix);
	gb_point_apply(&triangle->p2, matrix);
}
tb_void_t gb_triangle_apply2(gb_triangle_ref_t triangle, gb_triangle_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert(triangle && applied);
    
    // apply it
	gb_point_apply2(&triangle->p0, &applied->p0, matrix);
	gb_point_apply2(&triangle->p1, &applied->p1, matrix);
	gb_point_apply2(&triangle->p2, &applied->p2, matrix);
}
