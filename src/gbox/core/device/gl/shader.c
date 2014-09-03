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
 * @file        shader.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_shader"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "shader.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_shader_ref_t gb_gl_shader_init_linear(gb_gl_device_ref_t device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    return tb_null;
}
gb_shader_ref_t gb_gl_shader_init_radial(gb_gl_device_ref_t device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    return tb_null;
}
gb_shader_ref_t gb_gl_shader_init_bitmap(gb_gl_device_ref_t device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    return tb_null;
}
