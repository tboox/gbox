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
 * @file        program.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_GL_PROGRAM_H
#define GB_CORE_DEVICE_GL_PROGRAM_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl program type enum
typedef enum __gb_gl_program_type_e
{
    GB_GL_PROGRAM_TYPE_NONE         = 0
,   GB_GL_PROGRAM_TYPE_COLOR        = 1
,   GB_GL_PROGRAM_TYPE_BITMAP       = 2
,   GB_GL_PROGRAM_TYPE_MAXN         = 3

}gb_gl_program_type_e;

// the gl program location id enum
typedef enum __gb_gl_program_location_e
{
    GB_GL_PROGRAM_LOCATION_COLORS               = 0
,   GB_GL_PROGRAM_LOCATION_VERTICES             = 1
,   GB_GL_PROGRAM_LOCATION_TEXCOORDS            = 2
,   GB_GL_PROGRAM_LOCATION_SAMPLER              = 3
,   GB_GL_PROGRAM_LOCATION_MATRIX_MODEL         = 4
,   GB_GL_PROGRAM_LOCATION_MATRIX_PROJECT       = 5
,   GB_GL_PROGRAM_LOCATION_MATRIX_TEXCOORD      = 6
,   GB_GL_PROGRAM_LOCATION_MAXN                 = 7

}gb_gl_program_location_e;

// the gl program ref type
typedef struct{}*   gb_gl_program_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init gl program for gl >= 2.x
 *
 * @param type          the program type
 * @param vshader       the vertex shader content
 * @param fshader       the fragment shader content
 *
 * @return              the program
 */
gb_gl_program_ref_t     gb_gl_program_init(tb_size_t type, tb_char_t const* vshader, tb_char_t const* fshader);

/* init color program
 *
 * @return              the program
 */
gb_gl_program_ref_t     gb_gl_program_init_color(tb_noarg_t);

/* init bitmap program
 *
 * @return              the program
 */
gb_gl_program_ref_t     gb_gl_program_init_bitmap(tb_noarg_t);

/* exit gl program
 *
 * @param program       the program
 */
tb_void_t               gb_gl_program_exit(gb_gl_program_ref_t program);

/* bind the program to the current gl context
 *
 * @param program       the program
 */
tb_void_t               gb_gl_program_bind(gb_gl_program_ref_t program);

/* the attribute location of the given name for the program
 *
 * @param program       the program
 * @param name          the attribute name
 *
 * @return              the attribute location 
 */
gb_GLint_t              gb_gl_program_attr(gb_gl_program_ref_t program, tb_char_t const* name);

/* the uniform location of the given name for the program
 *
 * @param program       the program
 * @param name          the uniform name
 *
 * @return              the uniform location 
 */
gb_GLint_t              gb_gl_program_unif(gb_gl_program_ref_t program, tb_char_t const* name);

/* the program location for the given id
 *
 * @param program       the program
 * @param index         the location id
 *
 * @return              the program type
 */
gb_GLint_t              gb_gl_program_location(gb_gl_program_ref_t program, tb_size_t id);

/* set the program location from the given id
 *
 * @param program       the program
 * @param index         the location id
 * @param location      the location
 *
 */
tb_void_t               gb_gl_program_location_set(gb_gl_program_ref_t program, tb_size_t id, gb_GLint_t location);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
