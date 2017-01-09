/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        type.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_TYPE_H
#define GB_CORE_PREFIX_TYPE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"
#include "pixfmt.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! @def gb_float_t
 *
 * the float type
 */
#ifdef GB_CONFIG_FLOAT_FIXED
typedef tb_fixed_t      gb_float_t;
typedef tb_int64_t      gb_double_t;
#else
typedef tb_float_t      gb_float_t;
typedef tb_double_t     gb_double_t;
#endif

/// the pixel type
typedef tb_uint32_t     gb_pixel_t;

/*! @def gb_color_t 
 *
 * the color type
 *
 * @code
 * union __gb_c2p_t
 * {
 *      gb_color_t c;
 *      gb_pixel_t p;
 *
 * }c2p;
 * c2p.c = c;
 * return c2p.p;
 * @endcode
 *
 */
#ifdef TB_WORDS_BIGENDIAN
typedef struct __gb_color_t
{
    tb_byte_t           a;
    tb_byte_t           r;
    tb_byte_t           g;
    tb_byte_t           b;

}gb_color_t, *gb_color_ref_t;
#else
typedef struct __gb_color_t
{
    tb_byte_t           b;
    tb_byte_t           g;
    tb_byte_t           r;
    tb_byte_t           a;

}gb_color_t, *gb_color_ref_t;
#endif

/*! the matrix type
 *
 * <pre>
 * sx: the x-scale
 * sy: the y-scale
 * kx: the x-skew
 * ky: the y-skew
 * tx: the x-translate
 * ty: the y-translate
 *
 * x' = x * sx + y * kx + tx
 * y' = x * ky + y * sy + ty
 *
 * x'           sx kx tx    x * sx + y * kx + tx
 * y' = x y 1 * ky sy ty =  x * ky + y * sy + ty
 * 1             0  0  1                       1
 *
 * the following table describes how the members of the matrix are used for each type of
 * operation:
 *                    sx                kx               ky              sy                tx     ty
 * rotation           sx * cos          sx * -sin        sy * sin        sy * cos          0      0
 * scaling            sx                0                0               sy                0      0
 * translation        0                 0                0               0                 tx     ty
 * ...
 *
 * </pre>
 *
 */
typedef struct __gb_matrix_t
{
	gb_float_t 		    sx, kx, tx;
	gb_float_t 		    ky, sy, ty;

}gb_matrix_t, *gb_matrix_ref_t;

/// the point type
typedef struct __gb_point_t
{
    gb_float_t          x;
    gb_float_t          y;

}gb_point_t, *gb_point_ref_t;

/// the rotate direction enum
typedef enum __gb_rotate_direction_e
{
    GB_ROTATE_DIRECTION_CW    = 0 //!< the clockwise direction
,   GB_ROTATE_DIRECTION_CCW   = 1 //!< the counter-clockwise direction

}gb_rotate_direction_e;

/// the vector type
typedef gb_point_t      gb_vector_t;

/// the vector ref type
typedef gb_point_ref_t  gb_vector_ref_t;

/// the line type
typedef struct __gb_line_t
{
    gb_point_t          p0;
    gb_point_t          p1;

}gb_line_t, *gb_line_ref_t;

/// the triangle type
typedef struct __gb_triangle_t
{
    gb_point_t          p0;
    gb_point_t          p1;
    gb_point_t          p2;

}gb_triangle_t, *gb_triangle_ref_t;

/// the rect type
typedef struct __gb_rect_t
{
    gb_float_t          x;
    gb_float_t          y;
    gb_float_t          w;
    gb_float_t          h;

}gb_rect_t, *gb_rect_ref_t;

/// the rect corner enum
typedef enum __gb_rect_corner_e
{
    GB_RECT_CORNER_LT   = 0     //<! the left-top corner    
,   GB_RECT_CORNER_RT   = 1     //<! the right-top corner    
,   GB_RECT_CORNER_RB   = 2     //<! the right-bottom corner    
,   GB_RECT_CORNER_LB   = 3     //<! the left-bottom corner    
,   GB_RECT_CORNER_MAXN = 4     //<! the corner max count

}gb_rect_corner_e;

/// the round rect type
typedef struct __gb_round_rect_t
{
    // the bounds
    gb_rect_t           bounds;

    /*! the radius of the four corners
     *
     * <pre>
     *  
     *  lt                     rt
     *   --------------------->
     * /|\                     |
     *  |                      |
     *  |                      |
     *  |                      |
     *  |                     \|/
     *  <----------------------
     *  lb                     rb
     *
     * </pre>
     */
    gb_vector_t         radius[GB_RECT_CORNER_MAXN];

}gb_round_rect_t, *gb_round_rect_ref_t;

