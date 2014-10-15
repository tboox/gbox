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
#include "biltter.h"
#include "../../impl/raster.h"
#include "../../impl/stroker.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

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

    // the bounds
    gb_rect_t                       bounds;

    // the shader
    gb_shader_ref_t                 shader;

    // the raster
    gb_raster_ref_t                 raster;

    // the biltter
    gb_bitmap_biltter_t             biltter;

    // the stroker
    gb_stroker_ref_t                stroker;

}gb_bitmap_device_t, *gb_bitmap_device_ref_t;

#endif


