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
 * @file        path.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "path"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "path.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_path_ref_t gb_path_init()
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_path_exit(gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_clear(gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_copy(gb_path_ref_t path, gb_path_ref_t copied)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_null(gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_clos(gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_move_to(gb_path_ref_t path, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_move2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y)
{
    // make point
    gb_point_t point = gb_point_make(x, y);

    // move-to
    gb_path_move_to(path, &point);
}
tb_void_t gb_path_move2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y)
{
    // make point
    gb_point_t point = gb_point_imake(x, y);

    // move-to
    gb_path_move_to(path, &point);
}
tb_void_t gb_path_line_to(gb_path_ref_t path, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_line2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y)
{
    // make point
    gb_point_t point = gb_point_make(x, y);

    // line-to
    gb_path_line_to(path, &point);
}
tb_void_t gb_path_line2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y)
{    
    // make point
    gb_point_t point = gb_point_imake(x, y);

    // line-to
    gb_path_line_to(path, &point);
}
tb_void_t gb_path_quad_to(gb_path_ref_t path, gb_point_ref_t ctrl, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_quad2_to(gb_path_ref_t path, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y)
{
    // make ctrl
    gb_point_t ctrl = gb_point_make(cx, cy);

    // make point
    gb_point_t point = gb_point_make(x, y);

    // quad-to
    gb_path_quad_to(path, &ctrl, &point);
}
tb_void_t gb_path_quad2i_to(gb_path_ref_t path, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y)
{
    // make ctrl
    gb_point_t ctrl = gb_point_imake(cx, cy);

    // make point
    gb_point_t point = gb_point_imake(x, y);

    // quad-to
    gb_path_quad_to(path, &ctrl, &point);
}
tb_void_t gb_path_cube_to(gb_path_ref_t path, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_cube2_to(gb_path_ref_t path, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y)
{
    // make ctrl0
    gb_point_t ctrl0 = gb_point_make(cx0, cy0);

    // make ctrl1
    gb_point_t ctrl1 = gb_point_make(cx1, cy1);

    // make point
    gb_point_t point = gb_point_make(x, y);

    // cube-to
    gb_path_cube_to(path, &ctrl0, &ctrl1, &point);
}
tb_void_t gb_path_cube2i_to(gb_path_ref_t path, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y)
{
    // make ctrl0
    gb_point_t ctrl0 = gb_point_imake(cx0, cy0);

    // make ctrl1
    gb_point_t ctrl1 = gb_point_imake(cx1, cy1);

    // make point
    gb_point_t point = gb_point_imake(x, y);

    // cube-to
    gb_path_cube_to(path, &ctrl0, &ctrl1, &point);
}
tb_void_t gb_path_arc_to(gb_path_ref_t path, gb_arc_ref_t arc)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_arc2_to(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_make(x0, y0, rx, ry, ab, an);

    // arc-to
    gb_path_arc_to(path, &arc);
}
tb_void_t gb_path_arc2i_to(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t ab, tb_size_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_imake(x0, y0, rx, ry, ab, an);

    // arc-to
    gb_path_arc_to(path, &arc);
}
tb_void_t gb_path_add_path(gb_path_ref_t path, gb_path_ref_t added)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_line(gb_path_ref_t path, gb_line_ref_t line)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_line2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1)
{
    // make line
    gb_line_t line = gb_line_make(x0, y0, x1, y1);

    // add line
    gb_path_add_line(path, &line);
}
tb_void_t gb_path_add_line2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1)
{
    // make line
    gb_line_t line = gb_line_imake(x0, y0, x1, y1);

    // add line
    gb_path_add_line(path, &line);
}
tb_void_t gb_path_add_arc(gb_path_ref_t path, gb_arc_ref_t arc)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_arc2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_make(x0, y0, rx, ry, ab, an);

    // add arc
    gb_path_add_arc(path, &arc);
}
tb_void_t gb_path_add_arc2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t ab, tb_size_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_imake(x0, y0, rx, ry, ab, an);

    // add arc
    gb_path_add_arc(path, &arc);
}
tb_void_t gb_path_add_triangle(gb_path_ref_t path, gb_triangle_ref_t triangle)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_triangle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_make(x0, y0, x1, y1, x2, y2);

    // add triangle
    gb_path_add_triangle(path, &triangle);
}
tb_void_t gb_path_add_triangle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_imake(x0, y0, x1, y1, x2, y2);

    // add triangle
    gb_path_add_triangle(path, &triangle);
}
tb_void_t gb_path_add_rect(gb_path_ref_t path, gb_rect_ref_t rect)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_rect2(gb_path_ref_t path, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_make(x, y, w, h);

    // add rect
    gb_path_add_rect(path, &rect);
}
tb_void_t gb_path_add_rect2i(gb_path_ref_t path, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_imake(x, y, w, h);

    // add rect
    gb_path_add_rect(path, &rect);
}
tb_void_t gb_path_add_circle(gb_path_ref_t path, gb_circle_ref_t circle)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_circle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_make(x0, y0, r);

    // add circle
    gb_path_add_circle(path, &circle);
}
tb_void_t gb_path_add_circle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_imake(x0, y0, r);

    // add circle
    gb_path_add_circle(path, &circle);
}
tb_void_t gb_path_add_ellipse(gb_path_ref_t path, gb_ellipse_ref_t ellipse)
{
    tb_trace_noimpl();
}
tb_void_t gb_path_add_ellipse2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_make(x0, y0, rx, ry);

    // add ellipse
    gb_path_add_ellipse(path, &ellipse);
}
tb_void_t gb_path_add_ellipse2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_imake(x0, y0, rx, ry);

    // add ellipse
    gb_path_add_ellipse(path, &ellipse);
}
#ifdef __gb_debug__
tb_void_t gb_path_dump(gb_path_ref_t path)
{
    tb_trace_noimpl();
}
#endif

