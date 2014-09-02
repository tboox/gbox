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
 * @file        program.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_program"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "program.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl program impl type
typedef struct __gb_gl_program_impl_t
{
    // the type
    tb_size_t           type;

    // the program
    gb_GLuint_t         program;

    // the vertex shader
    gb_GLuint_t         vshader;

    // the fragment shader
    gb_GLuint_t         fshader;

    // the locations
    gb_GLint_t          location[GB_GL_PROGRAM_LOCATION_MAXN];

}gb_gl_program_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static gb_GLuint_t gb_gl_program_shader(tb_char_t const* data, tb_size_t type)
{
    // init shader
    gb_GLuint_t shader = gb_glCreateShader((gb_GLenum_t)type);
    tb_assert_and_check_return_val(shader, 0);

    // init source
    gb_glShaderSource(shader, 1, (gb_GLchar_t const**)&data, tb_null);

    // compile shader
    gb_glCompileShader(shader);

    // ok?
    gb_GLint_t ok = GB_GL_FALSE;
    gb_glGetShaderiv(shader, GB_GL_COMPILE_STATUS, &ok);
    if (ok == GB_GL_FALSE)
    {
        // get info
        gb_GLchar_t info[1024] = {0};
        gb_glGetProgramInfoLog(shader, sizeof(info), 0, info);

        // trace
        tb_trace_e("shader: compile failed: %s", info);

        // exit it
        gb_glDeleteShader(shader);
        return 0;
    }

    // ok
    return shader;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_gl_program_ref_t gb_gl_program_init(tb_size_t type, tb_char_t const* vshader, tb_char_t const* fshader)
{
    // check
    tb_assert_and_check_return_val(type && vshader && fshader, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_gl_program_impl_t*   impl = tb_null;
    do
    {
        // make program
        impl = tb_malloc0_type(gb_gl_program_impl_t);
        tb_assert_and_check_break(impl);

        // init program
        impl->program = gb_glCreateProgram();
        tb_assert_and_check_break(impl->program);

        // init type
        impl->type = type;

        // load vshader
        impl->vshader = gb_gl_program_shader(vshader, GB_GL_VERTEX_SHADER);
        tb_assert_and_check_break(impl->vshader);

        // load fshader
        impl->fshader = gb_gl_program_shader(fshader, GB_GL_FRAGMENT_SHADER);
        tb_assert_and_check_break(impl->fshader);

        // bind vshader
        gb_glAttachShader(impl->program, impl->vshader);

        // bind fshader
        gb_glAttachShader(impl->program, impl->fshader);

        // link program
        gb_glLinkProgram(impl->program);

        // get status
        gb_GLint_t status = GB_GL_FALSE;
        gb_glGetProgramiv(impl->program, GB_GL_LINK_STATUS, &status);
        if (status == GB_GL_FALSE) 
        {
            // get info
            gb_GLchar_t info[1024] = {0};
            gb_glGetProgramInfoLog(impl->program, sizeof(info), 0, info);

            // trace
            tb_trace_e("link failed: %s", info);
            break;
        }

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_gl_program_exit((gb_gl_program_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_gl_program_ref_t)impl;
}
tb_void_t gb_gl_program_exit(gb_gl_program_ref_t program)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return(impl);

    // exit vshader
    if (impl->vshader) gb_glDeleteShader(impl->vshader);
    impl->vshader = 0;

    // exit fshader
    if (impl->fshader) gb_glDeleteShader(impl->fshader);
    impl->fshader = 0;

    // exit impl
    if (impl->program) gb_glDeleteProgram(impl->program);
    impl->program = 0;

    // free it
    tb_free(impl);
}
tb_void_t gb_gl_program_bind(gb_gl_program_ref_t program)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return(impl && impl->program);

    // bind it
    gb_glUseProgram(impl->program);
}
gb_GLint_t gb_gl_program_attr(gb_gl_program_ref_t program, tb_char_t const* name)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return_val(impl && impl->program && name, -1);

    // the location
    return gb_glGetAttribLocation(impl->program, name);
}
gb_GLint_t gb_gl_program_unif(gb_gl_program_ref_t program, tb_char_t const* name)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return_val(impl && impl->program && name, -1);

    // the location
    return gb_glGetUniformLocation(impl->program, name);
}
gb_GLint_t gb_gl_program_location(gb_gl_program_ref_t program, tb_size_t id)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return_val(impl && id < GB_GL_PROGRAM_LOCATION_MAXN, 0);

    // the location
    return impl->location[id];
}
tb_void_t gb_gl_program_location_set(gb_gl_program_ref_t program, tb_size_t id, gb_GLint_t location)
{
    // check
    gb_gl_program_impl_t* impl = (gb_gl_program_impl_t*)program;
    tb_assert_and_check_return(impl && id < GB_GL_PROGRAM_LOCATION_MAXN);

    // save location
    impl->location[id] = location;
}

