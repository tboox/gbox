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
 * @file        glut.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_glut"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../canvas.h"
#ifdef TB_CONFIG_OS_MAC
# 	include <GLUT/glut.h>
#else
# 	define GL_GLEXT_PROTOTYPES
# 	include <GL/glut.h>
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the glut window impl type
typedef struct __gb_window_glut_impl_t
{
    // the base
    gb_window_impl_t        base;

    // the window id
    tb_int_t                id;

    // is stoped?
    tb_atomic_t             stop;

    // the canvas
    gb_canvas_ref_t         canvas;

}gb_window_glut_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the windows
static gb_window_glut_impl_t*   g_windows[16] = {tb_null};

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tb_inline__ gb_window_glut_impl_t* gb_window_glut_get()
{
    tb_int_t id = glutGetWindow();
    return (id < tb_arrayn(g_windows))? g_windows[id] : tb_null;
}
static tb_void_t gb_window_glut_display()
{
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && impl->canvas);

    // done init
    if (impl->base.info.init) impl->base.info.init((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);

	// flush
	glutSwapBuffers();
}
static tb_void_t gb_window_glut_idle()
{
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && impl->base.info.draw && impl->canvas);

    // done draw
    impl->base.info.draw((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);

	// flush
	glutSwapBuffers();
}
static tb_void_t gb_window_glut_reshape(tb_int_t width, tb_int_t height)
{
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // TODO
    // ...

    // done resize
    if (impl->base.info.resize) impl->base.info.resize((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);
}
static tb_void_t gb_window_glut_keyboard(tb_byte_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

}
static tb_void_t gb_window_glut_special(tb_int_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);
}
static tb_void_t gb_window_glut_mouse(tb_int_t button, tb_int_t state, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);
}
static tb_void_t gb_window_glut_passive_motion(tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);
}
static tb_void_t gb_window_glut_motion(tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);
}
static tb_void_t gb_window_glut_close()
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && impl->canvas);

    // done exit
    if (impl->base.info.exit) impl->base.info.exit((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);

	// flush
	glutSwapBuffers();

    // stop it
    tb_atomic_set(&impl->stop, 1);
}
static tb_void_t gb_window_glut_exit(gb_window_ref_t window)
{
    // check
    gb_window_glut_impl_t* impl = (gb_window_glut_impl_t*)window;
    tb_assert_and_check_return(impl && impl->id < tb_arrayn(g_windows));

    // stop it
    tb_atomic_set(&impl->stop, 1);

    // clear it
    g_windows[impl->id] = tb_null;

    // exit canvas
    if (impl->canvas) gb_canvas_exit(impl->canvas);
    impl->canvas = tb_null;

    // exit it
    tb_free(window);
}
static tb_void_t gb_window_glut_loop(gb_window_ref_t window)
{
    // check
    gb_window_glut_impl_t* impl = (gb_window_glut_impl_t*)window;
    tb_assert_and_check_return(impl);

    // init canvas
    if (!impl->canvas) impl->canvas = gb_canvas_init_from_window(window);
    tb_assert_abort(impl->canvas);

    // loop
    while (!tb_atomic_get(&impl->stop))
    {
        glutCheckLoop();
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init_glut(gb_window_info_t const* info, tb_size_t width, tb_size_t height)
{
    // done
    tb_bool_t               ok = tb_false;
    gb_window_glut_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(info);
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);

        // make window
        impl = tb_malloc0_type(gb_window_glut_impl_t);
        tb_assert_and_check_break(impl);

        // init base
        impl->base.type     = GB_WINDOW_TYPE_GLUT;
        impl->base.mode     = GB_WINDOW_MODE_GL;
        impl->base.width    = width;
        impl->base.height   = height;
        impl->base.loop     = gb_window_glut_loop;
        impl->base.exit     = gb_window_glut_exit;
        impl->base.info     = *info;

        /* init pixfmt
         * 
         * supports: 
         * - rgba8888_be
         * - rgb565_le
         * - rgb888_be
         * - rgba4444_be
         * - rgba5551_le
         */
        impl->base.pixfmt   = gb_quality() < GB_QUALITY_TOP? GB_PIXFMT_RGB565 : (GB_PIXFMT_RGBA8888 | GB_PIXFMT_BENDIAN);

        // init loop
        impl->stop = 0;

        // init glut
        tb_int_t    argc = 0;
        tb_char_t*  argv[1] = {0};
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(impl->base.width, impl->base.height);
        
        // make window
        impl->id = glutCreateWindow(impl->base.info.title? impl->base.info.title : "");
        tb_assert_and_check_break(impl->id < tb_arrayn(g_windows));

        // save window
        g_windows[impl->id] = impl;

        // init window func
        glutDisplayFunc(gb_window_glut_display);
        glutReshapeFunc(gb_window_glut_reshape);
        glutKeyboardFunc(gb_window_glut_keyboard);
        glutSpecialFunc(gb_window_glut_special);
        glutMouseFunc(gb_window_glut_mouse);
        glutPassiveMotionFunc(gb_window_glut_passive_motion);
        glutMotionFunc(gb_window_glut_motion);
        glutWMCloseFunc(gb_window_glut_close);
        glutIdleFunc(gb_window_glut_idle);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_window_exit((gb_window_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_window_ref_t)impl;
}
