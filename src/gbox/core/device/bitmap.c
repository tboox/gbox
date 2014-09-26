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
 * @file        bitmap.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device_bitmap"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "bitmap/bitmap.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the points grow count
#ifdef __gb_small__
#   define GB_DEVICE_BITMAP_POINTS_GROW      (64)
#else
#   define GB_DEVICE_BITMAP_POINTS_GROW      (128)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_bitmap_resize(gb_device_impl_t* device, tb_size_t width, tb_size_t height)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->bitmap);

    // resize
    gb_bitmap_resize(impl->bitmap, width, height);
}
static tb_void_t gb_device_bitmap_draw_clear(gb_device_impl_t* device, gb_color_t color)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->bitmap);

    // the pixels data
    tb_pointer_t pixels = gb_bitmap_data(impl->bitmap);
    tb_assert_abort(pixels);

    // the pixmap
    gb_pixmap_ref_t pixmap = impl->pixmap;
    tb_assert_abort(pixmap && pixmap->pixel && pixmap->pixels_fill);

    // the pixels count
    tb_size_t count = gb_bitmap_size(impl->bitmap) / pixmap->btp;
    tb_assert_abort(count);

    // clear it
    pixmap->pixels_fill(pixels, pixmap->pixel(color), count, 0xff);
}
static tb_void_t gb_device_bitmap_draw_lines(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl && points && count);

    // init render
    if (gb_bitmap_render_init(impl))
    {
        // draw lines
        gb_bitmap_render_draw_lines(impl, points, count, bounds);
    
        // exit render
        gb_bitmap_render_exit(impl);
    }
}
static tb_void_t gb_device_bitmap_draw_points(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl && points && count);

    // init render
    if (gb_bitmap_render_init(impl))
    {
        // draw points
        gb_bitmap_render_draw_points(impl, points, count, bounds);
    
        // exit render
        gb_bitmap_render_exit(impl);
    }
}
static tb_void_t gb_device_bitmap_draw_polygon(gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl && polygon);

    // init render
    if (gb_bitmap_render_init(impl))
    {
        // draw polygon
        gb_bitmap_render_draw_polygon(impl, polygon, hint, bounds);
    
        // exit render
        gb_bitmap_render_exit(impl);
    }
}
static gb_shader_ref_t gb_device_bitmap_shader_linear(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_bitmap_shader_radial(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_bitmap_shader_bitmap(gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static tb_void_t gb_device_bitmap_exit(gb_device_impl_t* device)
{
    // check
    gb_bitmap_device_ref_t impl = (gb_bitmap_device_ref_t)device;
    tb_assert_and_check_return(impl);

    // exit points
    if (impl->points) tb_vector_exit(impl->points);
    impl->points = tb_null;

    // exit counts
    if (impl->counts) tb_vector_exit(impl->counts);
    impl->counts = tb_null;

    // exit stroker
    if (impl->stroker) gb_stroker_exit(impl->stroker);
    impl->stroker = tb_null;

    // exit it
    tb_free(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init_bitmap(gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(bitmap, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_bitmap_device_ref_t  impl = tb_null;
    do
    {
        // the width and height
        tb_size_t width     = gb_bitmap_width(bitmap);
        tb_size_t height    = gb_bitmap_height(bitmap);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make device
        impl = tb_malloc0_type(gb_bitmap_device_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_BITMAP;
        impl->base.resize           = gb_device_bitmap_resize;
        impl->base.draw_clear       = gb_device_bitmap_draw_clear;
        impl->base.draw_lines       = gb_device_bitmap_draw_lines;
        impl->base.draw_points      = gb_device_bitmap_draw_points;
        impl->base.draw_polygon     = gb_device_bitmap_draw_polygon;
        impl->base.shader_linear    = gb_device_bitmap_shader_linear;
        impl->base.shader_radial    = gb_device_bitmap_shader_radial;
        impl->base.shader_bitmap    = gb_device_bitmap_shader_bitmap;
        impl->base.exit             = gb_device_bitmap_exit;

        // init bitmap
        impl->bitmap = bitmap;

        // init pixmap
        impl->pixmap = gb_pixmap(gb_bitmap_pixfmt(bitmap), 0xff);
        tb_assert_and_check_break(impl->pixmap);

        // init stroker
        impl->stroker = gb_stroker_init();
        tb_assert_and_check_break(impl->stroker);

        // init points
        impl->points = tb_vector_init(GB_DEVICE_BITMAP_POINTS_GROW, tb_item_func_mem(sizeof(gb_point_t), tb_null, tb_null));
        tb_assert_and_check_break(impl->points);

        // init counts
        impl->counts = tb_vector_init(8, tb_item_func_uint16());
        tb_assert_and_check_break(impl->counts);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_device_exit((gb_device_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_device_ref_t)impl;
}
