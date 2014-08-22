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
#ifdef TB_CONFIG_OS_MAC
# 	include <GLUT/glut.h>
#else
# 	define GL_GLEXT_PROTOTYPES
# 	include <GL/glut.h>
#endif

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

}gb_device_gl_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_gl_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // clear it
	glClearColor((tb_float_t)color.r / 0xff, (tb_float_t)color.g / 0xff, (tb_float_t)color.b / 0xff, (tb_float_t)color.a / 0xff);
	glClear(GL_COLOR_BUFFER_BIT);
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
        impl->base.pixfmt           = gb_window_pixfmt(window); 
        impl->base.draw_clear       = gb_device_gl_draw_clear;
        impl->base.exit             = gb_device_gl_exit;

        // init window
        impl->window          = window;

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
