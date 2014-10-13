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
#define TB_TRACE_MODULE_NAME            "bitmap_biltter_solid"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "solid.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_biltter_solid_done_h(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w)
{
    // check
    tb_assert_abort(biltter && biltter->pixmap && biltter->pixmap->pixels_fill);
    tb_assert_abort(x >= 0 && y >= 0 && w >= 0);

    // no width? ignore it
    tb_check_return(w);

    // the pixels
    tb_byte_t* pixels = (tb_byte_t*)gb_bitmap_data(biltter->bitmap);
    tb_assert_abort(pixels);

    // done
    biltter->pixmap->pixels_fill(pixels + y * biltter->row_bytes + x * biltter->btp, biltter->u.solid.pixel, w, biltter->u.solid.alpha);
}
static tb_void_t gb_bitmap_biltter_solid_done_v(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t h)
{
    // check
    tb_assert_abort(biltter && biltter->pixmap && biltter->pixmap->pixels_fill);
    tb_assert_abort(x >= 0 && y >= 0 && h >= 0);

    // no height? ignore it
    tb_check_return(h);

    // the pixels
    tb_byte_t* pixels = (tb_byte_t*)gb_bitmap_data(biltter->bitmap);
    tb_assert_abort(pixels);

    // the factors
    tb_size_t                       btp = biltter->btp;
    tb_size_t                       row_bytes = biltter->row_bytes;
    gb_pixel_t                      pixel = biltter->u.solid.pixel;
    tb_byte_t                       alpha = biltter->u.solid.alpha;
    gb_pixmap_func_pixel_set_t      pixel_set = biltter->pixmap->pixel_set;

    // done
    while (h--) pixel_set(pixels + y++ * row_bytes + x * btp, pixel, alpha);
}
static tb_void_t gb_bitmap_biltter_solid_done_r(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w, tb_long_t h)
{
    // check
    tb_assert_abort(biltter && biltter->pixmap && biltter->pixmap->pixels_fill);
    tb_assert_abort(x >= 0 && y >= 0 && w >= 0 && h >= 0);

    // no width and height? ignore it
    tb_check_return(h && w);

    // the pixels
    tb_byte_t* pixels = (tb_byte_t*)gb_bitmap_data(biltter->bitmap);
    tb_assert_abort(pixels);

    // the factors
    tb_size_t                       btp = biltter->btp;
    tb_size_t                       row_bytes = biltter->row_bytes;
    gb_pixel_t                      pixel = biltter->u.solid.pixel;
    tb_byte_t                       alpha = biltter->u.solid.alpha;
    gb_pixmap_func_pixels_fill_t    pixels_fill = biltter->pixmap->pixels_fill;

    // done
    if (!x && (w * btp == row_bytes)) pixels_fill(pixels, pixel, h * w, alpha);
    else
    {
        while (h--) pixels_fill(pixels + y++ * row_bytes + x * btp, pixel, w, alpha);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_bitmap_biltter_solid_init(gb_bitmap_biltter_ref_t biltter, gb_bitmap_ref_t bitmap, gb_paint_ref_t paint)
{
    // check
    tb_assert_abort(biltter && bitmap && paint);
 
    // init bitmap
    biltter->bitmap = bitmap;

    // init pixmap
    biltter->pixmap = gb_pixmap(gb_bitmap_pixfmt(bitmap), gb_paint_alpha(paint));
    tb_check_return_val(biltter->pixmap, tb_false);

    // init btp and row_bytes
    biltter->btp        = biltter->pixmap->btp;
    biltter->row_bytes  = gb_bitmap_row_bytes(biltter->bitmap);

    // init solid
    biltter->u.solid.pixel = biltter->pixmap->pixel(gb_paint_color(paint));
    biltter->u.solid.alpha = gb_paint_alpha(paint);

    // init operations
    biltter->done_h     = gb_bitmap_biltter_solid_done_h;
    biltter->done_v     = gb_bitmap_biltter_solid_done_v;
    biltter->done_r     = gb_bitmap_biltter_solid_done_r;
    biltter->exit       = tb_null;

    // ok
    return tb_true;
}

