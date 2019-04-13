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
 * @file        window.h
 * @ingroup     platform
 *
 */
#ifndef GB_PLATFORM_WINDOW_H
#define GB_PLATFORM_WINDOW_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "event.h"
#include "../core/prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the window type enum
typedef enum __gb_window_type_e
{
    GB_WINDOW_TYPE_NONE             = 0
,   GB_WINDOW_TYPE_GLUT             = 1
,   GB_WINDOW_TYPE_IOS              = 2
,   GB_WINDOW_TYPE_ANDROID          = 3
,   GB_WINDOW_TYPE_SDL              = 4
,   GB_WINDOW_TYPE_X11              = 5

}gb_window_type_e;

/// the window mode enum
typedef enum __gb_window_mode_e
{
    GB_WINDOW_MODE_NONE             = 0
,   GB_WINDOW_MODE_GL               = 1
,   GB_WINDOW_MODE_BITMAP           = 2

}gb_window_mode_e;

/// the window flag enum
typedef enum __gb_window_flag_e
{
    GB_WINDOW_FLAG_NONE             = 0
,   GB_WINDOW_FLAG_FULLSCREEN       = 1
,   GB_WINDOW_FLAG_HIHE_TITLEBAR    = 2
,   GB_WINDOW_FLAG_HIHE_CURSOR      = 4
,   GB_WINDOW_FLAG_NOT_REISZE       = 8

}gb_window_flag_e;

/// the window type
typedef struct{}*           gb_window_ref_t;

/*! the window init func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 *
 * @return                  tb_true or tb_false
 */
typedef tb_bool_t           (*gb_window_init_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window exit func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_exit_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window draw func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_draw_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window resize func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_resize_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window event func type
 *
 * @param window            the window
 * @param event             the event
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_event_func_t)(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv);

/// the window info type
typedef struct __gb_window_info_t
{
    /// the window title
    tb_char_t const*                title;

    /// the framerate
    tb_uint8_t                      framerate;

    /// the flag
    tb_uint8_t                      flag;

    /// the width
    tb_uint16_t                     width;

    /// the height
    tb_uint16_t                     height;

    /// the init func
    gb_window_init_func_t           init;

    /// the exit func
    gb_window_exit_func_t           exit;

    /// the draw func
    gb_window_draw_func_t           draw;

    /// the resize func
    gb_window_resize_func_t         resize;

    /// the event func
    gb_window_event_func_t          event;

    /// the user private data
    tb_cpointer_t                   priv;

    /*! the hint data
     *
     * - framebuffer: the device name, .e.g: "/dev/fb0", ...
     */
    tb_cpointer_t                   hint;

}gb_window_info_t, *gb_window_info_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init(gb_window_info_ref_t info);

#ifdef GB_CONFIG_PACKAGE_HAVE_GLUT
/*! init glut window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_glut(gb_window_info_ref_t info);
#endif

#ifdef GB_CONFIG_PACKAGE_HAVE_SDL
/*! init sdl window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_sdl(gb_window_info_ref_t info);
#endif

#ifdef GB_CONFIG_PACKAGE_HAVE_X11
/*! init x11 window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_x11(gb_window_info_ref_t info);
#endif

/*! exit window 
 *
 * @param window        the window
 */
tb_void_t               gb_window_exit(gb_window_ref_t window);

/*! the window type
 *
 * @param window        the window
 *
 * @return              the type
 */
tb_size_t               gb_window_type(gb_window_ref_t window);

/*! the window mode
 *
 * @param window        the window
 *
 * @return              the mode
 */
tb_size_t               gb_window_mode(gb_window_ref_t window);

/*! the window flag
 *
 * @param window        the window
 *
 * @return              the flag
 */
tb_size_t               gb_window_flag(gb_window_ref_t window);

/*! loop window
 *
 * @param window        the window
 */
tb_void_t               gb_window_loop(gb_window_ref_t window);

/*! the window pixfmt
 *
 * @param window        the window
 *
 * @return              the pixfmt
 */
tb_size_t               gb_window_pixfmt(gb_window_ref_t window);

/*! the window width
 *
 * @param window        the window
 *
 * @return              the width
 */
tb_size_t               gb_window_width(gb_window_ref_t window);

/*! the window height
 *
 * @param window        the window
 *
 * @return              the height
 */
tb_size_t               gb_window_height(gb_window_ref_t window);

/*! the window title
 *
 * @param window        the window
 *
 * @return              the title
 */
tb_char_t const*        gb_window_title(gb_window_ref_t window);

/*! the window bitmap for the bitmap mode
 *
 * @param window        the window
 *
 * @return              the bitmap if be the bitmap mode, otherwise null
 */
gb_bitmap_ref_t         gb_window_bitmap(gb_window_ref_t window);

/*! the window framerate
 *
 * @param window        the window
 *
 * @return              the framerate
 */
gb_float_t              gb_window_framerate(gb_window_ref_t window);

/*! enter or leave the fullscreen only for the desktop window
 *
 * @param window        the window
 * @param fullscreen    is fullscreen?
 */
tb_void_t               gb_window_fullscreen(gb_window_ref_t window, tb_bool_t fullscreen);

/*! the window timer
 *
 * @note the timer task will be called in the draw loop
 *
 * @param window        the window
 *
 * @return              the timer
 */
tb_timer_ref_t          gb_window_timer(gb_window_ref_t window);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
