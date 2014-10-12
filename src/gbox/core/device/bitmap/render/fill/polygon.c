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
#include "filler.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_render_fill_polygon_raster(tb_long_t y, tb_long_t xb, tb_long_t xe, tb_cpointer_t priv)
{
    // the factors
    tb_value_ref_t factors = (tb_value_ref_t)priv;
    tb_assert_abort(factors && factors[0].ptr && factors[1].ptr && xe >= xb);

    // done filler
    gb_bitmap_render_filler_done((gb_bitmap_filler_ref_t)factors[0].ptr, xb, xe - xb, (tb_byte_t*)factors[1].ptr + y * factors[2].ul + xb * factors[3].ul);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_fill_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint && device->pixmap && device->bitmap && gb_bitmap_data(device->bitmap) && bounds);

    // init raster
    if (gb_polygon_raster_init(&device->raster, polygon, bounds)) 
    {
        // init filler
        gb_bitmap_filler_ref_t filler = gb_bitmap_render_filler_init(device, bounds);
        if (filler)
        {
            // the factors
            tb_value_t factors[4];
            factors[0].ptr  = filler;
            factors[1].ptr  = gb_bitmap_data(device->bitmap);
            factors[2].ul   = gb_bitmap_row_bytes(device->bitmap);
            factors[3].ul   = device->pixmap->btp;

            // the paint rule
            tb_size_t rule = gb_paint_fill_rule(device->base.paint);

            // done raster
            gb_polygon_raster_done(&device->raster, rule, gb_bitmap_render_fill_polygon_raster, factors);

            // exit filler
            gb_bitmap_render_filler_exit(filler);
        }

        // exit raster
        gb_polygon_raster_exit(&device->raster);
    }
}
