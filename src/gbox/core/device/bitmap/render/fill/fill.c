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
 * @file        fill.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_fill"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "fill.h"
#include "solid.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_bitmap_filler_ref_t gb_bitmap_render_fill_init(gb_bitmap_device_ref_t device, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device);

    // done
    gb_bitmap_filler_ref_t filler = tb_null;
    if (device->shader)
    {
        // TODO
        tb_trace_noimpl();
    }
    else filler = gb_bitmap_render_fill_solid_init(device, bounds);

    // ok?
    return filler;
}
tb_void_t gb_bitmap_render_fill_exit(gb_bitmap_filler_ref_t filler)
{
    // check
    tb_assert_abort(filler);

    // exit fill
    if (filler->exit) filler->exit(filler);
}
tb_void_t gb_bitmap_render_fill_done(gb_bitmap_filler_ref_t filler, tb_size_t start, tb_size_t count, tb_byte_t* pixels)
{   
    // check
    tb_assert_abort(filler && filler->done);

    // done fill
    filler->done(filler, start, count, pixels);
}
