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
#include "path.h"
#include "paint.h"

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
#ifdef GB_CONFIG_THIRD_HAVE_SKIA
        device = gb_device_init_skia(gb_window_bitmap(window));
#else
        device = gb_device_init_bitmap(gb_window_bitmap(window));
#endif
        break;
    default:
        // trace
        tb_trace_e("unknown window mode: %lu", mode);
        break;
    }

    // init
    tb_bool_t ok = tb_false;
    do
    {
        // check
        gb_device_impl_t* impl = (gb_device_impl_t*)device;
        tb_assert_and_check_break(impl);

        // init pixfmt
        impl->pixfmt           = (tb_uint16_t)gb_window_pixfmt(window); 

        // init width and height
        impl->width            = (tb_uint16_t)gb_window_width(window); 
        impl->height           = (tb_uint16_t)gb_window_height(window); 

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (device) gb_device_exit(device);
        device = tb_null;
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
    if (impl->exit) impl->exit(impl);
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
    impl->resize(impl, width, height);
}
tb_void_t gb_device_bind_paint(gb_device_ref_t device, gb_paint_ref_t paint)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // bind it
    impl->paint = paint;
}
tb_void_t gb_device_bind_matrix(gb_device_ref_t device, gb_matrix_ref_t matrix)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // bind it
    impl->matrix = matrix;
}
tb_void_t gb_device_bind_clipper(gb_device_ref_t device, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // bind it
    impl->clipper = clipper;
}
tb_void_t gb_device_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_clear);

    // clear it
    impl->draw_clear(impl, color);
}
tb_void_t gb_device_draw_lines(gb_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_lines);

    // draw lines
    impl->draw_lines(impl, points, count, bounds);
}
tb_void_t gb_device_draw_points(gb_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_points);

    // draw points
    impl->draw_points(impl, points, count, bounds);
}
tb_void_t gb_device_draw_polygon(gb_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_polygon);

    // draw polygon
    impl->draw_polygon(impl, polygon, hint, bounds);
}

