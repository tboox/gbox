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
 * @file        device.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "device.h"
#include "device/prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#ifdef GB_CONFIG_THIRD_HAVE_GL
__tb_extern_c__ gb_device_ref_t gb_device_init_gl(gb_window_ref_t window);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init(gb_window_ref_t window)
{
    // check
    tb_assert_and_check_return_val(window, tb_null);

    // the window mode
    tb_size_t mode = gb_window_mode(window);
    tb_assert_and_check_return_val(mode, tb_null);

    // done
    gb_device_ref_t device = tb_null;
    switch (mode)
    {
#ifdef GB_CONFIG_THIRD_HAVE_GL
    case GB_WINDOW_MODE_GL:
        device = gb_device_init_gl(window);
        break;
#endif
    case GB_WINDOW_MODE_BITMAP:
        device = gb_device_init_bitmap(gb_window_bitmap(window));
        break;
    default:
        // trace
        tb_trace_e("unknown window mode: %lu", mode);
        break;
    }

    // ok?
    return device;
}
tb_void_t gb_device_exit(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // exit it
    if (impl->exit) impl->exit(device);
}
tb_size_t gb_device_type(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_DEVICE_TYPE_NONE);

    // the type
    return impl->type;
}
tb_size_t gb_device_pixfmt(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
    return impl->pixfmt;
}
tb_size_t gb_device_width(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, 0);

    // the width
    return impl->width;
}
tb_size_t gb_device_height(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, 0);

    // the height
    return impl->height;
}
tb_void_t gb_device_resize(gb_device_ref_t device, tb_size_t width, tb_size_t height)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->resize);

    // resize
    impl->resize(device, width, height);
}
tb_void_t gb_device_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_clear);

    // clear it
    impl->draw_clear(device, color);
}
tb_void_t gb_device_draw_path(gb_device_ref_t device, gb_path_ref_t path, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_path);

    // draw path
    impl->draw_path(device, path, matrix, paint, clipper);
}
tb_void_t gb_device_draw_point(gb_device_ref_t device, gb_point_ref_t point, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_point);

    // draw point
    impl->draw_point(device, point, matrix, paint, clipper);
}
tb_void_t gb_device_draw_line(gb_device_ref_t device, gb_line_ref_t line, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_line);

    // draw line
    impl->draw_line(device, line, matrix, paint, clipper);
}
tb_void_t gb_device_draw_arc(gb_device_ref_t device, gb_arc_ref_t arc, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_arc);

    // draw arc
    impl->draw_arc(device, arc, matrix, paint, clipper);
}
tb_void_t gb_device_draw_triangle(gb_device_ref_t device, gb_triangle_ref_t triangle, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_triangle);

    // draw triangle
    impl->draw_triangle(device, triangle, matrix, paint, clipper);
}
tb_void_t gb_device_draw_rect(gb_device_ref_t device, gb_rect_ref_t rect, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_rect);

    // draw rect
    impl->draw_rect(device, rect, matrix, paint, clipper);
}
tb_void_t gb_device_draw_circle(gb_device_ref_t device, gb_circle_ref_t circle, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_circle);

    // draw circle
    impl->draw_circle(device, circle, matrix, paint, clipper);
}
tb_void_t gb_device_draw_ellipse(gb_device_ref_t device, gb_ellipse_ref_t ellipse, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_ellipse);

    // draw ellipse
    impl->draw_ellipse(device, ellipse, matrix, paint, clipper);
}
tb_void_t gb_device_draw_polygon(gb_device_ref_t device, gb_polygon_ref_t polygon, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_polygon);

    // draw polygon
    impl->draw_polygon(device, polygon, matrix, paint, clipper);
}

