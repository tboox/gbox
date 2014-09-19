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
 * @file        render.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_BITMAP_RENDER_H
#define GB_CORE_DEVICE_BITMAP_RENDER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "device.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init render
 *
 * @param device    the device
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_bitmap_render_init(gb_bitmap_device_ref_t device);

/* exit render
 *
 * @param device    the device
 */
tb_void_t           gb_bitmap_render_exit(gb_bitmap_device_ref_t device);

/* draw lines
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count
 * @param bounds    the bounds
 */
tb_void_t           gb_bitmap_render_draw_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/* draw points
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count
 * @param bounds    the bounds
 */
tb_void_t           gb_bitmap_render_draw_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/* draw polygon
 *
 * @param device    the device
 * @param polygon   the polygon
 * @param hint      the hint shape
 * @param bounds    the bounds
 */
tb_void_t           gb_bitmap_render_draw_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
