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
 * @file        shader.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_GL_SHADER_H
#define GB_CORE_DEVICE_GL_SHADER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "device.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */
	
/*! init gl linear gradient shader
 *
 * @param device    the device
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param line      the line
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_gl_shader_init_linear(gb_gl_device_ref_t device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line);

/*! init gl radial gradient shader
 *
 * @param device    the device
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param circle    the circle
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_gl_shader_init_radial(gb_gl_device_ref_t device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle);

/*! init gl bitmap shader
 *
 * @param device    the device
 * @param mode      the mode 
 * @param bitmap    the bitmap
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_gl_shader_init_bitmap(gb_gl_device_ref_t device, tb_size_t mode, gb_bitmap_ref_t bitmap);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
