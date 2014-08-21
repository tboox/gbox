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
#ifdef GB_CONFIG_THIRD_HAVE_GL
gb_canvas_ref_t gb_canvas_init_from_gl(gb_gl_view_ref_t view)
{
    // done
    gb_canvas_ref_t canvas = tb_null;
    gb_device_ref_t device = tb_null;
    do
    {
        // init device 
        device = gb_device_init_gl(view);
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

