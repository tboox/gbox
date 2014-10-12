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
 * @file        rect.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_fill_rect"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "rect.h"
#include "filler.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_fill_rect(gb_bitmap_device_ref_t device, gb_rect_ref_t rect)
{
    // check
    tb_assert_abort(device && device->base.paint && device->pixmap);
    tb_assert_abort(device->bitmap && gb_bitmap_data(device->bitmap) && rect);

    // the factors
    tb_byte_t*      data        = gb_bitmap_data(device->bitmap);
    tb_size_t       row_bytes   = gb_bitmap_row_bytes(device->bitmap);
    tb_size_t       btp         = device->pixmap->btp;
	tb_long_t       x 		    = gb_float_to_long(rect->x);
	tb_long_t       y 		    = gb_float_to_long(rect->y);
	tb_long_t       w 		    = gb_float_to_long(rect->w);
	tb_long_t       h 		    = gb_float_to_long(rect->h);
	tb_byte_t*      q 		    = data + y * row_bytes + x * btp;

    // optimization for solid
    if (!x && !device->shader && (w * btp == row_bytes))
        device->pixmap->pixels_fill(q, device->pixmap->pixel(gb_paint_color(device->base.paint)), h * w, gb_paint_alpha(device->base.paint));
    // fill for solid
    else if (!device->shader)
    {
        // done fill
        gb_pixel_t                      pixel       = device->pixmap->pixel(gb_paint_color(device->base.paint));
        tb_byte_t                       alpha       = gb_paint_alpha(device->base.paint);
        gb_pixmap_func_pixels_fill_t    pixels_fill = device->pixmap->pixels_fill;
        while (h--)
        {
            pixels_fill(q, pixel, w, alpha);
            q += row_bytes;
        }
    }
    // fill for shader
    else
    {
        // init filler
        gb_bitmap_filler_ref_t filler = gb_bitmap_render_filler_init(device, rect);
        if (filler)
        {
            // done filler
            while (h--)
            {
                gb_bitmap_render_filler_done(filler, x, w, q);
                q += row_bytes;
            }

            // exit filler
            gb_bitmap_render_filler_exit(filler);
        }
    }
}