/// the circle type
typedef struct __gb_circle_t
{
    /// the center point
    gb_point_t          c;

    /// the radius
    gb_float_t          r;

}gb_circle_t, *gb_circle_ref_t;

/// the ellipse type
typedef struct __gb_ellipse_t
{
    /// the center point
    gb_point_t          c;

    /// the x-radius 
    gb_float_t          rx;

    /// the y-radius
    gb_float_t          ry;

}gb_ellipse_t, *gb_ellipse_ref_t;

/// the arc type
typedef struct __gb_arc_t
{
    /// the center point
    gb_point_t          c;

    /// the x-radius 
    gb_float_t          rx;

    /// the y-radius 
    gb_float_t          ry;

    /// the start angle
    gb_float_t          ab;

    /// the sweep angle, clockwise: > 0, counter-clockwise: < 0
    gb_float_t          an;

}gb_arc_t, *gb_arc_ref_t;

/*! the polygon type
 *
 * @code
    gb_point_t      points[] = {    {x0, y0}, {x1, y1}, {x2, y2}
                                ,   {x3, y3}, {x4, y4}, {x5, y5}, {x3, y3}};
    tb_size_t       counts[] = {3, 4, 0};
    gb_polygon_t    polygon = {points, counts}; 
 * @endcode
 */
typedef struct __gb_polygon_t
{
    /// the points
    gb_point_ref_t      points;

    /// the counts
    tb_uint16_t*        counts;

    /// is convex?
    tb_bool_t           convex;

}gb_polygon_t, *gb_polygon_ref_t;

/// the gradient type
typedef struct __gb_gradient_t
{
    /// the colors
    gb_color_t*         colors;

    /// the radios
    gb_float_t*         radios;

    /// the count
    tb_size_t           count;

}gb_gradient_t, *gb_gradient_ref_t;

/// the path ref type
typedef tb_iterator_ref_t gb_path_ref_t;

/// the shape type enum
typedef enum __gb_shape_type_e
{
    GB_SHAPE_TYPE_NONE          = 0x0000 //!< none
,   GB_SHAPE_TYPE_ARC           = 0x0001 //!< arc
,   GB_SHAPE_TYPE_PATH          = 0x0002 //!< path
,   GB_SHAPE_TYPE_LINE          = 0x0003 //!< line
,   GB_SHAPE_TYPE_RECT          = 0x0004 //!< rect
,   GB_SHAPE_TYPE_POINT         = 0x0005 //!< point
,   GB_SHAPE_TYPE_CIRCLE        = 0x0006 //!< circle
,   GB_SHAPE_TYPE_ELLIPSE       = 0x0007 //!< ellipse
,   GB_SHAPE_TYPE_POLYGON       = 0x0008 //!< polygon
,   GB_SHAPE_TYPE_TRIANGLE      = 0x0009 //!< triangle
,   GB_SHAPE_TYPE_ROUND_RECT    = 0x0010 //!< round rect

}gb_shape_type_e;

/// the shape type
typedef struct __gb_shape_t
{
    /// the type
    tb_size_t           type;

    /// the shape
    union
    {
        /// the arc
        gb_arc_t        arc;

        /// the path
        gb_path_ref_t   path;

        /// the line
        gb_line_t       line;

        /// the rect
        gb_rect_t       rect;

        /// the point
        gb_point_t      point;

        /// the circle
        gb_circle_t     circle;
    
        /// the ellipse
        gb_ellipse_t    ellipse;
 
        /// the polygon
        gb_polygon_t    polygon;
 
        /// the triangle
        gb_triangle_t   triangle;
 
        /// the round rect
        gb_round_rect_t round_rect;
    }u;

}gb_shape_t, *gb_shape_ref_t;

/// the device ref type
typedef struct{}*       gb_device_ref_t;

/// the canvas ref type
typedef struct{}*       gb_canvas_ref_t;

/// the paint ref type
typedef struct{}*       gb_paint_ref_t;

/// the bitmap ref type
typedef struct{}*       gb_bitmap_ref_t;

/// the shader ref type
typedef struct{}*       gb_shader_ref_t;

/// the clipper ref type
typedef struct{}*       gb_clipper_ref_t;

#endif


