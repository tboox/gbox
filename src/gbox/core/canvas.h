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
 * @file        canvas.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_CANVAS_H
#define GB_CORE_CANVAS_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../platform/window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init canvas from the given device
 *
 * @param device    the device
 *
 * @return          the canvas
 */
gb_canvas_ref_t     gb_canvas_init(gb_device_ref_t device);

/*! init canvas from the given window
 *
 * @param window    the window
 *
 * @return          the canvas
 */
gb_canvas_ref_t     gb_canvas_init_from_window(gb_window_ref_t window);

#ifdef GB_CONFIG_THIRD_HAVE_SKIA
/*! init canvas from skia
 *
 * @param bitmap    the bitmap
 *
 * @return          the canvas
 */
gb_canvas_ref_t     gb_canvas_init_from_skia(gb_bitmap_ref_t bitmap);
#endif

/*! init canvas from the given bitmap
 *
 * @param bitmap    the bitmap
 *
 * @return          the canvas
 */
gb_canvas_ref_t     gb_canvas_init_from_bitmap(gb_bitmap_ref_t bitmap);

/*! exit canvas
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_exit(gb_canvas_ref_t canvas);

/*! the pixfmt 
 *
 * @param canvas    the canvas
 *
 * @return          the pixfmt
 */
tb_size_t           gb_canvas_pixfmt(gb_canvas_ref_t canvas);

/*! the canvas width
 *
 * @param canvas    the canvas
 *
 * @return          the width
 */
tb_size_t           gb_canvas_stroke_width(gb_canvas_ref_t canvas);

/*! the canvas height
 *
 * @param canvas    the canvas
 *
 * @return          the height
 */
tb_size_t           gb_canvas_height(gb_canvas_ref_t canvas);

/*! get the device 
 *
 * @param canvas    the canvas
 *
 * @return          the device
 */
gb_device_ref_t     gb_canvas_device(gb_canvas_ref_t canvas);

/*! get the paint 
 *
 * @param canvas    the canvas
 *
 * @return          the paint
 */
gb_paint_ref_t      gb_canvas_paint(gb_canvas_ref_t canvas);

/*! get the path 
 *
 * @param canvas    the canvas
 *
 * @return          the path
 */
gb_path_ref_t       gb_canvas_path(gb_canvas_ref_t canvas);

/*! get the matrix 
 *
 * @param canvas    the canvas
 *
 * @return          the matrix
 */
gb_matrix_ref_t     gb_canvas_matrix(gb_canvas_ref_t canvas);

/*! get the clipper 
 *
 * @param canvas    the canvas
 *
 * @return          the clipper
 */
gb_clipper_ref_t    gb_canvas_clipper(gb_canvas_ref_t canvas);

/*! save path 
 *
 * @param canvas    the canvas
 *
 * @return          the current path
 */
gb_path_ref_t       gb_canvas_save_path(gb_canvas_ref_t canvas);

/*! load path 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_load_path(gb_canvas_ref_t canvas);

/*! save paint 
 *
 * @param canvas    the canvas
 *
 * @return          the current paint
 */
gb_paint_ref_t      gb_canvas_save_paint(gb_canvas_ref_t canvas);

/*! load paint 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_load_paint(gb_canvas_ref_t canvas);

/*! save matrix 
 *
 * @param canvas    the canvas
 *
 * @return          the current matrix
 */
gb_matrix_ref_t     gb_canvas_save_matrix(gb_canvas_ref_t canvas);

/*! load matrix 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_load_matrix(gb_canvas_ref_t canvas);

/*! save clipper 
 *
 * @param canvas    the canvas
 *
 * @return          the current clipper
 */
gb_clipper_ref_t    gb_canvas_save_clipper(gb_canvas_ref_t canvas);

/*! load clipper 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_load_clipper(gb_canvas_ref_t canvas);

/*! clear path 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_clear_path(gb_canvas_ref_t canvas);

/*! clear paint 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_clear_paint(gb_canvas_ref_t canvas);

/*! clear matrix 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_clear_matrix(gb_canvas_ref_t canvas);

/*! clear clipper 
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_clear_clipper(gb_canvas_ref_t canvas);

/*! set the paint mode 
 *
 * @param canvas    the canvas
 * @param mode      the paint mode
 */
tb_void_t           gb_canvas_mode_set(gb_canvas_ref_t canvas, tb_size_t mode);

/*! set the paint flag 
 *
 * @param canvas    the canvas
 * @param mode      the paint flag
 */
tb_void_t           gb_canvas_flag_set(gb_canvas_ref_t canvas, tb_size_t flag);

