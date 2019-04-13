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
 * @file        matrix.h
 *
 */
#ifndef GB_CORE_DEVICE_GL_MATRIX_H
#define GB_CORE_DEVICE_GL_MATRIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl matrix type
typedef gb_GLfloat_t    gb_gl_matrix_t[16];

// the gl matrix ref type
typedef gb_GLfloat_t*   gb_gl_matrix_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* init matrix
 *
 * @param sx        the x-scale
 * @param kx        the x-skew
 * @param ky        the y-skew
 * @param sy        the y-scale
 * @param tx        the x-translate
 * @param ty        the y-translate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sx, gb_GLfloat_t kx, gb_GLfloat_t ky, gb_GLfloat_t sy, gb_GLfloat_t tx, gb_GLfloat_t ty)
{
    matrix[0]  = sx;
    matrix[1]  = ky;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;
    matrix[4]  = kx;
    matrix[5]  = sy;
    matrix[6]  = 0.0f; 
    matrix[7]  = 0.0f;       
    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f; 
    matrix[10] = 1.0f; 
    matrix[11] = 0.0f; 
    matrix[12] = tx; 
    matrix[13] = ty; 
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

/*! init matrix from the sin and cos value 
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_sincos(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sin, gb_GLfloat_t cos)
{
    gb_gl_matrix_init(matrix, cos, -sin, sin, cos, 0.0f, 0.0f);    
}

/*! init matrix from the sin and cos value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_sincosp(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sin, gb_GLfloat_t cos, gb_GLfloat_t px, gb_GLfloat_t py)
{
    gb_GLfloat_t const one_cos = 1.0f - cos;
    gb_gl_matrix_init(matrix, cos, -sin, sin, cos, sin * py + one_cos * px, -sin * px + one_cos * py); 
}

/*! init matrix from the rotate degree 
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees 
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_rotate(gb_gl_matrix_ref_t matrix, gb_GLfloat_t degrees)
{
    gb_GLfloat_t s;
    gb_GLfloat_t c;
    tb_sincosf((gb_GLfloat_t)((degrees * TB_PI) / 180.0f), &s, &c);
    gb_gl_matrix_init_sincos(matrix, s, c);
}

/*! init matrix from the rotate degree by the coordinate: (px, py)
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees 
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_rotatep(gb_gl_matrix_ref_t matrix, gb_GLfloat_t degrees, gb_GLfloat_t px, gb_GLfloat_t py)
{
    gb_GLfloat_t s;
    gb_GLfloat_t c;
    tb_sincosf((gb_GLfloat_t)((degrees * TB_PI) / 180.0f), &s, &c);
    gb_gl_matrix_init_sincosp(matrix, s, c, px, py);
}

/*! init matrix from the skew value 
 *
 * @param matrix    the matrix
 * @param kx        the x-skew
 * @param ky        the y-skew
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_skew(gb_gl_matrix_ref_t matrix, gb_GLfloat_t kx, gb_GLfloat_t ky)
{
    gb_gl_matrix_init(matrix, 1.0f, kx, ky, 1.0f, 0.0f, 0.0f);
}

/*! init matrix from the skew value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param kx        the x-skew
 * @param ky        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_skewp(gb_gl_matrix_ref_t matrix, gb_GLfloat_t kx, gb_GLfloat_t ky, gb_GLfloat_t px, gb_GLfloat_t py)
{
    gb_gl_matrix_init(matrix, 1.0f, kx, ky, 1.0f, -kx * py, -ky * px);
}

/*! init matrix from the scale value 
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_scale(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sx, gb_GLfloat_t sy)
{
    gb_gl_matrix_init(matrix, sx, 0.0f, 0.0f, sy, 0.0f, 0.0f);
}

/*! init matrix from the scale value by the coordinate: (px, py) 
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_scalep(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sx, gb_GLfloat_t sy, gb_GLfloat_t px, gb_GLfloat_t py)
{
    gb_gl_matrix_init(matrix, sx, 0.0f, 0.0f, sy, px - sx * px, py - sy * py);
}

/*! init matrix from the translate value
 *
 * @param matrix    the matrix
 * @param tx        the x-translate
 * @param ty        the y-translate 
 */
static __tb_inline__ tb_void_t gb_gl_matrix_init_translate(gb_gl_matrix_ref_t matrix, gb_GLfloat_t tx, gb_GLfloat_t ty)
{
    gb_gl_matrix_init(matrix, 1.0f, 0.0f, 0.0f, 1.0f, tx, ty);
}

/*! reset to the identity matrix
 *
 * @param matrix    the matrix
 */
static __tb_inline__ tb_void_t gb_gl_matrix_clear(gb_gl_matrix_ref_t matrix)
{
    gb_gl_matrix_init(matrix, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
}

/*! copy matrix
 *
 * @param matrix    the matrix
 * @param copied    the copied matrix
 */
static __tb_inline__ tb_void_t gb_gl_matrix_copy(gb_gl_matrix_ref_t matrix, gb_gl_matrix_ref_t copied)
{
    tb_memcpy(matrix, copied, sizeof(gb_gl_matrix_t));
}

/*! multiply matrix with the given translate factor
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param factor    the factor
 */
static __tb_inline__ tb_void_t gb_gl_matrix_multiply(gb_gl_matrix_ref_t matrix, gb_gl_matrix_ref_t factor)
{
    gb_GLfloat_t sx = matrix[0] * factor[0] + matrix[4] * factor[1];
    gb_GLfloat_t ky = matrix[1] * factor[0] + matrix[5] * factor[1];

    gb_GLfloat_t kx = matrix[0] * factor[4] + matrix[4] * factor[5];
    gb_GLfloat_t sy = matrix[1] * factor[4] + matrix[5] * factor[5];

    gb_GLfloat_t tx = matrix[0] * factor[12] + matrix[4] * factor[13] + matrix[12];
    gb_GLfloat_t ty = matrix[1] * factor[12] + matrix[5] * factor[13] + matrix[13];

    gb_gl_matrix_init(matrix, sx, kx, ky, sy, tx, ty);
}

/*! multiply matrix with the given translate factor
 *
 * matrix = factor * matrix
 *
 * @param matrix    the matrix
 * @param factor    the factor
 */
static __tb_inline__ tb_void_t gb_gl_matrix_multiply_lhs(gb_gl_matrix_ref_t matrix, gb_gl_matrix_ref_t factor)
{
    gb_GLfloat_t sx = factor[0] * matrix[0] + factor[4] * matrix[1];
    gb_GLfloat_t ky = factor[1] * matrix[0] + factor[5] * matrix[1];

    gb_GLfloat_t kx = factor[0] * matrix[4] + factor[4] * matrix[5];
    gb_GLfloat_t sy = factor[1] * matrix[4] + factor[5] * matrix[5];

    gb_GLfloat_t tx = factor[0] * matrix[12] + factor[4] * matrix[13] + factor[12];
    gb_GLfloat_t ty = factor[1] * matrix[12] + factor[5] * matrix[13] + factor[13];

    gb_gl_matrix_init(matrix, sx, kx, ky, sy, tx, ty);
}

/*! transform matrix with the given scale value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param sx        the x-scale
 * @param sy        the y-scale
 */
static __tb_inline__ tb_void_t gb_gl_matrix_scale(gb_gl_matrix_ref_t matrix, gb_GLfloat_t sx, gb_GLfloat_t sy)
{
    matrix[0] *= sx;
    matrix[1] *= sx;
    matrix[4] *= sy;
    matrix[5] *= sy;
}

/*! transform matrix with the given rotate degrees
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 */
static __tb_inline__ tb_void_t gb_gl_matrix_rotate(gb_gl_matrix_ref_t matrix, gb_GLfloat_t degrees)
{
    // rotate
    gb_gl_matrix_t factor;
    gb_gl_matrix_init_rotate(factor, degrees);
    gb_gl_matrix_multiply(matrix, factor);
}

/*! transform matrix with the given rotate degrees by the coordinate: (px, py) 
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param degrees   the rotate degrees
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 */
static __tb_inline__ tb_void_t gb_gl_matrix_rotatep(gb_gl_matrix_ref_t matrix, gb_GLfloat_t degrees, gb_GLfloat_t px, gb_GLfloat_t py)
{
    // rotate
    gb_gl_matrix_t factor;
    gb_gl_matrix_init_rotatep(factor, degrees, px, py);
    gb_gl_matrix_multiply(matrix, factor);
}

/*! transform matrix with the given translate value
 *
 * matrix = matrix * factor
 *
 * @param matrix    the matrix
 * @param dx        the x-translate value
 * @param dy        the y-translate value
 */
static __tb_inline__ tb_void_t gb_gl_matrix_translate(gb_gl_matrix_ref_t matrix, gb_GLfloat_t dx, gb_GLfloat_t dy)
{
    matrix[12] += matrix[0] * dx + matrix[4] * dy;
    matrix[13] += matrix[1] * dx + matrix[5] * dy;
}

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
static __tb_inline__ gb_GLfloat_t gb_gl_matrix_apply_x(gb_gl_matrix_ref_t matrix, gb_GLfloat_t x, gb_GLfloat_t y)
{
    return (x * matrix[0]) + (y * matrix[4]) + matrix[12];
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
static __tb_inline__ gb_GLfloat_t gb_gl_matrix_apply_y(gb_gl_matrix_ref_t matrix, gb_GLfloat_t x, gb_GLfloat_t y)
{
    return (x * matrix[1]) + (y * matrix[5]) + matrix[13];
}

/* convert gb_matrix_t to the gl matrix
 *
 * @param matrix    the matrix
 * @param converted the converted matrix
 */
static __tb_inline__ tb_void_t gb_gl_matrix_convert(gb_gl_matrix_ref_t matrix, gb_matrix_ref_t converted)
{
    gb_gl_matrix_init(matrix, gb_float_to_tb(converted->sx), gb_float_to_tb(converted->kx), gb_float_to_tb(converted->ky), gb_float_to_tb(converted->sy), gb_float_to_tb(converted->tx), gb_float_to_tb(converted->ty));
}

/* multiply the current matrix by an orthographic matrix 
 *
 * @param matrix    the matrix
 * @param left      the left position
 * @param right     the right position
 * @param bottom    the bottom position
 * @param top       the top position
 * @param nearp     the near position
 * @param farp      the far position
 */
static __tb_inline__ tb_void_t gb_gl_matrix_orthof(gb_gl_matrix_ref_t matrix, gb_GLfloat_t left, gb_GLfloat_t right, gb_GLfloat_t bottom, gb_GLfloat_t top, gb_GLfloat_t nearp, gb_GLfloat_t farp)
{
    // clear matrix first
    gb_gl_matrix_clear(matrix);

    // done
    matrix[0]  = 2.0f / (right - left);
    matrix[5]  = 2.0f / (top - bottom);
    matrix[10] = -2.0f / (farp - nearp);  
    matrix[12] = -(right + left) / (right - left);  
    matrix[13] = -(top + bottom) / (top - bottom);  
    matrix[14] = -(farp + nearp) / (farp - nearp); 
}

#endif
