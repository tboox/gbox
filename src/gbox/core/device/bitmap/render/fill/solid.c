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
 * @file        solid.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_fill_solid"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "solid.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_render_fill_solid_done(gb_bitmap_filler_ref_t filler, tb_size_t start, tb_size_t count, tb_byte_t* pixels)
{
    // check
    tb_assert_abort(filler && filler->pixels_fill);

    // done
    filler->pixels_fill(pixels, filler->u.solid.pixel, count, filler->u.solid.alpha);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_bitmap_filler_ref_t gb_bitmap_render_fill_solid_init(gb_bitmap_device_ref_t device, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->pixmap && device->base.paint);

    // init it
    device->filler.done             = gb_bitmap_render_fill_solid_done;
    device->filler.exit             = tb_null;
    device->filler.u.solid.pixel    = device->pixmap->pixel(gb_paint_color(device->base.paint));
    device->filler.u.solid.alpha    = gb_paint_alpha(device->base.paint);
    device->filler.pixels_fill      = device->pixmap->pixels_fill;

    // ok
    return &device->filler;
}

