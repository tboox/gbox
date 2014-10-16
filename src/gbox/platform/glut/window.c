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
 * @file        window.c
 * @ingroup     platform
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_glut"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../impl/window.h"
#ifdef TB_CONFIG_OS_MAC
# 	include <GLUT/glut.h>
#else
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

    // the delay for framerate
    tb_size_t               delay;

    // the spak time
    tb_hong_t               time;

    // the button
    tb_size_t               button;

    // the normal width before fullscreen
    tb_uint16_t             normal_width;

    // the normal height before fullscreen
    tb_uint16_t             normal_height;

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

    // spak
    impl->time = gb_window_impl_spak((gb_window_ref_t)impl);

    // draw
    gb_window_impl_draw((gb_window_ref_t)impl, impl->canvas);

	// flush
	glutSwapBuffers();

    // compute the spak time
    impl->time = tb_cache_time_spak() - impl->time;
}
static tb_void_t gb_window_glut_reshape(tb_int_t width, tb_int_t height)
{
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);
    tb_check_return(width && height);

    // trace
    tb_trace_d("reshape: %dx%d", width, height);

    // the device
    gb_device_ref_t device = gb_canvas_device(impl->canvas);
    tb_assert_and_check_return(device);

    // update the window width and height
    impl->base.width   = width;
    impl->base.height  = height;

    // resize the device
    gb_device_resize(device, width, height);

    // done resize
    if (impl->base.info.resize) impl->base.info.resize((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);
}
static tb_void_t gb_window_glut_keyboard(tb_byte_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_d("keyboard: %d at: %d, %d", key, x, y);

    // init event
    gb_event_t event = {0};
    event.type                  = GB_EVENT_TYPE_KEYBOARD;
    event.u.keyboard.code       = key;
    event.u.keyboard.pressed    = tb_true;

    // done event
    if (event.u.keyboard.code) gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_void_t gb_window_glut_keyboard_up(tb_byte_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_d("keyboard: %d at: %d, %d", key, x, y);

    // init event
    gb_event_t event = {0};
    event.type                  = GB_EVENT_TYPE_KEYBOARD;
    event.u.keyboard.code       = key;
    event.u.keyboard.pressed    = tb_false;

    // done event
    if (event.u.keyboard.code) gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_uint16_t gb_window_glut_special_map(tb_int_t key)
{
    // done
    tb_uint16_t code = GB_KEY_NUL;
    switch (key)
    {
    case GLUT_KEY_F1:           code = GB_KEY_F1;           break;
    case GLUT_KEY_F2:           code = GB_KEY_F2;           break;
    case GLUT_KEY_F3:           code = GB_KEY_F3;           break;
    case GLUT_KEY_F4:           code = GB_KEY_F4;           break;
    case GLUT_KEY_F5:           code = GB_KEY_F5;           break;
    case GLUT_KEY_F6:           code = GB_KEY_F6;           break;
    case GLUT_KEY_F7:           code = GB_KEY_F7;           break;
    case GLUT_KEY_F8:           code = GB_KEY_F8;           break;
    case GLUT_KEY_F9:           code = GB_KEY_F9;           break;
    case GLUT_KEY_F10:          code = GB_KEY_F10;          break;
    case GLUT_KEY_F11:          code = GB_KEY_F11;          break;
    case GLUT_KEY_F12:          code = GB_KEY_F12;          break;

    case GLUT_KEY_LEFT:         code = GB_KEY_LEFT;         break;
    case GLUT_KEY_UP:           code = GB_KEY_UP;           break;
    case GLUT_KEY_RIGHT:        code = GB_KEY_RIGHT;        break;
    case GLUT_KEY_DOWN:         code = GB_KEY_DOWN;         break;

    case GLUT_KEY_HOME:         code = GB_KEY_HOME;         break;
    case GLUT_KEY_END:          code = GB_KEY_END;          break;
    case GLUT_KEY_INSERT:       code = GB_KEY_INSERT;       break;
    case GLUT_KEY_PAGE_UP:      code = GB_KEY_PAGEUP;       break;
    case GLUT_KEY_PAGE_DOWN:    code = GB_KEY_PAGEDOWN;     break;

    case 0x19:                  code = GB_KEY_TABBACK;      break;

    default:
        // trace
        tb_trace_e("unknown key: %d", key);
        break;
    }

    // ok?
    return code;
}
static tb_void_t gb_window_glut_special(tb_int_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_d("special: %d at: %d, %d", key, x, y);

    // init event
    gb_event_t event = {0};
    event.type                  = GB_EVENT_TYPE_KEYBOARD;
    event.u.keyboard.code       = gb_window_glut_special_map(key);
    event.u.keyboard.pressed    = tb_true;

    // done event
    if (event.u.keyboard.code) gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_void_t gb_window_glut_special_up(tb_int_t key, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_d("special: %d at: %d, %d", key, x, y);

    // init event
    gb_event_t event = {0};
    event.type                  = GB_EVENT_TYPE_KEYBOARD;
    event.u.keyboard.code       = gb_window_glut_special_map(key);
    event.u.keyboard.pressed    = tb_false;

    // done event
    if (event.u.keyboard.code) gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_void_t gb_window_glut_mouse(tb_int_t button, tb_int_t state, tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_d("mouse: button: %d, state: %d at: %d, %d", button, state, x, y);

    // init event
    gb_event_t event = {0};
    gb_point_imake(&event.u.mouse.cursor, x, y);

    // left
	if (button == GLUT_LEFT_BUTTON) 
	{
        // save event
        event.type              = GB_EVENT_TYPE_MOUSE;
        event.u.mouse.button    = GB_MOUSE_BUTTON_LEFT;
        event.u.mouse.code      = (state == GLUT_DOWN)? GB_MOUSE_DOWN : GB_MOUSE_UP;

        // save button
        impl->button            = (state == GLUT_DOWN)? GB_MOUSE_BUTTON_LEFT : GB_MOUSE_BUTTON_NONE;
	}
    // right
	else if (button == GLUT_RIGHT_BUTTON) 
	{	
        // save event
        event.type              = GB_EVENT_TYPE_MOUSE;
        event.u.mouse.button    = GB_MOUSE_BUTTON_RIGHT;
        event.u.mouse.code      = (state == GLUT_DOWN)? GB_MOUSE_DOWN : GB_MOUSE_UP;

        // save button
        impl->button            = (state == GLUT_DOWN)? GB_MOUSE_BUTTON_RIGHT : GB_MOUSE_BUTTON_NONE;
	}
    // middle
	else if (button == GLUT_MIDDLE_BUTTON) 
	{	
        // save event
        event.type              = GB_EVENT_TYPE_MOUSE;
        event.u.mouse.button    = GB_MOUSE_BUTTON_MIDDLE;
        event.u.mouse.code      = (state == GLUT_DOWN)? GB_MOUSE_DOWN : GB_MOUSE_UP;

        // save button
        impl->button            = (state == GLUT_DOWN)? GB_MOUSE_BUTTON_MIDDLE : GB_MOUSE_BUTTON_NONE;
	}
    // scroll: wheelup: 3, wheeldown: 4
	else if ((button == 3 || button == 4) && (state == GLUT_DOWN))
	{	
        // save event
        event.type              = GB_EVENT_TYPE_MOUSE;
        event.u.mouse.button    = GB_MOUSE_BUTTON_MIDDLE;
        event.u.mouse.code      = GB_MOUSE_SCROLL;
        gb_point_imake(&event.u.mouse.scroll, x, button == 3? y - 10 : y + 10);

        // clear button
        impl->button            = GB_MOUSE_BUTTON_NONE;
	}
    else
    {
        // clear button
        impl->button            = GB_MOUSE_BUTTON_NONE;
    }

    // done event
    if (event.type) gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_void_t gb_window_glut_motion(tb_int_t x, tb_int_t y)
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl);

    // trace
//    tb_trace_d("motion: %d, %d", x, y);
 
    // init event
    gb_event_t event = {0};
    event.type              = GB_EVENT_TYPE_MOUSE;
    event.u.mouse.code      = GB_MOUSE_MOVE;
    event.u.mouse.button    = impl->button;
    gb_point_imake(&event.u.mouse.cursor, x, y);

    // done event
    gb_window_impl_event((gb_window_ref_t)impl, &event);
}
static tb_void_t gb_window_glut_timer(tb_int_t value)
{
    // check
    gb_window_glut_impl_t* impl = g_windows[value];
    tb_assert_and_check_return(impl);

    // trace
//    tb_trace_d("timer: %d", value);

    // post to draw it
    glutPostRedisplay();

    // compute the delay for framerate
    if (!impl->delay) impl->delay = 1000 / (impl->base.info.framerate? impl->base.info.framerate : GB_WINDOW_DEFAULT_FRAMERATE);

    // next timer
    glutTimerFunc(impl->delay > (tb_size_t)impl->time? impl->delay - (tb_size_t)impl->time : 0, gb_window_glut_timer, value);
}
static tb_void_t gb_window_glut_visibility(tb_int_t state)
{
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && impl->canvas);

    // trace
    tb_trace_d("visibility: %d", state);
 
    // init event
    gb_event_t              event = {0};
    event.type              = GB_EVENT_TYPE_ACTIVE;
    event.u.active.code     = state == GLUT_VISIBLE? GB_ACTIVE_FOREGROUND : GB_ACTIVE_BACKGROUND;

    // done event
    gb_window_impl_event((gb_window_ref_t)impl, &event);
}
#ifndef TB_CONFIG_OS_WINDOWS
static tb_void_t gb_window_glut_close()
{ 
    // check
    gb_window_glut_impl_t* impl = gb_window_glut_get();
    tb_assert_and_check_return(impl && impl->canvas);

    // trace
    tb_trace_d("clos");

    // stop it
    tb_atomic_set(&impl->stop, 1);
}
#endif
static tb_void_t gb_window_glut_exit(gb_window_ref_t window)
{
    // check
    gb_window_glut_impl_t* impl = (gb_window_glut_impl_t*)window;
    tb_assert_and_check_return(impl && impl->id < tb_arrayn(g_windows));

    // stop it
    tb_atomic_set(&impl->stop, 1);

    // exit canvas
    if (impl->canvas) gb_canvas_exit(impl->canvas);
    impl->canvas = tb_null;

    // exit window
    if (impl->id) 
    {
        glutDestroyWindow(impl->id);
        g_windows[impl->id] = tb_null;
        impl->id = 0;
    }

    // exit it
    tb_free(window);
}
static tb_void_t gb_window_glut_loop(gb_window_ref_t window)
{
    // check
    gb_window_glut_impl_t* impl = (gb_window_glut_impl_t*)window;
    tb_assert_and_check_return(impl);

    // fullscreen
    if (impl->base.flag & GB_WINDOW_FLAG_FULLSCREEN) 
    {
        // remove fullscreen first
        impl->base.flag &= ~GB_WINDOW_FLAG_FULLSCREEN;

        // enter fullscreen 
        gb_window_fullscreen((gb_window_ref_t)impl, tb_true);
    }
    else
    {
        // append fullscreen first
        impl->base.flag |= GB_WINDOW_FLAG_FULLSCREEN;

        // leave fullscreen 
        gb_window_fullscreen((gb_window_ref_t)impl, tb_false);
    }

    // hide cursor?
    if (impl->base.flag & GB_WINDOW_FLAG_HIHE_CURSOR) 
        glutSetCursor(GLUT_CURSOR_NONE);

    // init canvas
    if (!impl->canvas) impl->canvas = gb_canvas_init_from_window(window);
    tb_assert_abort(impl->canvas);

    // done init
    if (impl->base.info.init && !impl->base.info.init((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv)) return ;

    // loop
#ifdef TB_CONFIG_OS_WINDOWS
    glutMainLoop();
#else
    while (!tb_atomic_get(&impl->stop))
    {
        glutCheckLoop();
    }
#endif

    // done exit
    if (impl->base.info.exit) impl->base.info.exit((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);
}
static tb_void_t gb_window_glut_fullscreen(gb_window_ref_t window, tb_bool_t fullscreen)
{
    // check
    gb_window_glut_impl_t* impl = (gb_window_glut_impl_t*)window;
    tb_assert_and_check_return(impl);

    // fullscreen?
    if (fullscreen && !(impl->base.flag & GB_WINDOW_FLAG_FULLSCREEN)) 
    {
        // save the normal width and height
        impl->normal_width  = impl->base.width;
        impl->normal_height = impl->base.height;

        // enter fullscreen
        glutFullScreen();

        // update flag
        impl->base.flag |= GB_WINDOW_FLAG_FULLSCREEN;
    }
    else if (impl->base.flag & GB_WINDOW_FLAG_FULLSCREEN)
    {
        // the screen width and height
        tb_size_t screen_width  = tb_screen_width();
        tb_size_t screen_height = tb_screen_height();
        tb_assert_abort(screen_width && screen_height && screen_width >= impl->normal_width && screen_height >= impl->normal_height);

        // move window
        glutPositionWindow((screen_width - impl->normal_width) >> 1, (screen_height - impl->normal_height) >> 1);

        // resize window
        glutReshapeWindow(impl->normal_width, impl->normal_height);

        // update flag
        impl->base.flag &= ~GB_WINDOW_FLAG_FULLSCREEN;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init_glut(gb_window_info_ref_t info)
{
    // done
    tb_bool_t               ok = tb_false;
    gb_window_glut_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(info && info->framerate);
        tb_assert_and_check_break(info->width && info->width <= GB_WIDTH_MAXN && info->height && info->height <= GB_HEIGHT_MAXN);

        // make window
        impl = tb_malloc0_type(gb_window_glut_impl_t);
        tb_assert_and_check_break(impl);

        // init base
        impl->base.type         = GB_WINDOW_TYPE_GLUT;
        impl->base.mode         = GB_WINDOW_MODE_GL;
        impl->base.flag         = info->flag;
        impl->base.width        = info->width;
        impl->base.height       = info->height;
        impl->base.loop         = gb_window_glut_loop;
        impl->base.exit         = gb_window_glut_exit;
        impl->base.fullscreen   = gb_window_glut_fullscreen;
        impl->base.info         = *info;

        // init normal width and height
        impl->normal_width      = info->width;
        impl->normal_height     = info->height;

        /* init pixfmt
         * 
         * supports: 
         * - rgba8888_be
         * - rgb565_le
         * - rgb888_be
         * - rgba4444_be
         * - rgba5551_le
         */
        impl->base.pixfmt       = gb_quality() < GB_QUALITY_TOP? GB_PIXFMT_RGB565 : (GB_PIXFMT_RGBA8888 | GB_PIXFMT_BENDIAN);

        // init loop
        impl->stop = 0;

        // init glut
        tb_int_t    argc = 1;
        tb_char_t*  argv[] = {"", tb_null};
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(impl->base.width, impl->base.height);

        // make window
        impl->id = glutCreateWindow(impl->base.info.title? impl->base.info.title : "");
        tb_assert_and_check_break(impl->id && impl->id < tb_arrayn(g_windows));

        // save window
        g_windows[impl->id] = impl;

        // init window func
        glutDisplayFunc(gb_window_glut_display);
        glutReshapeFunc(gb_window_glut_reshape);
        glutKeyboardFunc(gb_window_glut_keyboard);
        glutSpecialFunc(gb_window_glut_special);
        glutKeyboardUpFunc(gb_window_glut_keyboard_up);
        glutSpecialUpFunc(gb_window_glut_special_up);
        glutMouseFunc(gb_window_glut_mouse);
        glutMotionFunc(gb_window_glut_motion);
        glutPassiveMotionFunc(gb_window_glut_motion);
        glutTimerFunc(1000 / impl->base.info.framerate, gb_window_glut_timer, impl->id);
        glutVisibilityFunc(gb_window_glut_visibility);
#ifndef TB_CONFIG_OS_WINDOWS
        glutWMCloseFunc(gb_window_glut_close);
#endif

        // check: not implementation
        tb_assert(!(info->flag & GB_WINDOW_FLAG_HIHE_TITLEBAR));
        tb_assert(!(info->flag & GB_WINDOW_FLAG_NOT_REISZE));

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
