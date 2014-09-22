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
#include "render/stroke/stroke.h"
#include "../../impl/stroke.h"

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

    // stroke lines
    gb_bitmap_render_stroke_lines(device, points, count);
}
tb_void_t gb_bitmap_render_draw_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint && device->base.matrix && device->points && points && count);

    // the width
    gb_float_t width = gb_paint_width(device->base.paint);

    // clear points
    tb_vector_clear(device->points);

    // width == 1 and solid? stroke it
    if (gb_e1(width) && gb_e1(gb_fabs(device->base.matrix->sx)) && gb_e1(gb_fabs(device->base.matrix->sy)) && !device->render.shader)
    {
        // apply matrix to points
        tb_size_t index = 0;
        for (index = 0; index < count; index++)
        {
            // apply to point
            gb_point_t point;
            gb_matrix_apply_point2(device->base.matrix, points + index, &point);

            // append point
            tb_vector_insert_tail(device->points, &point);
        }

        // the stroked points 
        gb_point_ref_t  stroked_points = (gb_point_ref_t)tb_vector_data(device->points);
        tb_assert_abort(stroked_points);

        // TODO: clip it
        // ...

        // stroke points
        gb_bitmap_render_stroke_points(device, stroked_points, tb_vector_size(device->points));
    }
    // make the filled polygon 
    else
    {
        // check
        tb_assert_abort(device->counts);

        // clear counts
        tb_vector_clear(device->counts);

        // make the filled polygon
        tb_bool_t       filled_convex = gb_stroke_make_fill_for_points(device->points, device->counts, device->base.paint, points, count);
        gb_point_ref_t  filled_points = (gb_point_ref_t)tb_vector_data(device->points);
        tb_uint16_t*    filled_counts = (tb_uint16_t*)tb_vector_data(device->counts);
        gb_polygon_t    filled_polygon = {filled_points, filled_counts, filled_convex};
        tb_assert_abort(filled_points && filled_counts);

        // apply matrix to the filled points
        gb_matrix_apply_points(device->base.matrix, filled_points, tb_vector_size(device->points));

        // TODO: clip it
        // ...

        // TODO: paint: fill

        // fill polygon, TODO: hint
        gb_bitmap_render_fill_polygon(device, &filled_polygon, tb_null);
    }
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

    // stroke it
    if (mode & GB_PAINT_MODE_STROKE)
    {
        // stroke polygon
        gb_bitmap_render_stroke_polygon(device, polygon, hint);
    }
}

