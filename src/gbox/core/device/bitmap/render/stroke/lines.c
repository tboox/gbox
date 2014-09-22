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
 * @file        lines.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_stroke_lines"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "lines.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static tb_void_t gb_bitmap_render_stroke_lines_w1_generic(gb_bitmap_device_ref_t device, gb_point_ref_t pb, gb_point_ref_t pe)
{

}
static tb_void_t gb_bitmap_render_stroke_lines_w1_vertical(gb_bitmap_device_ref_t device, gb_point_ref_t pb, gb_point_ref_t pe)
{

}
static tb_void_t gb_bitmap_render_stroke_lines_w1_horizontal(gb_bitmap_device_ref_t device, gb_point_ref_t pb, gb_point_ref_t pe)
{

}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_stroke_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && device->base.paint && device->base.matrix && points && count);

    // the width
    gb_float_t width = gb_paint_width(device->base.paint);

    // width == 1 and solid? 
    if (gb_e1(width) && gb_e1(gb_fabs(device->base.matrix->sx)) && gb_e1(gb_fabs(device->base.matrix->sy)) && !device->render.shader)
        gb_bitmap_render_stroke_lines_w1(device, points, count);
    else
    {
        // TODO
        gb_bitmap_render_stroke_lines_w1(device, points, count);
    }
}
tb_void_t gb_bitmap_render_stroke_lines_w1(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(points && count && !(count & 0x1));

    // done
    tb_size_t       i = 0;
    gb_point_ref_t  pb = tb_null;
    gb_point_ref_t  pe = tb_null;
    for (i = 0; i < count; i += 2)
    {
        // line: pb => pe
        pb = points + i;
        pe = points + i + 1;

        // horizontal?
        if (pb->y == pe->y) gb_bitmap_render_stroke_lines_w1_horizontal(device, pb, pe);
        // vertical?
        else if (pb->x == pe->x) gb_bitmap_render_stroke_lines_w1_vertical(device, pb, pe);
        // generic?
        else gb_bitmap_render_stroke_lines_w1_generic(device, pb, pe);
    }
}
