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
#define TB_TRACE_MODULE_NAME            "bitmap_fill_polygon"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "polygon.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_render_fill_polygon_raster(tb_long_t xb, tb_long_t xe, tb_long_t yb, tb_long_t ye, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(priv && xe >= xb && ye > yb);

    // done biltter
    gb_bitmap_biltter_done_r((gb_bitmap_biltter_ref_t)priv, xb, yb, xe - xb, ye - yb);
}
static tb_void_t gb_bitmap_render_fill_polygon_done(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && polygon && bounds);

    // init raster
    if (gb_polygon_raster_init(&device->raster, polygon, bounds)) 
    {
        // done raster
        gb_polygon_raster_done(&device->raster, gb_paint_fill_rule(device->base.paint), gb_bitmap_render_fill_polygon_raster, &device->biltter);

        // exit raster
        gb_polygon_raster_exit(&device->raster);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_fill_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && polygon && bounds);

    // convex? fill the each convex contour
    if (polygon->convex)
    {
        // done
        tb_size_t       index               = 0;
        gb_point_ref_t  points              = polygon->points;
        tb_uint16_t*    counts              = polygon->counts;
        tb_uint16_t     contour_counts[2]   = {0, 0};
        gb_polygon_t    contour             = {tb_null, contour_counts, tb_true};
        while ((contour_counts[0] = *counts++))
        {
            // init the polygon for this contour
            contour.points = points + index;

            // fill this convex contour
            gb_bitmap_render_fill_polygon_done(device, &contour, bounds);

            // update the contour index
            index += contour_counts[0];
        }
    }
    // fill this polygon directly
    else gb_bitmap_render_fill_polygon_done(device, polygon, bounds);
}
