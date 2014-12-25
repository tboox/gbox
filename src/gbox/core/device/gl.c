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
 * @file        gl.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device_gl"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "gl/gl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// testing gl v1 interfaces
//#define GB_DEVICE_GL_TEST_v1

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#ifdef GB_CONFIG_PACKAGE_HAVE_OPENGL
__tb_extern_c__ gb_device_ref_t gb_device_init_gl(gb_window_ref_t window);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_size_t gb_device_gl_version()
{
#ifdef TB_CONFIG_OS_WINDOWS
    // only supports gl 1.1 for windows
    return 0x11;
#else
	// version 
	tb_char_t const* version = (tb_char_t const*)gb_glGetString(GB_GL_VERSION);
	tb_assert_and_check_return_val(version, 0);

	// find
	tb_char_t const* p = version;
	tb_char_t const* e = version + tb_strlen(version);
	for (; p < e && *p && !tb_isdigit(*p); p++) ;
	tb_assert_and_check_return_val(p + 2 < e && p[1] == '.' && tb_isdigit(p[2]), 0);

	// major & minor
	tb_byte_t major = p[0] - '0';
	tb_byte_t minor = p[2] - '0';

	// trace
	tb_trace_d("version: %s: %x", version, ((major << 4) + minor));

	// [0x10, 0x19] 
	return ((major << 4) + minor);
#endif
}
static tb_void_t gb_device_gl_resize(gb_device_impl_t* device, tb_size_t width, tb_size_t height)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl);

	// update viewport
	gb_glViewport(0, 0, width, height);

	// update matrix	
	if (impl->version >= 0x20) 
    {
        // update the projection matrix
        gb_gl_matrix_orthof(impl->matrix_project, 0.0f, (gb_GLfloat_t)width, (gb_GLfloat_t)height, 0.0f, -1.0f, 1.0f);
    }
    else
	{
		// update the projection matrix
		gb_glMatrixMode(GB_GL_PROJECTION);
		gb_glLoadIdentity();
		gb_glOrthof(0.0f, (gb_GLfloat_t)width, (gb_GLfloat_t)height, 0.0f, -1.0f, 1.0f);

		// update the model matrix
		gb_glMatrixMode(GB_GL_MODELVIEW);
		gb_glLoadIdentity();
	}
}
static tb_void_t gb_device_gl_draw_clear(gb_device_impl_t* device, gb_color_t color)
{
    // clear it
	gb_glClearColor((gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
	gb_glClear(GB_GL_COLOR_BUFFER_BIT);
}
static tb_void_t gb_device_gl_draw_path(gb_device_impl_t* device, gb_path_ref_t path)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl && path);

    // init render
    if (gb_gl_render_init(impl))
    {
        // draw path
        gb_gl_render_draw_path(impl, path);
    
        // exit render
        gb_gl_render_exit(impl);
    }
}
static tb_void_t gb_device_gl_draw_lines(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl && points && count);

    // init render
    if (gb_gl_render_init(impl))
    {
        // draw lines
        gb_gl_render_draw_lines(impl, points, count, bounds);
    
        // exit render
        gb_gl_render_exit(impl);
    }
}
static tb_void_t gb_device_gl_draw_points(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl && points && count);

    // init render
    if (gb_gl_render_init(impl))
    {
        // draw points
        gb_gl_render_draw_points(impl, points, count, bounds);
    
        // exit render
        gb_gl_render_exit(impl);
    }
}
static tb_void_t gb_device_gl_draw_polygon(gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl && polygon);

    // init render
    if (gb_gl_render_init(impl))
    {
        // draw polygon
        gb_gl_render_draw_polygon(impl, polygon, hint, bounds);
    
        // exit render
        gb_gl_render_exit(impl);
    }
}
static gb_shader_ref_t gb_device_gl_shader_linear(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // init shader
    return gb_gl_shader_init_linear(impl, mode, gradient, line);
}
static gb_shader_ref_t gb_device_gl_shader_radial(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // init shader
    return gb_gl_shader_init_radial(impl, mode, gradient, circle);
}
static gb_shader_ref_t gb_device_gl_shader_bitmap(gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // init shader
    return gb_gl_shader_init_bitmap(impl, mode, bitmap);
}
static tb_void_t gb_device_gl_exit(gb_device_impl_t* device)
{
    // check
    gb_gl_device_ref_t impl = (gb_gl_device_ref_t)device;
    tb_assert_and_check_return(impl);
    
    // exit stroker
    if (impl->stroker) gb_stroker_exit(impl->stroker);
    impl->stroker = tb_null;
 
    // exit programs 
    tb_size_t i = 0;
    for (i = 0; i < GB_GL_PROGRAM_TYPE_MAXN; i++)
    {
        if (impl->programs[i]) gb_gl_program_exit(impl->programs[i]);
        impl->programs[i] = 0;
    }

    // exit it
    if (impl) tb_free(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init_gl(gb_window_ref_t window)
{
    // check
    tb_assert_and_check_return_val(window, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_gl_device_ref_t      impl = tb_null;
    do
    {
        // the width and height
        tb_size_t width     = gb_window_width(window);
        tb_size_t height    = gb_window_height(window);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make device
        impl = tb_malloc0_type(gb_gl_device_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_GL;
        impl->base.resize           = gb_device_gl_resize;
        impl->base.draw_clear       = gb_device_gl_draw_clear;
        impl->base.draw_path        = gb_device_gl_draw_path;
        impl->base.draw_lines       = gb_device_gl_draw_lines;
        impl->base.draw_points      = gb_device_gl_draw_points;
        impl->base.draw_polygon     = gb_device_gl_draw_polygon;
        impl->base.shader_linear    = gb_device_gl_shader_linear;
        impl->base.shader_radial    = gb_device_gl_shader_radial;
        impl->base.shader_bitmap    = gb_device_gl_shader_bitmap;
        impl->base.exit             = gb_device_gl_exit;

        // init window
        impl->window                = window;

        // init stroker
        impl->stroker = gb_stroker_init();
        tb_assert_and_check_break(impl->stroker);

        // init version 
        if (!impl->version)
        {
            // load gl interfaces first
            if (!gb_gl_interface_load()) break;

            // get version
            impl->version = gb_device_gl_version();
            tb_assert_and_check_break(impl->version && impl->version < 0xff);

            // force setting version: v1 for testing
#ifdef GB_DEVICE_GL_TEST_v1
            impl->version = 0x10;
#endif

#if 0
            // dump extensions
            tb_trace_d("extensions: %s", gb_glGetString(GB_GL_EXTENSIONS));
#endif
        }

        // init viewport
        gb_glViewport(0, 0, width, height);

        // init gl >= 2.0
        if (impl->version >= 0x20)
        {
            // init color program
            impl->programs[GB_GL_PROGRAM_TYPE_COLOR] = gb_gl_program_init_color();
            tb_assert_and_check_break(impl->programs[GB_GL_PROGRAM_TYPE_COLOR]);

            // init bitmap program
            impl->programs[GB_GL_PROGRAM_TYPE_BITMAP] = gb_gl_program_init_bitmap();
            tb_assert_and_check_break(impl->programs[GB_GL_PROGRAM_TYPE_BITMAP]);

            // init the projection matrix
            gb_gl_matrix_orthof(impl->matrix_project, 0.0f, (gb_GLfloat_t)width, (gb_GLfloat_t)height, 0.0f, -1.0f, 1.0f);
        }
        // init gl 1.x
        else
        {
            // init the projection matrix
            gb_glMatrixMode(GB_GL_PROJECTION);
            gb_glLoadIdentity();
            gb_glOrthof(0.0f, (gb_GLfloat_t)width, (gb_GLfloat_t)height, 0.0f, -1.0f, 1.0f);

            // init the model matrix
            gb_glMatrixMode(GB_GL_MODELVIEW);
            gb_glLoadIdentity();
        }

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_device_exit((gb_device_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_device_ref_t)impl;
}
