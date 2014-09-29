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
 * @file        vector.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_VECTOR_H
#define GB_CORE_PREFIX_VECTOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// make vector from point
#define gb_vector_make_from_point(point)    (*((gb_vector_ref_t)(point)))

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! nagate the vector
 * 
 * @param vector        the vector
 */
tb_void_t               gb_vector_negate(gb_vector_ref_t vector);

/*! rotate 90 degrees 
 * 
 * @param vector        the vector
 * @param direction     the direction
 */
tb_void_t               gb_vector_rotate(gb_vector_ref_t vector, tb_size_t direction);

/*! rotate 90 degrees to the given vector
 * 
 * @param vector        the vector
 * @param rotated       the rotated vector
 * @param direction     the direction
 */
tb_void_t               gb_vector_rotate2(gb_vector_ref_t vector, gb_vector_ref_t rotated, tb_size_t direction);

/*! scale the given value
 * 
 * @param vector        the vector
 * @param scale         the scale
 */
tb_void_t               gb_vector_scale(gb_vector_ref_t vector, gb_float_t scale);

/*! scale the given value to the given vector
 * 
 * @param vector        the vector
 * @param scaled        the scaled vector
 * @param scale         the scale
 */
tb_void_t               gb_vector_scale2(gb_vector_ref_t vector, gb_vector_ref_t scaled, gb_float_t scale);

/*! the vector length
 * 
 * @param vector        the vector
 */
gb_float_t              gb_vector_length(gb_vector_ref_t vector);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
