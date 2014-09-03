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
 * @file        prefix.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_PREFIX_H
#define GB_CORE_DEVICE_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../path.h"
#include "../paint.h"
#include "../shader.h"
#include "../matrix.h"
#include "../device.h"
#include "../../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the device impl type
typedef struct __gb_device_impl_t
{
    // the device type
    tb_uint8_t              type;

    // the pixfmt
    tb_uint16_t             pixfmt;

    // the width
    tb_uint16_t             width;

    // the height
    tb_uint16_t             height;

    // the path
    gb_path_ref_t           path;

    /* resize
     *
     * @param device        the device
     * @param width         the width
     * @param height        the height
     */
    tb_void_t               (*resize)(struct __gb_device_impl_t* device, tb_size_t width, tb_size_t height);

    /* clear draw
     *
     * @param device        the device
     * @param color         the color
     */
    tb_void_t               (*draw_clear)(struct __gb_device_impl_t* device, gb_color_t color);

    /*! fill polygon
     *
     * @param device        the device
     * @param polygon       the polygon
     */
    tb_void_t               (*fill_polygon)(struct __gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper);

    /*! stroke segment with the one pixel width 
     *
     * @param device        the device
     * @param segment       the segment
     */
    tb_void_t               (*stok_segment)(struct __gb_device_impl_t* device, gb_segment_ref_t segment, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper);
	
    /*! init linear gradient shader
     *
     * @param device        the device
     * @param mode          the mode 
     * @param gradient      the gradient
     * @param line          the line
     *
     * @return              the shader
     */
    gb_shader_ref_t         (*shader_linear)(struct __gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line);

    /*! init radial gradient shader
     *
     * @param device        the device
     * @param mode          the mode 
     * @param gradient      the gradient
     * @param circle        the circle
     *
     * @return              the shader
     */
    gb_shader_ref_t         (*shader_radial)(struct __gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle);

    /*! init bitmap shader
     *
     * @param device        the device
     * @param mode          the mode 
     * @param bitmap        the bitmap
     *
     * @return              the shader
     */
    gb_shader_ref_t         (*shader_bitmap)(struct __gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap);

    /* exit device
     *
     * @param device        the device
     */
    tb_void_t               (*exit)(struct __gb_device_impl_t* device);

}gb_device_impl_t;

// the shader impl type
typedef struct __gb_shader_impl_t
{
    // the shader type
    tb_uint8_t              type;

    // the shader mode
    tb_uint8_t              mode;

    // the reference count
    tb_size_t               refn;

    // the matrix
    gb_matrix_t             matrix;

    /* exit shader
     *
     * @param shader        the shader
     */
    tb_void_t               (*exit)(struct __gb_shader_impl_t* shader);

}gb_shader_impl_t;

#endif