/*! set the paint color 
 *
 * @param canvas    the canvas
 * @param mode      the paint color
 */
tb_void_t           gb_canvas_color_set(gb_canvas_ref_t canvas, gb_color_t color);

/*! set the paint alpha 
 *
 * @param canvas    the canvas
 * @param mode      the paint alpha
 */
tb_void_t           gb_canvas_alpha_set(gb_canvas_ref_t canvas, tb_byte_t alpha);

/*! set the paint width 
 *
 * @param canvas    the canvas
 * @param mode      the paint width
 */
tb_void_t           gb_canvas_stroke_width_set(gb_canvas_ref_t canvas, gb_float_t width);

/*! set the paint cap 
 *
 * @param canvas    the canvas
 * @param mode      the paint cap
 */
tb_void_t           gb_canvas_stroke_cap_set(gb_canvas_ref_t canvas, tb_size_t cap);

/*! set the paint join 
 *
 * @param canvas    the canvas
 * @param mode      the paint join
 */
tb_void_t           gb_canvas_stroke_join_set(gb_canvas_ref_t canvas, tb_size_t join);

/*! set the paint miter limit
 *
 * @param paint     the paint 
 * @param miter     the miter limit
 */
tb_void_t           gb_cavas_stroke_miter_set(gb_canvas_ref_t canvas, gb_float_t miter);

/*! set the paint fill rule
 *
 * @param paint     the paint 
 * @param rule      the paint fill rule
 */
tb_void_t           gb_canvas_fill_rule_set(gb_canvas_ref_t canvas, tb_size_t rule);

/*! set the paint shader 
 *
 * @param canvas    the canvas
 * @param mode      the paint shader
 */
tb_void_t           gb_canvas_shader_set(gb_canvas_ref_t canvas, gb_shader_ref_t shader);

/*! transform matrix with the given rotate degrees
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param degrees   the rotate degrees
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_rotate(gb_canvas_ref_t canvas, gb_float_t degrees);

/*! transform matrix with the given rotate degrees
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param degrees   the rotate degrees
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_rotate_lhs(gb_canvas_ref_t canvas, gb_float_t degrees);

/*! transform matrix with the given rotate degrees by the coordinate: (px, py) 
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param degrees   the rotate degrees
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_rotatep(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py);

/*! transform matrix with the given rotate degrees by the coordinate: (px, py) 
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param degrees   the rotate degrees
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_rotatep_lhs(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py);

/*! transform matrix with the given scale value
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sx        the x-scale
 * @param sy        the y-scale
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_scale(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy);

/*! transform matrix with the given scale value
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sx        the x-scale
 * @param sy        the y-scale
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_scale_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy);

/*! transform matrix with the given scale value by the coordinate: (px, py) 
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sx        the x-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_scalep(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py);

/*! transform matrix with the given scale value by the coordinate: (px, py) 
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sy        the y-scale
 * @param sy        the y-scale
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_scalep_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py);

/*! transform matrix with the given skew value
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sk        the x-skew
 * @param sk        the y-skew
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_skew(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky);

/*! transform matrix with the given skew value
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sk        the x-skew
 * @param sk        the y-skew
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_skew_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky);

/*! transform matrix with the given skew value by the coordinate: (px, py)
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sk        the x-skew
 * @param sk        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_skewp(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py);

/*! transform matrix with the given skew value by the coordinate: (px, py)
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sk        the x-skew
 * @param sk        the y-skew
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_skewp_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py);

/*! transform matrix with the given sin and cos value
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sin       the sin value
 * @param cos       the cos value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_sincos(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos);

/*! transform matrix with the given sin and cos value
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sin       the sin value
 * @param cos       the cos value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_sincos_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos);

/*! transform matrix with the given sin and cos value by the coordinate: (px, py)
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_sincosp(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py);

/*! transform matrix with the given sin and cos value by the coordinate: (px, py)
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param sin       the sin value
 * @param cos       the cos value
 * @param px        the x-coordinate
 * @param py        the y-coordinate
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_sincosp_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py);

/*! transform matrix with the given translate value
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param dx        the x-translate value
 * @param dy        the y-translate value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_translate(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy);

/*! transform matrix with the given translate value
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param dx        the x-translate value
 * @param dy        the y-translate value
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_translate_lhs(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy);

/*! multiply matrix with the given translate factor
 *
 * matrix = matrix * factor
 *
 * @param canvas    the canvas
 * @param factor    the factor
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_multiply(gb_canvas_ref_t canvas, gb_matrix_ref_t factor);

/*! multiply matrix with the given translate factor
 *
 * matrix = factor * matrix
 *
 * @param canvas    the canvas
 * @param factor    the factor
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_canvas_multiply_lhs(gb_canvas_ref_t canvas, gb_matrix_ref_t factor);

/*! set the convex path manually for drawing faster
 *
 * @param canvas    the canvas
 * @param convex    is convex?
 */
