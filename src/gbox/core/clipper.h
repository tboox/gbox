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
 * @file        clipper.h
 * @ingroup     core
 */
#ifndef GB_CORE_CLIPPER_H
#define GB_CORE_CLIPPER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the clipper mode enum
typedef enum __gb_clipper_mode_e
{
    GB_CLIPPER_MODE_NONE        = 0x00
,   GB_CLIPPER_MODE_SUBTRACT    = 0x01
,   GB_CLIPPER_MODE_INTERSECT   = 0x02
,   GB_CLIPPER_MODE_UNION       = 0x03
,   GB_CLIPPER_MODE_REPLACE     = 0x04

}gb_clipper_mode_e;

/// the clipper shape enum
typedef enum __gb_clipper_shape_e
{
    GB_CLIPPER_SHAPE_RECT        = 0
,   GB_CLIPPER_SHAPE_PATH        = 1
,   GB_CLIPPER_SHAPE_CIRCLE      = 2
,   GB_CLIPPER_SHAPE_ELLIPSE     = 3
,   GB_CLIPPER_SHAPE_POLYGON     = 4
,   GB_CLIPPER_SHAPE_TRIANGLE    = 5

}gb_clipper_shape_e;

/// the clipper shape type
typedef struct __gb_clipper_shape_t
{
    /// the clipper mode
    tb_uint8_t              mode;

    /// the shape type
    tb_uint8_t              type;

    /// the shape
    union 
    {
        /// rect
        gb_rect_t           rect;
        
        /// path
        gb_path_ref_t       path;
            
        /// circle
        gb_circle_t         circle;

        /// ellipse
        gb_ellipse_t        ellipse;

        /// polygon
        gb_polygon_t        polygon;

        /// triangle
        gb_triangle_t       triangle;

    }shape;

    /// the clipper matrix
    gb_matrix_t             matrix;

}gb_clipper_shape_t;

/// the clipper shape ref type
typedef gb_clipper_shape_t* gb_clipper_shape_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init clipper
 *
 * @return                  the clipper
 */
gb_clipper_ref_t            gb_clipper_init(tb_noarg_t);

/*! exit clipper
 *
 * @param clipper           the clipper
 */
tb_void_t                   gb_clipper_exit(gb_clipper_ref_t clipper);

/*! the clipper shape count
 *
 * @param clipper           the clipper
 *
 * @return                  the item count
 */
tb_size_t                   gb_clipper_size(gb_clipper_ref_t clipper);

/*! get the clipper shape from the given index
 *
 * @param clipper           the clipper
 * @param index             the clipper shape index
 *
 * @return                  the clipper shape
 */
gb_clipper_shape_ref_t      gb_clipper_shape(gb_clipper_ref_t clipper, tb_size_t index);

/*! clear the clipper
 *
 * @param clipper           the clipper
 */
tb_void_t                   gb_clipper_clear(gb_clipper_ref_t clipper);

/*! copy clipper 
 *
 * @param clipper           the clipper 
 * @param copied            the copied clipper
 */
tb_void_t                   gb_clipper_copy(gb_clipper_ref_t clipper, gb_clipper_ref_t copied);

/*! get the current clipper matrix
 *
 * @param clipper           the clipper 
 *
 * @return                  the matrix
 */
gb_matrix_ref_t             gb_clipper_matrix(gb_clipper_ref_t clipper);

/*! set the current clipper matrix
 *
 * @param clipper           the clipper 
 * @param matrix            the matrix
 */
tb_void_t                   gb_clipper_matrix_set(gb_clipper_ref_t clipper, gb_matrix_ref_t matrix);

/*! add path
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param path              the path
 */
tb_void_t                   gb_clipper_add_path(gb_clipper_ref_t clipper, tb_size_t mode, gb_path_ref_t path);

/*! add triangle
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param triangle          the triangle
 */
tb_void_t                   gb_clipper_add_triangle(gb_clipper_ref_t clipper, tb_size_t mode, gb_triangle_ref_t triangle);

/*! add triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param x1                the x1-coordinate
 * @param y1                the y1-coordinate
 * @param x2                the x2-coordinate
 * @param y2                the y2-coordinate
 */
tb_void_t                   gb_clipper_add_triangle2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! add integer triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param x1                the x1-coordinate
 * @param y1                the y1-coordinate
 * @param x2                the x2-coordinate
 * @param y2                the y2-coordinate
 */
tb_void_t                   gb_clipper_add_triangle2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! add rect
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param rect              the rect
 */
tb_void_t                   gb_clipper_add_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_rect_ref_t rect);

/*! add rect(x, y, w, h)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x                 the x-coordinate
 * @param y                 the y-coordinate
 * @param w                 the width
 * @param h                 the height
 */
tb_void_t                   gb_clipper_add_rect2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h);

/*! add integer rect(x, y, w, h)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x                 the x-coordinate
 * @param y                 the y-coordinate
 * @param w                 the width
 * @param h                 the height
 */
tb_void_t                   gb_clipper_add_rect2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h);

/*! add circle
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param circle            the circle
 */
tb_void_t                   gb_clipper_add_circle(gb_clipper_ref_t clipper, tb_size_t mode, gb_circle_ref_t circle);

/*! add circle(x0, y0, r)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param r                 the radius
 */
tb_void_t                   gb_clipper_add_circle2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! add integer circle(x0, y0, r)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param r                 the radius
 */
tb_void_t                   gb_clipper_add_circle2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t r);

/*! add ellipse
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param ellipse           the ellipse
 */
tb_void_t                   gb_clipper_add_ellipse(gb_clipper_ref_t clipper, tb_size_t mode, gb_ellipse_ref_t ellipse);

/*! add ellipse(x0, y0, rx, ry)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param rx                the x-radius
 * @param ry                the y-radius
 */
tb_void_t                   gb_clipper_add_ellipse2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry);

/*! add integer ellipse(x0, y0, rx, ry)
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param x0                the x0-coordinate
 * @param y0                the y0-coordinate
 * @param rx                the x-radius
 * @param ry                the y-radius
 */
tb_void_t                   gb_clipper_add_ellipse2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry);

/*! add polygon
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param polygon           the polygon
 */
tb_void_t                   gb_clipper_add_polygon(gb_clipper_ref_t clipper, tb_size_t mode, gb_polygon_ref_t polygon);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
