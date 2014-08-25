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
 * types
 */

// the gl device impl type
typedef struct __gb_device_gl_impl_t
{
    // the base
    gb_device_impl_t            base;

    // the window
    gb_window_ref_t             window;

    // the version: 1.0, 2.x, ...
    tb_size_t                   version;

}gb_device_gl_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#ifdef GB_CONFIG_THIRD_HAVE_GL
__tb_extern_c__ gb_device_ref_t gb_device_init_gl(gb_window_ref_t window);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_size_t gb_device_gl_version()
{
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
}
static tb_void_t gb_device_gl_resize(gb_device_ref_t device, tb_size_t width, tb_size_t height)
{
}
static tb_void_t gb_device_gl_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // clear it
	gb_glClearColor((tb_float_t)color.r / 0xff, (tb_float_t)color.g / 0xff, (tb_float_t)color.b / 0xff, (tb_float_t)color.a / 0xff);
	gb_glClear(GB_GL_COLOR_BUFFER_BIT);
}
static tb_void_t gb_device_gl_exit(gb_device_ref_t device)
{
    // exit it
    if (device) tb_free(device);
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
    gb_device_gl_impl_t*    impl = tb_null;
    do
    {
        // make device
        impl = tb_malloc0_type(gb_device_gl_impl_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_GL;
        impl->base.pixfmt           = (tb_uint16_t)gb_window_pixfmt(window); 
        impl->base.width            = (tb_uint16_t)gb_window_width(window); 
        impl->base.height           = (tb_uint16_t)gb_window_height(window); 
        impl->base.resize           = gb_device_gl_resize;
        impl->base.draw_clear       = gb_device_gl_draw_clear;
        impl->base.exit             = gb_device_gl_exit;

        // init window
        impl->window                = window;

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
            impl->version = 1;
#endif
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