tb_void_t           gb_canvas_convex_set(gb_canvas_ref_t canvas, tb_bool_t convex);

/*! close path
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_clos(gb_canvas_ref_t canvas);

/*! move to the point
 *
 * @param canvas    the canvas
 * @param point     the point
 */
tb_void_t           gb_canvas_move_to(gb_canvas_ref_t canvas, gb_point_ref_t point);

/*! move to the point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_move2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y);

/*! move to the integer point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_move2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y);

/*! line to the point
 *
 * @param canvas    the canvas
 * @param point     the point
 */
tb_void_t           gb_canvas_line_to(gb_canvas_ref_t canvas, gb_point_ref_t point);

/*! line to the point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_line2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y);

/*! line to the integer point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_line2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y);

/*! quad to the ctrl, point
 *
 * @param canvas    the canvas
 * @param ctrl      the control point
 * @param point     the point
 */
tb_void_t           gb_canvas_quad_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl, gb_point_ref_t point);

/*! quad to the ctrl(cx, cy), point(x, y)
 *
 * @param canvas    the canvas
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_quad2_to(gb_canvas_ref_t canvas, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y);

/*! quad to the integer ctrl(cx, cy), point(x, y)
 *
 * @param canvas    the canvas
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_quad2i_to(gb_canvas_ref_t canvas, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y);

/*! cubic to the ctrl0, ctrl1, point
 *
 * @param canvas    the canvas
 * @param ctrl0     the control0 point
 * @param ctrl1     the control1 point
 * @param point     the point
 */
tb_void_t           gb_canvas_cubic_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point);

/*! cubic to the ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param canvas    the canvas
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_cubic2_to(gb_canvas_ref_t canvas, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y);

/*! cubic to the integer ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param canvas    the canvas
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_cubic2i_to(gb_canvas_ref_t canvas, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y);

/*! arc to the arc
 *
 * @param canvas    the canvas
 * @param arc       the arc
 */
tb_void_t           gb_canvas_arc_to(gb_canvas_ref_t canvas, gb_arc_ref_t arc);

/*! arc to the arc(x0, y0, rx, ry, ab, an)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_canvas_arc2_to(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! arc to the integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_canvas_arc2i_to(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! clip path
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param path      the clip path
 */
tb_void_t           gb_canvas_clip_path(gb_canvas_ref_t canvas, tb_size_t mode, gb_path_ref_t path);

/*! clip triangle
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param triangle  the clip triangle
 */
tb_void_t           gb_canvas_clip_triangle(gb_canvas_ref_t canvas, tb_size_t mode, gb_triangle_ref_t triangle);

/*! clip triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_canvas_clip_triangle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! clip integer triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_canvas_clip_triangle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! clip rect
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param rect      the clip rect
 */
tb_void_t           gb_canvas_clip_rect(gb_canvas_ref_t canvas, tb_size_t mode, gb_rect_ref_t rect);

/*! clip rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 */
tb_void_t           gb_canvas_clip_rect2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h);

/*! clip integer rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 */
tb_void_t           gb_canvas_clip_rect2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h);

/*! clip round rect
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param rect      the round rect
 */
tb_void_t           gb_canvas_clip_round_rect(gb_canvas_ref_t canvas, tb_size_t mode, gb_round_rect_ref_t rect);

/*! clip round rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_clip_round_rect2(gb_canvas_ref_t canvas, tb_size_t mode, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry);

/*! clip integer round rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_clip_round_rect2i(gb_canvas_ref_t canvas, tb_size_t mode, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry);

/*! clip circle
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param circle    the clip circle
 */
tb_void_t           gb_canvas_clip_circle(gb_canvas_ref_t canvas, tb_size_t mode, gb_circle_ref_t circle);

/*! clip circle(x0, y0, r)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 */
tb_void_t           gb_canvas_clip_circle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! clip integer circle(x0, y0, r)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 */
tb_void_t           gb_canvas_clip_circle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t r);

/*! clip ellipse
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param ellipse   the clip ellipse
 */
tb_void_t           gb_canvas_clip_ellipse(gb_canvas_ref_t canvas, tb_size_t mode, gb_ellipse_ref_t ellipse);

