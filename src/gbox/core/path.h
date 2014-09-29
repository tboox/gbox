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
 * @file        path.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PATH_H
#define GB_CORE_PATH_H

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

/// the path code enum
typedef enum __gb_path_code_e
{
    GB_PATH_CODE_MOVE       = 0 //!< the move-to code
,   GB_PATH_CODE_LINE       = 1 //!< the line-to code
,   GB_PATH_CODE_QUAD       = 2 //!< the quad-to code
,   GB_PATH_CODE_CUBE       = 3 //!< the cube-to code
,   GB_PATH_CODE_CLOS       = 4 //!< the clos code

}gb_path_code_e;

/// the path item for the iterator
typedef struct __gb_path_item_t
{
    /// the code
    tb_size_t       code;

    /// the point
    gb_point_t      point;

    /// the ctrls
    gb_point_t      ctrls[2];

}gb_path_item_t, *gb_path_item_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init path
 *
 * @return          the path
 */
gb_path_ref_t       gb_path_init(tb_noarg_t);

/*! exit path
 *
 * @param path      the path
 */
tb_void_t           gb_path_exit(gb_path_ref_t path);

/*! clear path
 *
 * @param path      the path
 */
tb_void_t           gb_path_clear(gb_path_ref_t path);

/*! copy path
 *
 * @param path      the path
 * @param copied    the copied path
 */
tb_void_t           gb_path_copy(gb_path_ref_t path, gb_path_ref_t copied);

/*! is null path?
 *
 * @param path      the path
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_path_null(gb_path_ref_t path);

/*! the path bounds
 *
 * @param path      the path
 *
 * @return          the bounds
 */
gb_rect_ref_t       gb_path_bounds(gb_path_ref_t path);

/*! set to be convex path, convex path may draw faster
 *
 * @param path      the path
 * @param convex    is convex?
 *
 */
tb_void_t           gb_path_set_convex(gb_path_ref_t path, tb_bool_t convex);

/*! get the last point
 *
 * @param path      the path
 * @param point     the last point
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_path_last(gb_path_ref_t path, gb_point_ref_t point);

/*! the path hint shape
 *
 * @param path      the path
 *
 * @return          the hint shape 
 */
gb_shape_ref_t      gb_path_hint(gb_path_ref_t path);

/*! the path polygon 
 *
 * @param path      the path
 *
 * @return          the polygon
 */
gb_polygon_ref_t    gb_path_polygon(gb_path_ref_t path);

/*! close the current contour
 *
 * if the current point is not equal to the first point of the contour
 * patch a line segment automatically
 *
 * @param path      the path
 */
tb_void_t           gb_path_clos(gb_path_ref_t path);

/*! move to the point
 *
 * @param path      the path
 * @param point     the point
 */
tb_void_t           gb_path_move_to(gb_path_ref_t path, gb_point_ref_t point);

/*! move to the point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_move2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y);

/*! move to the integer point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_move2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y);

/*! line to the point
 *
 * @param path      the path
 * @param point     the point
 */
tb_void_t           gb_path_line_to(gb_path_ref_t path, gb_point_ref_t point);

/*! line to the point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_line2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y);

/*! line to the integer point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_line2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y);

/*! quad to the ctrl, point
 *
 * @param path      the path
 * @param ctrl      the control point
 * @param point     the point
 */
tb_void_t           gb_path_quad_to(gb_path_ref_t path, gb_point_ref_t ctrl, gb_point_ref_t point);

/*! quad to the ctrl(cx, cy), point(x, y)
 *
 * @param path      the path
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_quad2_to(gb_path_ref_t path, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y);

/*! quad to the integer ctrl(cx, cy), point(x, y)
 *
 * @param path      the path
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_quad2i_to(gb_path_ref_t path, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y);

/*! cube to the ctrl0, ctrl1, point
 *
 * @param path      the path
 * @param ctrl0     the control0 point
 * @param ctrl1     the control1 point
 * @param point     the point
 */
tb_void_t           gb_path_cube_to(gb_path_ref_t path, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point);

/*! cube to the ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param path      the path
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_cube2_to(gb_path_ref_t path, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y);

/*! cube to the integer ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param path      the path
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_cube2i_to(gb_path_ref_t path, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y);

/*! arc to the arc
 *
 * @param path      the path
 * @param arc       the arc
 */
tb_void_t           gb_path_arc_to(gb_path_ref_t path, gb_arc_ref_t arc);

/*! arc to the arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_arc2_to(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! arc to the integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_arc2i_to(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! add path
 *
 * @param path      the path
 * @param added     the added path
 */
tb_void_t           gb_path_add_path(gb_path_ref_t path, gb_path_ref_t added);

/*! add line
 *
 * @param path      the path
 * @param line      the line
 */
tb_void_t           gb_path_add_line(gb_path_ref_t path, gb_line_ref_t line);

/*! add line(x0, y0, x1, y1)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_path_add_line2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1);

/*! add integer line(x0, y0, x1, y1)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_path_add_line2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1);

/*! add arc
 *
 * @param path      the path
 * @param arc       the arc
 */
tb_void_t           gb_path_add_arc(gb_path_ref_t path, gb_arc_ref_t arc);

/*! add arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_add_arc2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! add integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_add_arc2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! add triangle
 *
 * @param path      the path
 * @param triangle  the triangle
 */
tb_void_t           gb_path_add_triangle(gb_path_ref_t path, gb_triangle_ref_t triangle);

/*! add triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_path_add_triangle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! add integer triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_path_add_triangle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! add rect
 *
 * @param path      the path
 * @param rect      the rect
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect(gb_path_ref_t path, gb_rect_ref_t rect, tb_size_t direction);

/*! add rect(x, y, w, h)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect2(gb_path_ref_t path, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h, tb_size_t direction);

/*! add integer rect(x, y, w, h)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect2i(gb_path_ref_t path, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h, tb_size_t direction);

/*! add round rect
 *
 * @param path      the path
 * @param rect      the rect
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect(gb_path_ref_t path, gb_round_rect_ref_t rect, tb_size_t direction);

/*! add round rect(x, y, w, h)
 *
 * @param path      the path
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect2(gb_path_ref_t path, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry, tb_size_t direction);

/*! add integer round rect(x, y, w, h)
 *
 * @param path      the path
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect2i(gb_path_ref_t path, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry, tb_size_t direction);

/*! add circle
 *
 * @param path      the path
 * @param circle    the circle
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle(gb_path_ref_t path, gb_circle_ref_t circle, tb_size_t direction);

/*! add circle(x0, y0, r)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t r, tb_size_t direction);

/*! add integer circle(x0, y0, r)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t r, tb_size_t direction);

/*! add ellipse
 *
 * @param path      the path
 * @param ellipse   the ellipse
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse(gb_path_ref_t path, gb_ellipse_ref_t ellipse, tb_size_t direction);

/*! add ellipse(x0, y0, rx, ry)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, tb_size_t direction);

/*! add integer ellipse(x0, y0, rx, ry)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t direction);

#ifdef __gb_debug__
/*! dump path
 *
 * @param path      the path
 */
tb_void_t           gb_path_dump(gb_path_ref_t path);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
