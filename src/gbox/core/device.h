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
 *
 */
#ifndef GB_CORE_DEVICE_H
#define GB_CORE_DEVICE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../platform/window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the device type enum
typedef enum __gb_device_type_e
{
    GB_DEVICE_TYPE_NONE     = 0
,   GB_DEVICE_TYPE_GL       = 1
,   GB_DEVICE_TYPE_BITMAP   = 2
,   GB_DEVICE_TYPE_SKIA     = 3

}gb_device_type_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init device from window
 *
 * @param window    the window
 *
 * @return          the device
 */
gb_device_ref_t     gb_device_init(gb_window_ref_t window);

#ifdef GB_CONFIG_PACKAGE_HAVE_SKIA
/*! init skia device
 *
 * @param bitmap    the bitmap
 *
 * @return          the device
 */
gb_device_ref_t     gb_device_init_skia(gb_bitmap_ref_t bitmap);
#endif

#ifdef GB_CONFIG_DEVICE_HAVE_BITMAP
/*! init bitmap device
 *
 * @param bitmap    the bitmap
 *
 * @return          the device
 */
gb_device_ref_t     gb_device_init_bitmap(gb_bitmap_ref_t bitmap);
#endif

/*! exit device 
 *
 * @param device    the device
 */
tb_void_t           gb_device_exit(gb_device_ref_t device);

/*! the type 
 *
 * @param device    the device
 *
 * @return          the type
 */
tb_size_t           gb_device_type(gb_device_ref_t device);

/*! the pixfmt 
 *
 * @param device    the device
 *
 * @return          the pixfmt
 */
tb_size_t           gb_device_pixfmt(gb_device_ref_t device);

/*! the device width
 *
 * @param device    the device
 *
 * @return          the width
 */
tb_size_t           gb_device_width(gb_device_ref_t device);

/*! the device height
 *
 * @param device    the device
 *
 * @return          the height
 */
tb_size_t           gb_device_height(gb_device_ref_t device);

/*! resize the device
 *
 * @param device    the device
 * @param width     the width
 * @param height    the height
 */
tb_void_t           gb_device_resize(gb_device_ref_t device, tb_size_t width, tb_size_t height);

/*! bind paint
 *
 * @param device    the device
 * @param paint     the paint
 */
tb_void_t           gb_device_bind_paint(gb_device_ref_t device, gb_paint_ref_t paint);

/*! bind matrix
 *
 * @param device    the device
 * @param matrix    the matrix
 */
tb_void_t           gb_device_bind_matrix(gb_device_ref_t device, gb_matrix_ref_t matrix);

/*! bind clipper
 *
 * @param device    the device
 * @param clipper   the clipper
 */
tb_void_t           gb_device_bind_clipper(gb_device_ref_t device, gb_clipper_ref_t clipper);

/*! clear draw and fill the given color
 *
 * @param device    the device
 * @param color     the color
 */
tb_void_t           gb_device_draw_clear(gb_device_ref_t device, gb_color_t color);

/*! draw path
 *
 * @param device    the device
 * @param path      the path
 */
tb_void_t           gb_device_draw_path(gb_device_ref_t device, gb_path_ref_t path);

/*! draw lines
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count
 * @param bounds    the bounds
 */
tb_void_t           gb_device_draw_lines(gb_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/*! draw points
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count 
 * @param bounds    the bounds
 */
tb_void_t           gb_device_draw_points(gb_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/*! draw polygon
 *
 * @param device    the device
 * @param polygon   the polygon
 * @param hint      the hint shape
 * @param bounds    the bounds
 */
tb_void_t           gb_device_draw_polygon(gb_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
