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
 * @file        skia.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "skia/skia.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_skia_resize(gb_device_impl_t* device, tb_size_t width, tb_size_t height)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->bitmap);

}
static tb_void_t gb_device_skia_draw_clear(gb_device_impl_t* device, gb_color_t color)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas);

    // clear it
	impl->canvas->drawColor(gb_skia_color_to_sk(color));
}
static tb_void_t gb_device_skia_fill_polygon(gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && polygon);

    tb_trace_noimpl();
}
static tb_void_t gb_device_skia_stok_segment(gb_device_impl_t* device, gb_segment_ref_t segment, gb_shape_ref_t hint, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && paint && segment);

    tb_trace_noimpl();
}
static gb_shader_ref_t gb_device_skia_shader_linear(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_radial(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_bitmap(gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    tb_trace_noimpl();
    return tb_null;
}
static tb_void_t gb_device_skia_exit(gb_device_impl_t* device)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl);

    // exit canvas
    if (impl->canvas) delete impl->canvas;
    impl->canvas = tb_null;

    // exit bitmap
    if (impl->bitmap) delete impl->bitmap;
    impl->bitmap = tb_null;

    // exit it
    tb_free(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init_skia(gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(bitmap, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_skia_device_ref_t    impl = tb_null;
    do
    {
        // the width and height
        tb_size_t width     = gb_bitmap_width(bitmap);
        tb_size_t height    = gb_bitmap_height(bitmap);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make device
        impl = tb_malloc0_type(gb_skia_device_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_BITMAP;
        impl->base.resize           = gb_device_skia_resize;
        impl->base.draw_clear       = gb_device_skia_draw_clear;
        impl->base.fill_polygon     = gb_device_skia_fill_polygon;
        impl->base.stok_segment     = gb_device_skia_stok_segment;
        impl->base.shader_linear    = gb_device_skia_shader_linear;
        impl->base.shader_radial    = gb_device_skia_shader_radial;
        impl->base.shader_bitmap    = gb_device_skia_shader_bitmap;
        impl->base.exit             = gb_device_skia_exit;

        // make bitmap
        impl->bitmap = new SkBitmap();
        tb_assert_and_check_break(impl->bitmap);

        // init bitmap
        impl->bitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height, gb_bitmap_row_bytes(bitmap));
	    impl->bitmap->setPixels(gb_bitmap_data(bitmap));

        // make canvas
        impl->canvas = new SkCanvas(*impl->bitmap);
        tb_assert_and_check_break(impl->canvas);

        // init canvas
	    impl->canvas->resetMatrix();

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

