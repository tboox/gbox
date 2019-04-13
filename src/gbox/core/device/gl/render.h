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
 * @file        render.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_GL_RENDER_H
#define GB_CORE_DEVICE_GL_RENDER_H

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
tb_bool_t           gb_gl_render_init(gb_gl_device_ref_t device);

/* exit render
 *
 * @param device    the device
 */
tb_void_t           gb_gl_render_exit(gb_gl_device_ref_t device);

/* draw path
 *
 * @param device    the device
 * @param path      the path
 */
tb_void_t           gb_gl_render_draw_path(gb_gl_device_ref_t device, gb_path_ref_t path);

/* draw lines
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count
 * @param bounds    the bounds
 */
tb_void_t           gb_gl_render_draw_lines(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/* draw points
 *
 * @param device    the device
 * @param points    the points
 * @param count     the points count
 * @param bounds    the bounds
 */
tb_void_t           gb_gl_render_draw_points(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);

/* draw polygon
 *
 * @param device    the device
 * @param polygon   the polygon
 * @param hint      the hint shape
 * @param bounds    the bounds
 */
tb_void_t           gb_gl_render_draw_polygon(gb_gl_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
