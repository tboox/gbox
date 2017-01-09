/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        polygon.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_polygon"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "lines.h"
#include "polygon.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_bitmap_render_fill_raster(tb_long_t lx, tb_long_t rx, tb_long_t yb, tb_long_t ye, tb_cpointer_t priv)
{
    // check
    tb_assert(priv && rx >= lx && ye > yb);

    // done biltter
    gb_bitmap_biltter_done_r((gb_bitmap_biltter_ref_t)priv, lx, yb, rx - lx, ye - yb);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_fill_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint);

    // done raster
    gb_polygon_raster_done(device->raster, polygon, bounds, gb_paint_fill_rule(device->base.paint), gb_bitmap_render_fill_raster, &device->biltter);
}
tb_void_t gb_bitmap_render_stroke_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon)
{
    // check
    tb_assert(device && polygon && polygon->points && polygon->counts);

    // done
    tb_uint16_t     index = 0;
    gb_point_t      points_line[2];
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    while (index < count)
    {
        // the point
        points_line[1] = *points++;

        // stroke line
        if (index) gb_bitmap_render_stroke_lines(device, points_line, 2);

        // save the previous point
        points_line[0] = points_line[1];
        
        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }
}
