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
 * @file        polygon.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_polygon"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "lines.h"
#include "polygon.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_render_fill_raster(tb_long_t lx, tb_long_t rx, tb_long_t yb, tb_long_t ye, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(priv && rx >= lx && ye > yb);

    // done biltter
    gb_bitmap_biltter_done_r((gb_bitmap_biltter_ref_t)priv, lx, yb, rx - lx, ye - yb);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_fill_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint);

    // done raster
    gb_polygon_raster_done(device->raster, polygon, bounds, gb_paint_fill_rule(device->base.paint), gb_bitmap_render_fill_raster, &device->biltter);
}
tb_void_t gb_bitmap_render_stroke_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_abort(device && polygon && polygon->points && polygon->counts);

    // done
    tb_uint16_t     index = 0;
    gb_point_t      points_line[2];
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    while (index < count)
    {
        // the point
        points_line[1] = *points++;

        // stroke line
        if (index) gb_bitmap_render_stroke_lines(device, points_line, 2);

        // save the previous point
        points_line[0] = points_line[1];
        
        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }
}