/*! clip ellipse(x0, y0, rx, ry)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_clip_ellipse2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry);

/*! clip integer ellipse(x0, y0, rx, ry)
 *
 * @param canvas    the canvas
 * @param mode      the clip mode
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_clip_ellipse2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry);

/*! clear draw and fill the given color
 *
 * @param canvas    the canvas
 * @param color     the color
 */
tb_void_t           gb_canvas_draw_clear(gb_canvas_ref_t canvas, gb_color_t color);

/*! draw the current path
 *
 * @param canvas    the canvas
 */
tb_void_t           gb_canvas_draw(gb_canvas_ref_t canvas);

/*! draw the given path
 *
 * @param canvas    the canvas
 * @param path      the path
 */
tb_void_t           gb_canvas_draw_path(gb_canvas_ref_t canvas, gb_path_ref_t path);

/*! draw point
 *
 * @param canvas    the canvas
 * @param point     the point
 */
tb_void_t           gb_canvas_draw_point(gb_canvas_ref_t canvas, gb_point_ref_t point);

/*! draw point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_draw_point2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y);

/*! draw integer point(x, y)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_canvas_draw_point2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y);

/*! draw line
 *
 * @param canvas    the canvas
 * @param line      the line
 */
tb_void_t           gb_canvas_draw_line(gb_canvas_ref_t canvas, gb_line_ref_t line);

/*! draw line(x0, y0, x1, y1)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_canvas_draw_line2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1);

/*! draw integer line(x0, y0, x1, y1)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_canvas_draw_line2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1);

/*! draw arc
 *
 * @param canvas    the canvas
 * @param arc       the arc
 */
tb_void_t           gb_canvas_draw_arc(gb_canvas_ref_t canvas, gb_arc_ref_t arc);

/*! draw arc(x0, y0, rx, ry, ab, an)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_canvas_draw_arc2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! draw integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_canvas_draw_arc2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! draw triangle
 *
 * @param canvas    the canvas
 * @param triangle  the triangle
 */
tb_void_t           gb_canvas_draw_triangle(gb_canvas_ref_t canvas, gb_triangle_ref_t triangle);

/*! draw triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_canvas_draw_triangle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! draw integer triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_canvas_draw_triangle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! draw rect
 *
 * @param canvas    the canvas
 * @param rect      the rect
 */
tb_void_t           gb_canvas_draw_rect(gb_canvas_ref_t canvas, gb_rect_ref_t rect);

/*! draw rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 */
tb_void_t           gb_canvas_draw_rect2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h);

/*! draw integer rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 */
tb_void_t           gb_canvas_draw_rect2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h);

/*! draw round rect
 *
 * @param canvas    the canvas
 * @param rect      the round rect
 */
tb_void_t           gb_canvas_draw_round_rect(gb_canvas_ref_t canvas, gb_round_rect_ref_t rect);

/*! draw round rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_draw_round_rect2(gb_canvas_ref_t canvas, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry);

/*! draw integer round rect(x, y, w, h)
 *
 * @param canvas    the canvas
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_draw_round_rect2i(gb_canvas_ref_t canvas, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry);

/*! draw circle
 *
 * @param canvas    the canvas
 * @param circle    the circle
 */
tb_void_t           gb_canvas_draw_circle(gb_canvas_ref_t canvas, gb_circle_ref_t circle);

/*! draw circle(x0, y0, r)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 */
tb_void_t           gb_canvas_draw_circle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! draw integer circle(x0, y0, r)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 */
tb_void_t           gb_canvas_draw_circle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t r);

/*! draw ellipse
 *
 * @param canvas    the canvas
 * @param ellipse   the ellipse
 */
tb_void_t           gb_canvas_draw_ellipse(gb_canvas_ref_t canvas, gb_ellipse_ref_t ellipse);

/*! draw ellipse(x0, y0, rx, ry)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_draw_ellipse2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry);

/*! draw integer ellipse(x0, y0, rx, ry)
 *
 * @param canvas    the canvas
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_canvas_draw_ellipse2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry);

/*! draw polygon
 *
 * @param canvas    the canvas
 * @param polygon   the polygon
 */
tb_void_t           gb_canvas_draw_polygon(gb_canvas_ref_t canvas, gb_polygon_ref_t polygon);

/*! draw lines
 *
 * @param canvas    the canvas
 * @param points    the points
 * @param count     the points count
 */
tb_void_t           gb_canvas_draw_lines(gb_canvas_ref_t canvas, gb_point_ref_t points, tb_size_t count);

/*! draw points
 *
 * @param canvas    the canvas
 * @param points    the points
 * @param count     the points count
 */
tb_void_t           gb_canvas_draw_points(gb_canvas_ref_t canvas, gb_point_ref_t points, tb_size_t count);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
