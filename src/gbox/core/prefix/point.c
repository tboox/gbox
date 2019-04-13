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
 * @file        point.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "point.h"
#include "float.h"
#include "vector.h"
#include "matrix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_point_make(gb_point_ref_t point, gb_float_t x, gb_float_t y) 
{
    // check
    tb_assert(point);

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
    tb_assert(point && matrix);
    
    // apply it
	gb_float_t x = point->x;
	gb_float_t y = point->y;
	point->x = gb_matrix_apply_x(matrix, x, y);
	point->y = gb_matrix_apply_y(matrix, x, y);
}
tb_void_t gb_point_apply2(gb_point_ref_t point, gb_point_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert(point && applied && matrix);
    
    // apply it
	gb_float_t x = point->x;
	gb_float_t y = point->y;
	applied->x = gb_matrix_apply_x(matrix, x, y);
	applied->y = gb_matrix_apply_y(matrix, x, y);
}
gb_float_t gb_point_distance(gb_point_ref_t point, gb_point_ref_t other)
{
    // check
    tb_assert(point && other);
 
    // make vector: point => other
    gb_vector_t vector;
    gb_vector_make(&vector, other->x - point->x, other->y - point->y);

    // the distance
    return gb_vector_length(&vector);
}
tb_bool_t gb_point_near_eq(gb_point_ref_t point, gb_point_ref_t other)
{
    // check
    tb_assert(point && other);
 
    // equal?
    return (gb_near_eq(point->x, other->x)) && (gb_near_eq(point->y, other->y));
}
