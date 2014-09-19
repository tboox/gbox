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
 * @file        render.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_render"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "render.h"
#include "render/fill/fill.h"
#include "render/stok/stok.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_bitmap_render_init(gb_bitmap_device_ref_t device)
{
    // check
    tb_assert_and_check_return_val(device && device->base.matrix && device->base.paint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // init render
        tb_memset(&device->render, 0, sizeof(device->render));

        // init shader
        device->render.shader = gb_paint_shader(device->base.paint);

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_void_t gb_bitmap_render_exit(gb_bitmap_device_ref_t device)
{
    // check
    tb_assert_and_check_return(device);

}
tb_void_t gb_bitmap_render_draw_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // TODO: clip it
    // ...

    // stok lines
    gb_bitmap_render_stok_lines(device, points, count);
}
tb_void_t gb_bitmap_render_draw_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // TODO: clip it
    // ...

    // stok points
    gb_bitmap_render_stok_points(device, points, count);
}
tb_void_t gb_bitmap_render_draw_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint);

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // TODO: clip it
    // ...

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        // fill polygon
        gb_bitmap_render_fill_polygon(device, polygon, hint);
    }

    // stok it
    if (mode & GB_PAINT_MODE_STOK)
    {
        // stok polygon
        gb_bitmap_render_stok_polygon(device, polygon, hint);
    }
}

