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
 * @file        device.h
 * @ingroup     core
 */
#ifndef GB_CORE_DEVICE_BITMAP_DEVICE_H
#define GB_CORE_DEVICE_BITMAP_DEVICE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../../impl/polygon_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the bitmap filler solid type
typedef struct __gb_bitmap_filler_solid_t
{
    // the pixel
    gb_pixel_t                      pixel;

    // the alpha
    tb_byte_t                       alpha;

}gb_bitmap_filler_solid_t;

// the bitmap filler type
typedef struct __gb_bitmap_filler_t
{
    union 
    {
        // the solid filler
        gb_bitmap_filler_solid_t    solid;

    }u;

    // the pixels fill func
    gb_pixmap_func_pixels_fill_t    pixels_fill;

    // the exit func
    tb_void_t                       (*exit)(struct __gb_bitmap_filler_t* filler);

    // the done func
    tb_void_t                       (*done)(struct __gb_bitmap_filler_t* filler, tb_size_t start, tb_size_t count, tb_byte_t* pixels);

}gb_bitmap_filler_t, *gb_bitmap_filler_ref_t;

// the bitmap device type
typedef struct __gb_bitmap_device_t
{
    // the base
    gb_device_impl_t                base;

    // the bitmap
    gb_bitmap_ref_t                 bitmap;

    // the pixmap
    gb_pixmap_ref_t                 pixmap;

    // the points 
    tb_vector_ref_t                 points;

    // the counts
    tb_vector_ref_t                 counts;

    // the shader
    gb_shader_ref_t                 shader;

    // the filler
    gb_bitmap_filler_t              filler;

    // the raster
    gb_polygon_raster_t             raster;

}gb_bitmap_device_t, *gb_bitmap_device_ref_t;

#endif


