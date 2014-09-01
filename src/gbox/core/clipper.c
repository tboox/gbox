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
 * @file        clipper.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "clipper"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "clipper.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the clipper impl type
typedef struct __gb_clipper_impl_t
{


}gb_clipper_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

gb_clipper_ref_t gb_clipper_init()
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_clipper_exit(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
}
tb_size_t gb_clipper_size(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
    return 0;
}
gb_clipper_shape_ref_t gb_clipper_shape(gb_clipper_ref_t clipper, tb_size_t index)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_clipper_clear(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_copy(gb_clipper_ref_t clipper, gb_clipper_ref_t copied)
{
    tb_trace_noimpl();
}
gb_matrix_ref_t gb_clipper_matrix(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_clipper_matrix_set(gb_clipper_ref_t clipper, gb_matrix_ref_t matrix)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_path(gb_clipper_ref_t clipper, tb_size_t mode, gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_triangle(gb_clipper_ref_t clipper, tb_size_t mode, gb_triangle_ref_t triangle)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_triangle2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_make(x0, y0, x1, y1, x2, y2);

    // add triangle
    gb_clipper_add_triangle(clipper, mode, &triangle);
}
tb_void_t gb_clipper_add_triangle2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_imake(x0, y0, x1, y1, x2, y2);

    // add triangle
    gb_clipper_add_triangle(clipper, mode, &triangle);
}
tb_void_t gb_clipper_add_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_rect_ref_t rect)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_rect2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_make(x, y, w, h);

    // add rect
    gb_clipper_add_rect(clipper, mode, &rect);
}
tb_void_t gb_clipper_add_rect2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_imake(x, y, w, h);

    // add rect
    gb_clipper_add_rect(clipper, mode, &rect);
}
tb_void_t gb_clipper_add_circle(gb_clipper_ref_t clipper, tb_size_t mode, gb_circle_ref_t circle)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_circle2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_make(x0, y0, r);

    // add circle
    gb_clipper_add_circle(clipper, mode, &circle);
}
tb_void_t gb_clipper_add_circle2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_imake(x0, y0, r);

    // add circle
    gb_clipper_add_circle(clipper, mode, &circle);
}
tb_void_t gb_clipper_add_ellipse(gb_clipper_ref_t clipper, tb_size_t mode, gb_ellipse_ref_t ellipse)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_ellipse2(gb_clipper_ref_t clipper, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_make(x0, y0, rx, ry);

    // add ellipse
    gb_clipper_add_ellipse(clipper, mode, &ellipse);
}
tb_void_t gb_clipper_add_ellipse2i(gb_clipper_ref_t clipper, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_imake(x0, y0, rx, ry);

    // add ellipse
    gb_clipper_add_ellipse(clipper, mode, &ellipse);
}

