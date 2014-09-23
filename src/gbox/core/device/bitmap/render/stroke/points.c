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
 * @file        points.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_stroke_points"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "points.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_stroke_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && device->pixmap && device->base.paint && device->bitmap && gb_bitmap_data(device->bitmap));
    tb_assert_abort(points && count);

    // the factors
    tb_size_t   index;
    tb_byte_t*  data        = gb_bitmap_data(device->bitmap);
    tb_size_t   btp         = device->pixmap->btp;
    gb_pixel_t  pixel       = device->pixmap->pixel(gb_paint_color(device->base.paint));
    tb_byte_t   alpha       = gb_paint_alpha(device->base.paint);
    tb_size_t   row_bytes   = gb_bitmap_row_bytes(device->bitmap);

    // done
    gb_pixmap_func_pixel_set_t pixel_set = device->pixmap->pixel_set;
    for (index = 0; index < count; index++)
    {
        pixel_set(data + gb_float_to_long(points[index].y) * row_bytes + gb_float_to_long(points[index].x) * btp, pixel, alpha);
    }
}
