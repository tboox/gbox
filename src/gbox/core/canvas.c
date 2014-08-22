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
 * @file        canvas.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "canvas"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "canvas.h"
#include "device.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the canvas impl type
typedef struct __gb_canvas_impl_t
{
    // the device
    gb_device_ref_t         device;

}gb_canvas_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_canvas_ref_t gb_canvas_init(gb_device_ref_t device)
{
    // done
    tb_bool_t           ok = tb_false;
    gb_canvas_impl_t*   impl = tb_null;
    do
    {
        // make canvas
        impl = tb_malloc0_type(gb_canvas_impl_t);
        tb_assert_and_check_break(impl);

        // init canvas 
        impl->device = device;

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_canvas_exit((gb_canvas_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_canvas_ref_t)impl;
}
gb_canvas_ref_t gb_canvas_init_from_window(gb_window_ref_t window)
{
    // done
    gb_canvas_ref_t canvas = tb_null;
    gb_device_ref_t device = tb_null;
    do
    {
        // init device 
        device = gb_device_init(window);
        tb_assert_and_check_break(device);

        // init canvas 
        canvas = gb_canvas_init(device);

    } while (0);

    // failed?
    if (!canvas)
    {
        // exit device
        if (device) gb_device_exit(device);
        device = tb_null;
    }

    // ok?
    return canvas;
}
#ifdef GB_CONFIG_THIRD_HAVE_SKIA
gb_canvas_ref_t gb_canvas_init_from_skia(gb_bitmap_ref_t bitmap)
{
    // done
    gb_canvas_ref_t canvas = tb_null;
    gb_device_ref_t device = tb_null;
    do
    {
        // init device 
        device = gb_device_init_skia(bitmap);
        tb_assert_and_check_break(device);

        // init canvas 
        canvas = gb_canvas_init(device);

    } while (0);

    // failed?
    if (!canvas)
    {
        // exit device
        if (device) gb_device_exit(device);
        device = tb_null;
    }

    // ok?
    return canvas;
}
#endif
gb_canvas_ref_t gb_canvas_init_from_bitmap(gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(bitmap, tb_null);

    // done
    gb_canvas_ref_t canvas = tb_null;
    gb_device_ref_t device = tb_null;
    do
    {
        // init device 
#if 0//def GB_CONFIG_THIRD_HAVE_SKIA
        device = gb_device_init_skia(bitmap);
#else
        device = gb_device_init_bitmap(bitmap);
#endif
        tb_assert_and_check_break(device);

        // init canvas 
        canvas = gb_canvas_init(device);

    } while (0);

    // failed?
    if (!canvas)
    {
        // exit device
        if (device) gb_device_exit(device);
        device = tb_null;
    }

    // ok?
    return canvas;
}
tb_void_t gb_canvas_exit(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl);

    // exit device
    if (impl->device) gb_device_exit(impl->device);
    impl->device = tb_null;

    // exit it
    tb_free(canvas);
}
tb_size_t gb_canvas_pixfmt(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->device, GB_PIXFMT_NONE);

    // the pixfmt
    return gb_device_pixfmt(impl->device);
}
gb_device_ref_t gb_canvas_device(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl, tb_null);

    // the device
    return impl->device;
}
gb_paint_ref_t gb_canvas_paint(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
gb_path_ref_t gb_canvas_path(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
gb_matrix_ref_t gb_canvas_matrix(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
gb_clipper_ref_t gb_canvas_clipper(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
gb_path_ref_t gb_canvas_save_path(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_canvas_load_path(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
gb_paint_ref_t gb_canvas_save_paint(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_canvas_load_paint(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
gb_matrix_ref_t gb_canvas_save_matrix(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_canvas_load_matrix(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
gb_clipper_ref_t gb_canvas_save_clipper(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_canvas_load_clipper(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clear_path(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clear_paint(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clear_matrix(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clear_clipper(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_mode_set(gb_canvas_ref_t canvas, tb_size_t mode)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_flag_set(gb_canvas_ref_t canvas, tb_size_t flag)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_color_set(gb_canvas_ref_t canvas, gb_color_t color)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_alpha_set(gb_canvas_ref_t canvas, tb_byte_t alpha)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_width_set(gb_canvas_ref_t canvas, gb_float_t width)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_cap_set(gb_canvas_ref_t canvas, tb_size_t cap)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_join_set(gb_canvas_ref_t canvas, tb_size_t join)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_shader_set(gb_canvas_ref_t canvas, gb_shader_ref_t shader)
{
    tb_trace_noimpl();
}
tb_bool_t gb_canvas_rotate(gb_canvas_ref_t canvas, gb_float_t degrees)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_rotate_lhs(gb_canvas_ref_t canvas, gb_float_t degrees)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_rotatep(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_rotatep_lhs(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_scale(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_scale_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_scalep(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_scalep_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_skew(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_skew_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_skewp(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_skewp_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_sincos(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_sincos_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_sincosp(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_sincosp_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_translate(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_translate_lhs(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_multiply(gb_canvas_ref_t canvas, gb_matrix_ref_t factor)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_canvas_multiply_lhs(gb_canvas_ref_t canvas, gb_matrix_ref_t factor)
{
    tb_trace_noimpl();
    return tb_false;
}
tb_void_t gb_canvas_clos(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_move_to(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_move2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_move2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_line_to(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_line2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_line2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_quad_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_quad2_to(gb_canvas_ref_t canvas, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_quad2i_to(gb_canvas_ref_t canvas, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_cube_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_cube2_to(gb_canvas_ref_t canvas, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_cube2i_to(gb_canvas_ref_t canvas, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_arc_to(gb_canvas_ref_t canvas, gb_arc_ref_t arc)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_arc2_to(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_arc2i_to(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t ab, tb_size_t an)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_path(gb_canvas_ref_t canvas, tb_size_t mode, gb_paint_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_triangle(gb_canvas_ref_t canvas, tb_size_t mode, gb_triangle_ref_t triangle)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_triangle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_triangle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_rect(gb_canvas_ref_t canvas, tb_size_t mode, gb_rect_ref_t rect)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_rect2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_rect2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_circle(gb_canvas_ref_t canvas, tb_size_t mode, gb_circle_ref_t circle)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_circle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_circle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_ellipse(gb_canvas_ref_t canvas, tb_size_t mode, gb_ellipse_ref_t ellipse)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_ellipse2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_clip_ellipse2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_clear(gb_canvas_ref_t canvas, gb_color_t color)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device);

    // clear it
    gb_device_draw_clear(impl->device, color);
}
tb_void_t gb_canvas_draw(gb_canvas_ref_t canvas)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_path(gb_canvas_ref_t canvas, gb_paint_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_point(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_point2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_point2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_line(gb_canvas_ref_t canvas, gb_line_ref_t line)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_line2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_line2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_arc(gb_canvas_ref_t canvas, gb_arc_ref_t arc)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_arc2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_arc2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t ab, tb_size_t an)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_triangle(gb_canvas_ref_t canvas, gb_triangle_ref_t triangle)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_triangle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_triangle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_rect(gb_canvas_ref_t canvas, gb_rect_ref_t rect)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_rect2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_rect2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_circle(gb_canvas_ref_t canvas, gb_circle_ref_t circle)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_circle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_circle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_ellipse(gb_canvas_ref_t canvas, gb_ellipse_ref_t ellipse)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_ellipse2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    tb_trace_noimpl();
}
tb_void_t gb_canvas_draw_ellipse2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    tb_trace_noimpl();
}
