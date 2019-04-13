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
#include "../../../utils/tessellator.h"

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

    // the stroker
    gb_stroker_ref_t            stroker;

    // the program
    gb_gl_program_ref_t         program;

    // the tessellator
    gb_tessellator_ref_t        tessellator;

}gb_gl_device_t, *gb_gl_device_ref_t;

#endif


