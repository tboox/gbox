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
