/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
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
#include "../../impl/stroker.h"
#include "../../impl/polygon_raster.h"

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
    gb_polygon_raster_ref_t         raster;

    // the biltter
    gb_bitmap_biltter_t             biltter;

    // the stroker
    gb_stroker_ref_t                stroker;

}gb_bitmap_device_t, *gb_bitmap_device_ref_t;

#endif


