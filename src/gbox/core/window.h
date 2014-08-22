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
 * @ingroup     core
 *
 */
#ifndef GB_CORE_WINDOWH
#define GB_CORE_WINDOWH

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

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
,   GB_WINDOW_TYPE_FRAMEBUFFER      = 4

}gb_window_type_e;

/// the window mode enum
typedef enum __gb_window_mode_e
{
    GB_WINDOW_MODE_NONE             = 0
,   GB_WINDOW_MODE_GL               = 1
,   GB_WINDOW_MODE_BITMAP           = 2

}gb_window_mode_e;

/// the window type
typedef struct{}*           gb_window_ref_t;

/*! the window draw func type
 *
 * @param window            the window
 * @param canvas            the canvas
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_draw_func_t)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window clos func type
 *
 * @param window            the window
 * @param priv              the user private data
 */
typedef tb_void_t           (*gb_window_clos_func_t)(gb_window_ref_t window, tb_cpointer_t priv);

/// the window info type
typedef struct __gb_window_info_t
{
    /// the window title
    tb_char_t const*        title;

    /// the framerate
    gb_float_t              framerate;

    /// the draw func
    gb_window_draw_func_t   draw;

    /// the clos func
    gb_window_clos_func_t   clos;

    /// the user private data
    tb_cpointer_t           priv;

}gb_window_info_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

#ifdef GB_CONFIG_THIRD_HAVE_GLUT
/*! init glut window 
 *
 * init a new window
 *
 * @param info          the window info
 * @param width         the window width 
 * @param height        the window height
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_glut(gb_window_info_t const* info, tb_size_t width, tb_size_t height);
#endif

#ifdef GB_CONFIG_THIRD_HAVE_GLFW
/*! init glfw window 
 *
 * init a new window
 *
 * @param info          the window info
 * @param width         the window width 
 * @param height        the window height
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_glfw(gb_window_info_t const* info, tb_size_t width, tb_size_t height);
#endif

#ifdef TB_CONFIG_OS_IOS
/*! init ios window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_ios(gb_window_info_t const* info);
#endif

#ifdef TB_CONFIG_OS_ANDROID
/*! init android window 
 *
 * @param info          the window info
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_android(gb_window_info_t const* info);
#endif

#ifdef TB_CONFIG_OS_LINUX
/*! init framebuffer window 
 *
 * @param info          the window info
 * @param device_name   the framebuffer device name
 *
 * @return              the window
 */
gb_window_ref_t         gb_window_init_framebuffer(gb_window_info_t const* info, tb_char_t const* device_name);
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

__tb_extern_c_leave__
#endif
