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
#include "../device.h"
#include "../bitmap.h"
#include "../pixmap.h"
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

    // the paint
    gb_paint_ref_t          paint;

    // the matrix
    gb_matrix_ref_t         matrix;

    // the clipper
    gb_clipper_ref_t        clipper;

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
	
    /*! draw path
     *
     * @param device        the device
     * @param path          the path
     */
    tb_void_t               (*draw_path)(struct __gb_device_impl_t* device, gb_path_ref_t path);

    /*! draw lines 
     *
     * @param device        the device
     * @param points        the points
     * @param count         the count
     * @param bounds        the bounds
     */
    tb_void_t               (*draw_lines)(struct __gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);
	
    /*! draw points 
     *
     * @param device        the device
     * @param points        the points
     * @param count         the count
     * @param bounds        the bounds
     */
    tb_void_t               (*draw_points)(struct __gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds);
	
    /*! draw polygon
     *
     * @param device        the device
     * @param polygon       the polygon
     * @param hint          the hint shape
     * @param bounds        the bounds
     */
    tb_void_t               (*draw_polygon)(struct __gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds);

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


