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
 * @author		ruki
 * @file		matrix.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_MATRIX_H
#define GB_CORE_PREFIX_MATRIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
/*! init matrix
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param kx        the x-skew
 * @param ky        the y-skew
 * @param sy        the y-scale
 * @param tx        the x-translate
 * @param ty        the y-translate
 */
tb_void_t 		    gb_matrix_init(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t kx, gb_float_t ky, gb_float_t sy, gb_float_t tx, gb_float_t ty);

/*! init matrix from the rotate degree 
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees 
 */
tb_void_t 		    gb_matrix_init_rotate(gb_matrix_ref_t matrix, gb_float_t degrees);

/*! init matrix from the rotate degree by the coordinate: (px, py)
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees 
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
tb_void_t 		    gb_matrix_init_rotatep(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py);

/*! init matrix from the sin and cos value 
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 */
tb_void_t           gb_matrix_init_sincos(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos);

/*! init matrix from the sin and cos value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
tb_void_t 		    gb_matrix_init_sincosp(gb_matrix_ref_t matrix, 	gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py);

/*! init matrix from the skew value 
 *
 * @param matrix    the matrix
 * @param kx        the x-skew
 * @param ky        the y-skew
 */
tb_void_t 		    gb_matrix_init_skew(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky);

/*! init matrix from the skew value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param kx        the x-skew
 * @param ky        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
tb_void_t 		    gb_matrix_init_skewp(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py);

/*! init matrix from the scale value 
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 */
tb_void_t 	    	gb_matrix_init_scale(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy);

/*! init matrix from the scale value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
tb_void_t 		    gb_matrix_init_scalep(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py);

/*! init matrix from the translate value
 *
 * @param matrix    the matrix
 * @param tx        the x-translate
 * @param ty        the y-translate 
 */
tb_void_t 		    gb_matrix_init_translate(gb_matrix_ref_t matrix, gb_float_t tx, gb_float_t ty);

/*! reset to the identity matrix
 *
 * @param matrix    the matrix
 */
tb_void_t 		    gb_matrix_clear(gb_matrix_ref_t matrix);

/*! invert matrix
 *
 * @param matrix    the matrix
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_invert(gb_matrix_ref_t matrix);

/*! copy matrix
 *
 * @param matrix    the matrix
 * @param copied    the copied matrix
 */
tb_void_t 		    gb_matrix_copy(gb_matrix_ref_t matrix, gb_matrix_ref_t copied);

/*! is the identity matrix?
 *
 * @param matrix    the matrix
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_identity(gb_matrix_ref_t matrix);

/*! transform matrix with the given rotate degrees
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_rotate(gb_matrix_ref_t matrix, gb_float_t degrees);

/*! transform matrix with the given rotate degrees
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_rotate_lhs(gb_matrix_ref_t matrix, gb_float_t degrees);

/*! transform matrix with the given rotate degrees by the coordinate: (px, py) 
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_rotatep(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py);

/*! transform matrix with the given rotate degrees by the coordinate: (px, py) 
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_rotatep_lhs(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py);

/*! transform matrix with the given scale value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_scale(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy);

/*! transform matrix with the given scale value
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_scale_lhs(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy);

/*! transform matrix with the given scale value by the coordinate: (px, py) 
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_scalep(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py);

/*! transform matrix with the given scale value by the coordinate: (px, py) 
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sy        the y-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_scalep_lhs(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py);

/*! transform matrix with the given skew value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sk        the x-skew
 * @param sk        the y-skew
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_skew(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky);

/*! transform matrix with the given skew value
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sk        the x-skew
 * @param sk        the y-skew
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_skew_lhs(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky);

/*! transform matrix with the given skew value by the coordinate: (px, py)
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sk        the x-skew
 * @param sk        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_skewp(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py);

/*! transform matrix with the given skew value by the coordinate: (px, py)
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sk        the x-skew
 * @param sk        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_skewp_lhs(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py);

/*! transform matrix with the given sin and cos value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_sincos(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos);

/*! transform matrix with the given sin and cos value
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_sincos_lhs(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos);

/*! transform matrix with the given sin and cos value by the coordinate: (px, py)
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_sincosp(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py);

/*! transform matrix with the given sin and cos value by the coordinate: (px, py)
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_sincosp_lhs(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py);

/*! transform matrix with the given translate value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param dx        the x-translate value
 * @param dy        the y-translate value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_translate(gb_matrix_ref_t matrix, gb_float_t dx, gb_float_t dy);

/*! transform matrix with the given translate value
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param dx        the x-translate value
 * @param dy        the y-translate value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_translate_lhs(gb_matrix_ref_t matrix, gb_float_t dx, gb_float_t dy);

/*! multiply matrix with the given translate factor
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param factor    the factor
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_multiply(gb_matrix_ref_t matrix, gb_matrix_ref_t factor);

/*! multiply matrix with the given translate factor
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param factor    the factor
 *
 * @return          tb_true or tb_false
 */
tb_bool_t 		    gb_matrix_multiply_lhs(gb_matrix_ref_t matrix, gb_matrix_ref_t factor);

/*! apply matrix to the points
 *
 * @param matrix    the matrix 
 * @param points    the points
 * @param count     the count
 */
tb_void_t           gb_matrix_apply_points(gb_matrix_ref_t matrix, gb_point_ref_t points, tb_size_t count);

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/*! apply matrix to the x-coordinate
 *
 * x' = x * sx + y * kx + tx
 *
 * @param matrix    the matrix
 * @param x         the x value 
 * @param y         the y value
 *
 * @return          the new x value
 */
static __tb_inline__ gb_float_t gb_matrix_apply_x(gb_matrix_ref_t matrix, gb_float_t x, gb_float_t y)
{
    // check
    tb_assert(matrix);
    
    // apply it
	return gb_mul(x, matrix->sx) + gb_mul(y, matrix->kx) + matrix->tx;
}

/*! apply matrix to the y-coordinate
 *
 * y' = x * ky + y * sy + ty
 *
 * @param matrix    the matrix
 * @param x         the x value 
 * @param y         the y value
 *
 * @return          the new y value
 */
static __tb_inline__ gb_float_t gb_matrix_apply_y(gb_matrix_ref_t matrix, gb_float_t x, gb_float_t y)
{ 
    // check
    tb_assert(matrix);
    
    // apply it
	return gb_mul(x, matrix->ky) + gb_mul(y, matrix->sy) + matrix->ty;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
