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
,   GB_WINDOW_TYPE_FRAMEBUFFER      = 6

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
,   GB_WINDOW_FLAG_MAXIMUM          = 1
,   GB_WINDOW_FLAG_MINIMUM          = 2
,   GB_WINDOW_FLAG_FULLSCREEN       = 4
,   GB_WINDOW_FLAG_HIHE             = 8
,   GB_WINDOW_FLAG_HIHE_TITLEBAR    = 16
,   GB_WINDOW_FLAG_HIHE_CURSOR      = 32
,   GB_WINDOW_FLAG_NOT_REISZE       = 64

}gb_window_flag_e;

/// the window type
typedef struct{}*           gb_window_ref_t;

/*! the window init func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_init_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

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

#ifdef GB_CONFIG_THIRD_HAVE_GLUT
/*! init glut window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_glut(gb_window_info_ref_t info);
#endif

#ifdef GB_CONFIG_THIRD_HAVE_SDL
/*! init sdl window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_sdl(gb_window_info_ref_t info);
#endif

#ifdef GB_CONFIG_THIRD_HAVE_X11
/*! init x11 window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_x11(gb_window_info_ref_t info);
#endif

#ifdef GB_CONFIG_THIRD_HAVE_FRAMEBUFFER
/*! init framebuffer window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_framebuffer(gb_window_info_ref_t info);
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

/*! enter or leave the maximum only for the desktop window
 *
 * @param window        the window
 * @param maximum       is maximum?
 */
tb_void_t               gb_window_maximum(gb_window_ref_t window, tb_bool_t maximum);

/*! enter or leave the minimum only for the desktop window
 *
 * @param window        the window
 * @param minimum       is minimum?
 */
tb_void_t               gb_window_minimum(gb_window_ref_t window, tb_bool_t minimum);

/*! show or hide the desktop window
 *
 * @param window        the window
 * @param show          is show?
 */
tb_void_t               gb_window_show(gb_window_ref_t window, tb_bool_t show);

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
