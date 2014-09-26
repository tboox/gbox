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
#include "matrix.h"
#include "path.h"
#include "paint.h"
#include "clipper.h"
#include "impl/bounds.h"
#include "impl/cache_stack.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the canvas impl type
typedef struct __gb_canvas_impl_t
{
    // the device
    gb_device_ref_t         device;

    // the matrix
    gb_matrix_t             matrix;

    // the matrix stack
    tb_stack_ref_t          matrix_stack;

    // the path stack
    gb_cache_stack_ref_t    path_stack;

    // the paint stack
    gb_cache_stack_ref_t    paint_stack;

    // the clipper stack
    gb_cache_stack_ref_t    clipper_stack;

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

        // init matrix
        gb_matrix_clear(&impl->matrix);

        // init matrix stack
        impl->matrix_stack = tb_stack_init(8, tb_item_func_mem(sizeof(gb_matrix_t), tb_null, tb_null));
        tb_assert_and_check_break(impl->matrix_stack);

        // init path stack
        impl->path_stack = gb_cache_stack_init(8, GB_CACHE_STACK_TYPE_PATH);
        tb_assert_and_check_break(impl->path_stack);

        // init paint stack
        impl->paint_stack = gb_cache_stack_init(8, GB_CACHE_STACK_TYPE_PAINT);
        tb_assert_and_check_break(impl->paint_stack);

        // init clipper stack
        impl->clipper_stack = gb_cache_stack_init(8, GB_CACHE_STACK_TYPE_CLIPPER);
        tb_assert_and_check_break(impl->clipper_stack);

        // bind matrix
        gb_device_bind_matrix(impl->device, &impl->matrix);

        // bind paint
        gb_device_bind_paint(impl->device, (gb_paint_ref_t)gb_cache_stack_object(impl->paint_stack));

        // bind clipper
        gb_device_bind_clipper(impl->device, (gb_clipper_ref_t)gb_cache_stack_object(impl->clipper_stack));

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
        device = gb_device_init_bitmap(bitmap);
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

    // exit clipper stack
    if (impl->clipper_stack) gb_cache_stack_exit(impl->clipper_stack);
    impl->clipper_stack = tb_null;

    // exit paint stack
    if (impl->paint_stack) gb_cache_stack_exit(impl->paint_stack);
    impl->paint_stack = tb_null;

    // exit path stack
    if (impl->path_stack) gb_cache_stack_exit(impl->path_stack);
    impl->path_stack = tb_null;

    // exit matrix stack
    if (impl->matrix_stack) tb_stack_exit(impl->matrix_stack);
    impl->matrix_stack = tb_null;

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
tb_size_t gb_canvas_width(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl, 0);

    // the width
    return gb_device_width(impl->device);
}
tb_size_t gb_canvas_height(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl, 0);

    // the height
    return gb_device_height(impl->device);
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
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->paint_stack, tb_null);

    // the paint
    return (gb_paint_ref_t)gb_cache_stack_object(impl->paint_stack);
}
gb_path_ref_t gb_canvas_path(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->path_stack, tb_null);

    // the path
    return (gb_path_ref_t)gb_cache_stack_object(impl->path_stack);
}
gb_matrix_ref_t gb_canvas_matrix(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl, tb_null);

    // the matrix
    return &impl->matrix;
}
gb_clipper_ref_t gb_canvas_clipper(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->clipper_stack, tb_null);

    // the clipper
    return (gb_clipper_ref_t)gb_cache_stack_object(impl->clipper_stack);
}
gb_path_ref_t gb_canvas_save_path(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->path_stack, tb_null);

    // save path
    return (gb_path_ref_t)gb_cache_stack_save(impl->path_stack);
}
tb_void_t gb_canvas_load_path(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->path_stack);

    // load path
    gb_cache_stack_load(impl->path_stack);
}
gb_paint_ref_t gb_canvas_save_paint(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->device && impl->paint_stack, tb_null);

    // save paint
    gb_paint_ref_t paint = (gb_paint_ref_t)gb_cache_stack_save(impl->paint_stack);

    // bind paint
    gb_device_bind_paint(impl->device, paint);

    // ok?
    return paint;
}
tb_void_t gb_canvas_load_paint(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && impl->paint_stack);

    // load paint
    gb_cache_stack_load(impl->paint_stack);

    // bind paint
    gb_device_bind_paint(impl->device, gb_canvas_paint(canvas));
}
gb_matrix_ref_t gb_canvas_save_matrix(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->device && impl->matrix_stack, tb_null);

    // save matrix
    tb_stack_put(impl->matrix_stack, &impl->matrix);

    // bind matrix
    gb_device_bind_matrix(impl->device, &impl->matrix);

    // the matrix
    return &impl->matrix;
}
tb_void_t gb_canvas_load_matrix(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && impl->matrix_stack);

    // init matrix
    gb_matrix_ref_t matrix = (gb_matrix_ref_t)tb_stack_top(impl->matrix_stack);
    tb_assert_and_check_return(matrix);

    // load matrix
    impl->matrix = *matrix;

    // bind matrix
    gb_device_bind_matrix(impl->device, &impl->matrix);

    // pop it
    tb_stack_pop(impl->matrix_stack);
}
gb_clipper_ref_t gb_canvas_save_clipper(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return_val(impl && impl->clipper_stack, tb_null);

    // save clipper
    gb_clipper_ref_t clipper = (gb_clipper_ref_t)gb_cache_stack_save(impl->clipper_stack);

    // bind clipper
    gb_device_bind_clipper(impl->device, clipper);

    // ok?
    return clipper;
}
tb_void_t gb_canvas_load_clipper(gb_canvas_ref_t canvas)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->clipper_stack);

    // load clipper
    gb_cache_stack_load(impl->clipper_stack);

    // bind clipper
    gb_device_bind_clipper(impl->device, gb_canvas_clipper(canvas));
}
tb_void_t gb_canvas_clear_path(gb_canvas_ref_t canvas)
{
    gb_path_clear(gb_canvas_path(canvas));
}
tb_void_t gb_canvas_clear_paint(gb_canvas_ref_t canvas)
{
    gb_paint_clear(gb_canvas_paint(canvas));
}
tb_void_t gb_canvas_clear_matrix(gb_canvas_ref_t canvas)
{
    gb_matrix_clear(gb_canvas_matrix(canvas));
}
tb_void_t gb_canvas_clear_clipper(gb_canvas_ref_t canvas)
{
    gb_clipper_clear(gb_canvas_clipper(canvas));
}
tb_void_t gb_canvas_mode_set(gb_canvas_ref_t canvas, tb_size_t mode)
{
    gb_paint_mode_set(gb_canvas_paint(canvas), mode);
}
tb_void_t gb_canvas_flag_set(gb_canvas_ref_t canvas, tb_size_t flag)
{
    gb_paint_flag_set(gb_canvas_paint(canvas), flag);
}
tb_void_t gb_canvas_color_set(gb_canvas_ref_t canvas, gb_color_t color)
{
    gb_paint_color_set(gb_canvas_paint(canvas), color);
}
tb_void_t gb_canvas_alpha_set(gb_canvas_ref_t canvas, tb_byte_t alpha)
{
    gb_paint_alpha_set(gb_canvas_paint(canvas), alpha);
}
tb_void_t gb_canvas_width_set(gb_canvas_ref_t canvas, gb_float_t width)
{
    gb_paint_width_set(gb_canvas_paint(canvas), width);
}
tb_void_t gb_canvas_cap_set(gb_canvas_ref_t canvas, tb_size_t cap)
{
    gb_paint_cap_set(gb_canvas_paint(canvas), cap);
}
tb_void_t gb_canvas_join_set(gb_canvas_ref_t canvas, tb_size_t join)
{
    gb_paint_join_set(gb_canvas_paint(canvas), join);
}
tb_void_t gb_canvas_shader_set(gb_canvas_ref_t canvas, gb_shader_ref_t shader)
{
    gb_paint_shader_set(gb_canvas_paint(canvas), shader);
}
tb_bool_t gb_canvas_rotate(gb_canvas_ref_t canvas, gb_float_t degrees)
{
    return gb_matrix_rotate(gb_canvas_matrix(canvas), degrees);
}
tb_bool_t gb_canvas_rotate_lhs(gb_canvas_ref_t canvas, gb_float_t degrees)
{
    return gb_matrix_rotate_lhs(gb_canvas_matrix(canvas), degrees);
}
tb_bool_t gb_canvas_rotatep(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    return gb_matrix_rotatep(gb_canvas_matrix(canvas), degrees, px, py);
}
tb_bool_t gb_canvas_rotatep_lhs(gb_canvas_ref_t canvas, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    return gb_matrix_rotatep_lhs(gb_canvas_matrix(canvas), degrees, px, py);
}
tb_bool_t gb_canvas_scale(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy)
{
    return gb_matrix_scale(gb_canvas_matrix(canvas), sx, sy);
}
tb_bool_t gb_canvas_scale_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy)
{
    return gb_matrix_scale_lhs(gb_canvas_matrix(canvas), sx, sy);
}
tb_bool_t gb_canvas_scalep(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    return gb_matrix_scalep(gb_canvas_matrix(canvas), sx, sy, px, py);
}
tb_bool_t gb_canvas_scalep_lhs(gb_canvas_ref_t canvas, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    return gb_matrix_scalep_lhs(gb_canvas_matrix(canvas), sx, sy, px, py);
}
tb_bool_t gb_canvas_skew(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky)
{
    return gb_matrix_skew(gb_canvas_matrix(canvas), kx, ky);
}
tb_bool_t gb_canvas_skew_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky)
{
    return gb_matrix_skew_lhs(gb_canvas_matrix(canvas), kx, ky);
}
tb_bool_t gb_canvas_skewp(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    return gb_matrix_skewp(gb_canvas_matrix(canvas), kx, ky, px, py);
}
tb_bool_t gb_canvas_skewp_lhs(gb_canvas_ref_t canvas, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    return gb_matrix_skewp_lhs(gb_canvas_matrix(canvas), kx, ky, px, py);
}
tb_bool_t gb_canvas_sincos(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos)
{
    return gb_matrix_sincos(gb_canvas_matrix(canvas), sin, cos);
}
tb_bool_t gb_canvas_sincos_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos)
{
    return gb_matrix_sincos_lhs(gb_canvas_matrix(canvas), sin, cos);
}
tb_bool_t gb_canvas_sincosp(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    return gb_matrix_sincosp(gb_canvas_matrix(canvas), sin, cos, px, py);
}
tb_bool_t gb_canvas_sincosp_lhs(gb_canvas_ref_t canvas, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    return gb_matrix_sincosp_lhs(gb_canvas_matrix(canvas), sin, cos, px, py);
}
tb_bool_t gb_canvas_translate(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy)
{
    return gb_matrix_translate(gb_canvas_matrix(canvas), dx, dy);
}
tb_bool_t gb_canvas_translate_lhs(gb_canvas_ref_t canvas, gb_float_t dx, gb_float_t dy)
{
    return gb_matrix_translate_lhs(gb_canvas_matrix(canvas), dx, dy);
}
tb_bool_t gb_canvas_multiply(gb_canvas_ref_t canvas, gb_matrix_ref_t factor)
{
    return gb_matrix_multiply(gb_canvas_matrix(canvas), factor);
}
tb_bool_t gb_canvas_multiply_lhs(gb_canvas_ref_t canvas, gb_matrix_ref_t factor)
{
    return gb_matrix_multiply_lhs(gb_canvas_matrix(canvas), factor);
}
tb_void_t gb_canvas_clos(gb_canvas_ref_t canvas)
{
    gb_path_clos(gb_canvas_path(canvas));
}
tb_void_t gb_canvas_move_to(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    gb_path_move_to(gb_canvas_path(canvas), point);
}
tb_void_t gb_canvas_move2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    // make point
    gb_point_t point = gb_point_make(x, y);

    // move-to
    gb_canvas_move_to(canvas, &point);
}
tb_void_t gb_canvas_move2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{
    // make point
    gb_point_t point = gb_point_imake(x, y);

    // move-to
    gb_canvas_move_to(canvas, &point);
}
tb_void_t gb_canvas_line_to(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    gb_path_line_to(gb_canvas_path(canvas), point);
}
tb_void_t gb_canvas_line2_to(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    // make point
    gb_point_t point = gb_point_make(x, y);

    // line-to
    gb_canvas_line_to(canvas, &point);
}
tb_void_t gb_canvas_line2i_to(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{    
    // make point
    gb_point_t point = gb_point_imake(x, y);

    // line-to
    gb_canvas_line_to(canvas, &point);
}
tb_void_t gb_canvas_quad_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl, gb_point_ref_t point)
{
    gb_path_quad_to(gb_canvas_path(canvas), ctrl, point);
}
tb_void_t gb_canvas_quad2_to(gb_canvas_ref_t canvas, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y)
{
    // make ctrl
    gb_point_t ctrl = gb_point_make(cx, cy);

    // make point
    gb_point_t point = gb_point_make(x, y);

    // quad-to
    gb_canvas_quad_to(canvas, &ctrl, &point);
}
tb_void_t gb_canvas_quad2i_to(gb_canvas_ref_t canvas, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y)
{
    // make ctrl
    gb_point_t ctrl = gb_point_imake(cx, cy);

    // make point
    gb_point_t point = gb_point_imake(x, y);

    // quad-to
    gb_canvas_quad_to(canvas, &ctrl, &point);
}
tb_void_t gb_canvas_cube_to(gb_canvas_ref_t canvas, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point)
{
    gb_path_cube_to(gb_canvas_path(canvas), ctrl0, ctrl1, point);
}
tb_void_t gb_canvas_cube2_to(gb_canvas_ref_t canvas, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y)
{
    // make ctrl0
    gb_point_t ctrl0 = gb_point_make(cx0, cy0);

    // make ctrl1
    gb_point_t ctrl1 = gb_point_make(cx1, cy1);

    // make point
    gb_point_t point = gb_point_make(x, y);

    // cube-to
    gb_canvas_cube_to(canvas, &ctrl0, &ctrl1, &point);
}
tb_void_t gb_canvas_cube2i_to(gb_canvas_ref_t canvas, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y)
{
    // make ctrl0
    gb_point_t ctrl0 = gb_point_imake(cx0, cy0);

    // make ctrl1
    gb_point_t ctrl1 = gb_point_imake(cx1, cy1);

    // make point
    gb_point_t point = gb_point_imake(x, y);

    // cube-to
    gb_canvas_cube_to(canvas, &ctrl0, &ctrl1, &point);
}
tb_void_t gb_canvas_arc_to(gb_canvas_ref_t canvas, gb_arc_ref_t arc)
{
    gb_path_arc_to(gb_canvas_path(canvas), arc);
}
tb_void_t gb_canvas_arc2_to(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_make(x0, y0, rx, ry, ab, an);

    // arc-to
    gb_canvas_arc_to(canvas, &arc);
}
tb_void_t gb_canvas_arc2i_to(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_imake(x0, y0, rx, ry, ab, an);

    // arc-to
    gb_canvas_arc_to(canvas, &arc);
}
tb_void_t gb_canvas_clip_path(gb_canvas_ref_t canvas, tb_size_t mode, gb_path_ref_t path)
{
    // clip path
    gb_clipper_add_path(gb_canvas_clipper(canvas), mode, path);
}
tb_void_t gb_canvas_clip_triangle(gb_canvas_ref_t canvas, tb_size_t mode, gb_triangle_ref_t triangle)
{
    // clip triangle
    gb_clipper_add_triangle(gb_canvas_clipper(canvas), mode, triangle);
}
tb_void_t gb_canvas_clip_triangle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_make(x0, y0, x1, y1, x2, y2);

    // clip triangle
    gb_canvas_clip_triangle(canvas, mode, &triangle);
}
tb_void_t gb_canvas_clip_triangle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_imake(x0, y0, x1, y1, x2, y2);

    // clip triangle
    gb_canvas_clip_triangle(canvas, mode, &triangle);
}
tb_void_t gb_canvas_clip_rect(gb_canvas_ref_t canvas, tb_size_t mode, gb_rect_ref_t rect)
{
    // clip rect
    gb_clipper_add_rect(gb_canvas_clipper(canvas), mode, rect);
}
tb_void_t gb_canvas_clip_rect2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_make(x, y, w, h);

    // clip rect
    gb_canvas_clip_rect(canvas, mode, &rect);
}
tb_void_t gb_canvas_clip_rect2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_imake(x, y, w, h);

    // clip rect
    gb_canvas_clip_rect(canvas, mode, &rect);
}
tb_void_t gb_canvas_clip_circle(gb_canvas_ref_t canvas, tb_size_t mode, gb_circle_ref_t circle)
{
    // clip circle
    gb_clipper_add_circle(gb_canvas_clipper(canvas), mode, circle);
}
tb_void_t gb_canvas_clip_circle2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_make(x0, y0, r);

    // clip circle
    gb_canvas_clip_circle(canvas, mode, &circle);
}
tb_void_t gb_canvas_clip_circle2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_imake(x0, y0, r);

    // clip circle
    gb_canvas_clip_circle(canvas, mode, &circle);
}
tb_void_t gb_canvas_clip_ellipse(gb_canvas_ref_t canvas, tb_size_t mode, gb_ellipse_ref_t ellipse)
{
    // clip ellipse
    gb_clipper_add_ellipse(gb_canvas_clipper(canvas), mode, ellipse);
}
tb_void_t gb_canvas_clip_ellipse2(gb_canvas_ref_t canvas, tb_size_t mode, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_make(x0, y0, rx, ry);

    // clip ellipse
    gb_canvas_clip_ellipse(canvas, mode, &ellipse);
}
tb_void_t gb_canvas_clip_ellipse2i(gb_canvas_ref_t canvas, tb_size_t mode, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_imake(x0, y0, rx, ry);

    // clip ellipse
    gb_canvas_clip_ellipse(canvas, mode, &ellipse);
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
    // draw path
    gb_canvas_draw_path(canvas, gb_canvas_path(canvas));
}
tb_void_t gb_canvas_draw_path(gb_canvas_ref_t canvas, gb_path_ref_t path)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && path);

    // draw path
    gb_device_draw_path(impl->device, path);
}
tb_void_t gb_canvas_draw_point(gb_canvas_ref_t canvas, gb_point_ref_t point)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device);

    // draw point
    gb_device_draw_points(impl->device, point, 1, tb_null);
}
tb_void_t gb_canvas_draw_point2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y)
{
    // make point
    gb_point_t point = gb_point_make(x, y);

    // draw point
    gb_canvas_draw_point(canvas, &point);
}
tb_void_t gb_canvas_draw_point2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y)
{
    // make point
    gb_point_t point = gb_point_imake(x, y);

    // draw point
    gb_canvas_draw_point(canvas, &point);
}
tb_void_t gb_canvas_draw_line(gb_canvas_ref_t canvas, gb_line_ref_t line)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && line);

    // init points
    gb_point_t points[] = {line->p0, line->p1};

    // init bounds
    gb_rect_t bounds;
    gb_bounds_make(&bounds, points, tb_arrayn(points));

    // draw lines
    gb_device_draw_lines(impl->device, points, 2, &bounds);
}
tb_void_t gb_canvas_draw_line2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1)
{
    // make line
    gb_line_t line = gb_line_make(x0, y0, x1, y1);

    // draw line
    gb_canvas_draw_line(canvas, &line);
}
tb_void_t gb_canvas_draw_line2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1)
{
    // make line
    gb_line_t line = gb_line_imake(x0, y0, x1, y1);

    // draw line
    gb_canvas_draw_line(canvas, &line);
}
tb_void_t gb_canvas_draw_arc(gb_canvas_ref_t canvas, gb_arc_ref_t arc)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && arc);

    // save path
    gb_path_ref_t path = gb_canvas_save_path(canvas);
    tb_assert_and_check_return(path);

    // make arc
    gb_path_clear(path);
    gb_path_add_arc(path, arc);

    // draw it
    gb_canvas_draw_path(canvas, path);

    // load path
    gb_canvas_load_path(canvas);
}
tb_void_t gb_canvas_draw_arc2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_make(x0, y0, rx, ry, ab, an);

    // draw arc
    gb_canvas_draw_arc(canvas, &arc);
}
tb_void_t gb_canvas_draw_arc2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an)
{
    // make arc
    gb_arc_t arc = gb_arc_imake(x0, y0, rx, ry, ab, an);

    // draw arc
    gb_canvas_draw_arc(canvas, &arc);
}
tb_void_t gb_canvas_draw_triangle(gb_canvas_ref_t canvas, gb_triangle_ref_t triangle)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && triangle);

    // init polygon
    gb_point_t      points[] = {triangle->p0, triangle->p1, triangle->p2, triangle->p0};
    tb_uint16_t     counts[] = {4, 0};
    gb_polygon_t    polygon = {points, counts, tb_true};

    // init hint
    gb_shape_t      hint;
    hint.type       = GB_SHAPE_TYPE_TRIANGLE;
    hint.u.triangle = *triangle;

    // init bounds
    gb_rect_t       bounds;
    gb_bounds_make(&bounds, points, tb_arrayn(points));

    // draw it
    gb_device_draw_polygon(impl->device, &polygon, &hint, &bounds);
}
tb_void_t gb_canvas_draw_triangle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_make(x0, y0, x1, y1, x2, y2);

    // draw triangle
    gb_canvas_draw_triangle(canvas, &triangle);
}
tb_void_t gb_canvas_draw_triangle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    // make triangle
    gb_triangle_t triangle = gb_triangle_imake(x0, y0, x1, y1, x2, y2);

    // draw triangle
    gb_canvas_draw_triangle(canvas, &triangle);
}
tb_void_t gb_canvas_draw_rect(gb_canvas_ref_t canvas, gb_rect_ref_t rect)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && rect);

    // init polygon
    gb_point_t      points[5];
    tb_uint16_t     counts[] = {5, 0};
    gb_polygon_t    polygon = {points, counts, tb_true};

    // init points
    points[0].x = rect->x;
    points[0].y = rect->y;
    points[1].x = rect->x + rect->w;
    points[1].y = rect->y;
    points[2].x = rect->x + rect->w;
    points[2].y = rect->y + rect->h;
    points[3].x = rect->x;
    points[3].y = rect->y + rect->h;
    points[4] = points[0];

    // init hint
    gb_shape_t      hint;
    hint.type       = GB_SHAPE_TYPE_RECT;
    hint.u.rect     = *rect;

    // draw it
    gb_device_draw_polygon(impl->device, &polygon, &hint, rect);
}
tb_void_t gb_canvas_draw_rect2(gb_canvas_ref_t canvas, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_make(x, y, w, h);

    // draw rect
    gb_canvas_draw_rect(canvas, &rect);
}
tb_void_t gb_canvas_draw_rect2i(gb_canvas_ref_t canvas, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    // make rect
    gb_rect_t rect = gb_rect_imake(x, y, w, h);

    // draw rect
    gb_canvas_draw_rect(canvas, &rect);
}
tb_void_t gb_canvas_draw_circle(gb_canvas_ref_t canvas, gb_circle_ref_t circle)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && circle);

    // save path
    gb_path_ref_t path = gb_canvas_save_path(canvas);
    tb_assert_and_check_return(path);

    // make circle
    gb_path_clear(path);
    gb_path_add_circle(path, circle, GB_PATH_DIRECTION_CW);

    // draw it
    gb_canvas_draw_path(canvas, path);

    // load path
    gb_canvas_load_path(canvas);
}
tb_void_t gb_canvas_draw_circle2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_make(x0, y0, r);

    // draw circle
    gb_canvas_draw_circle(canvas, &circle);
}
tb_void_t gb_canvas_draw_circle2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    // make circle
    gb_circle_t circle = gb_circle_imake(x0, y0, r);

    // draw circle
    gb_canvas_draw_circle(canvas, &circle);
}
tb_void_t gb_canvas_draw_ellipse(gb_canvas_ref_t canvas, gb_ellipse_ref_t ellipse)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && ellipse);

    // save path
    gb_path_ref_t path = gb_canvas_save_path(canvas);
    tb_assert_and_check_return(path);

    // make ellipse
    gb_path_clear(path);
    gb_path_add_ellipse(path, ellipse, GB_PATH_DIRECTION_CW);

    // draw it
    gb_canvas_draw_path(canvas, path);

    // load path
    gb_canvas_load_path(canvas);
}
tb_void_t gb_canvas_draw_ellipse2(gb_canvas_ref_t canvas, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_make(x0, y0, rx, ry);

    // draw ellipse
    gb_canvas_draw_ellipse(canvas, &ellipse);
}
tb_void_t gb_canvas_draw_ellipse2i(gb_canvas_ref_t canvas, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    // make ellipse
    gb_ellipse_t ellipse = gb_ellipse_imake(x0, y0, rx, ry);

    // draw ellipse
    gb_canvas_draw_ellipse(canvas, &ellipse);
}
tb_void_t gb_canvas_draw_polygon(gb_canvas_ref_t canvas, gb_polygon_ref_t polygon)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device);

    // draw polygon
    gb_device_draw_polygon(impl->device, polygon, tb_null, tb_null);
}
tb_void_t gb_canvas_draw_lines(gb_canvas_ref_t canvas, gb_point_ref_t points, tb_size_t count)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && count && !(count & 0x1));
 
    // draw lines
    gb_device_draw_lines(impl->device, points, count, tb_null);
}
tb_void_t gb_canvas_draw_points(gb_canvas_ref_t canvas, gb_point_ref_t points, tb_size_t count)
{
    // check
    gb_canvas_impl_t* impl = (gb_canvas_impl_t*)canvas;
    tb_assert_and_check_return(impl && impl->device && count);

    // draw points
    gb_device_draw_points(impl->device, points, count, tb_null);
}
