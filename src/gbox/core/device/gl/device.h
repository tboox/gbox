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
 * @file        device.h
 * @ingroup     core
 */
#ifndef GB_CORE_DEVICE_GL_DEVICE_H
#define GB_CORE_DEVICE_GL_DEVICE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "interface.h"
#include "program.h"
#include "matrix.h"
#include "../../impl/stroker.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl device type
typedef struct __gb_gl_device_t
{
    // the base
    gb_device_impl_t            base;

    // the window
    gb_window_ref_t             window;

    // the version: 1.0, 2.x, ...
    tb_size_t                   version;

    // the programs
    gb_gl_program_ref_t         programs[GB_GL_PROGRAM_LOCATION_MAXN];

	// the projection matrix for gl >= 2.0
	gb_gl_matrix_t              matrix_project;

	// the vertex matrix 
	gb_gl_matrix_t              matrix_vertex;

    // the shader
    gb_shader_ref_t             shader;

    // the program
    gb_gl_program_ref_t         program;

    // the stroker
    gb_stroker_ref_t            stroker;

}gb_gl_device_t, *gb_gl_device_ref_t;

#endif


