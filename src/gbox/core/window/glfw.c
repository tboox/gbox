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
 * @file        glfw.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_glfw"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <glfw3.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the glfw window impl type
typedef struct __gb_window_glfw_impl_t
{
    // the base
    gb_window_impl_t        base;

    // the loop
    tb_thread_ref_t         loop;

    // is stoped?
    tb_atomic_t             stop;

}gb_window_glfw_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_window_glfw_error(tb_int_t code, tb_char_t const* info)
{
    // trace
    tb_trace_e("error: code: %d, info: %s", code, info);
}
static tb_void_t gb_window_glfw_mouse_button(GLFWwindow* window, tb_int_t button, tb_int_t action, tb_int_t modify)
{
    tb_trace_i("gb_window_glfw_mouse_button");
}
static tb_void_t gb_window_glfw_mouse_move(GLFWwindow* window, tb_double_t x, tb_double_t y)
{
    tb_trace_i("gb_window_glfw_mouse_move");
}
static tb_void_t gb_window_glfw_scroll(GLFWwindow* window, tb_double_t x, tb_double_t y)
{
    tb_trace_i("gb_window_glfw_scroll");
}
static tb_void_t gb_window_glfw_key(GLFWwindow* window, tb_int_t key, tb_int_t scancode, tb_int_t action, tb_int_t mods)
{
    tb_trace_i("gb_window_glfw_key");
}
static tb_void_t gb_window_glfw_char(GLFWwindow* window, tb_uint_t character)
{
    tb_trace_i("gb_window_glfw_char");
}
static tb_void_t gb_window_glfw_window_pos(GLFWwindow* windows, tb_int_t x, tb_int_t y)
{
    tb_trace_i("gb_window_glfw_window_pos");
}
static tb_void_t gb_window_glfw_framebuffer_size(GLFWwindow* window, tb_int_t w, tb_int_t h)
{
    tb_trace_i("gb_window_glfw_framebuffer_size");
}
static tb_void_t gb_window_glfw_window_size(GLFWwindow *window, tb_int_t width, tb_int_t height)
{
    tb_trace_i("gb_window_glfw_window_size");
}
static tb_void_t gb_window_glfw_exit(gb_window_ref_t window)
{
    // check
    gb_window_glfw_impl_t* impl = (gb_window_glfw_impl_t*)window;
    tb_assert_and_check_return(impl);

    // stop it
    tb_atomic_set(&impl->stop, 1);

    // exit loop
    if (impl->loop)
    {
        // wait loop
        tb_thread_wait(impl->loop, -1);
        tb_thread_exit(impl->loop);
        impl->loop = tb_null;
    }

    // exit it
    tb_free(window);
}
static tb_pointer_t gb_window_glfw_loop(tb_cpointer_t priv)
{
    // done
    GLFWwindow*     window = tb_null;
    GLFWmonitor*    monitor = tb_null;
    do
    {
        // check
        gb_window_glfw_impl_t* impl = (gb_window_glfw_impl_t*)priv;
        tb_assert_and_check_break(impl);

        // init glfw error func
        glfwSetErrorCallback(gb_window_glfw_error);

        // init glfw
        if (!glfwInit()) break;

        // can resize
//        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // make window
        window = glfwCreateWindow(impl->base.width, impl->base.height, impl->base.info.title? impl->base.info.title : "", monitor, tb_null);
        tb_assert_and_check_break(window);

        // draw it for this loop 
        glfwMakeContextCurrent(window);

        // init mouse button func
        glfwSetMouseButtonCallback(window, gb_window_glfw_mouse_button);

        // init mouse move func
        glfwSetCursorPosCallback(window, gb_window_glfw_mouse_move);

        // init scroll func
        glfwSetScrollCallback(window, gb_window_glfw_scroll);

        // init char func
        glfwSetCharCallback(window, gb_window_glfw_char);

        // init key func
        glfwSetKeyCallback(window, gb_window_glfw_key);

        // init window pos func
        glfwSetWindowPosCallback(window, gb_window_glfw_window_pos);

        // init framebuffer size func
        glfwSetFramebufferSizeCallback(window, gb_window_glfw_framebuffer_size);

        // init window size func
        glfwSetWindowSizeCallback(window, gb_window_glfw_window_size);

        // loop
        while (!tb_atomic_get(&impl->stop))
        {
            // clear
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);

            // poll events
            glfwPollEvents();

            // flush
            glfwSwapBuffers(window);
        }

    } while (0);

    // clos window
    if (window) glfwSetWindowShouldClose(window, 1);
    window = tb_null;

    // end
    glfwTerminate();
    tb_thread_return(tb_null);
    return tb_null;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init_glfw(gb_window_info_t const* info, tb_size_t width, tb_size_t height)
{
    // done
    tb_bool_t               ok = tb_false;
    gb_window_glfw_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(info);
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);

        // make window
        impl = tb_malloc0_type(gb_window_glfw_impl_t);
        tb_assert_and_check_break(impl);

        // init base
        impl->base.type     = GB_WINDOW_TYPE_GLUT;
        impl->base.mode     = GB_WINDOW_MODE_GL;
        impl->base.width    = width;
        impl->base.height   = height;
        impl->base.exit     = gb_window_glfw_exit;
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

gb_window_glfw_loop((tb_cpointer_t)impl);
        // init loop
        impl->stop = 0;
        impl->loop = tb_thread_init(__tb_lstring__("glfw_loop"), gb_window_glfw_loop, (tb_cpointer_t)impl, 0);
        tb_assert_and_check_break(impl->loop);

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
