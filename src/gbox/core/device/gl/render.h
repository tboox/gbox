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

/* init gl render
 *
 * @param device    the device
 * @param matrix    the matrix
 * @param paint     the paint
 * @param clipper   the clipper
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_gl_render_init(gb_gl_device_ref_t device, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper);

/* fill gl render
 *
 * @param device    the device
 * @param points    the points
 * @param counts    the counts
 */
tb_void_t           gb_gl_render_fill(gb_gl_device_ref_t device, gb_point_t const* points, tb_size_t const* counts);

/* stok gl render
 *
 * @param device    the device
 * @param points    the points
 * @param counts    the counts
 */
tb_void_t           gb_gl_render_stok(gb_gl_device_ref_t device, gb_point_t const* points, tb_size_t const* counts);

/* exit gl render
 *
 * @param device    the device
 */
tb_void_t           gb_gl_render_exit(gb_gl_device_ref_t device);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
