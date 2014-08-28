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
 * @file        prefix.h
 * @ingroup     platform
 */
#ifndef GB_PLATFORM_WINDOW_PREFIX_H
#define GB_PLATFORM_WINDOW_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the window impl type
typedef struct __gb_window_impl_t
{
    // the window type
    tb_uint8_t              type;

    // the window mode
    tb_uint8_t              mode;

    // the window flag
    tb_uint8_t              flag;

    // the window pixfmt
    tb_uint16_t             pixfmt;

    // the window width
    tb_uint16_t             width;

    // the window height
    tb_uint16_t             height;

    // the window info
    gb_window_info_t        info;
 
    // the timer
    tb_timer_ref_t          timer;

    // the bitmap for the bitmap mode
    gb_bitmap_ref_t         bitmap;
  
    // the framerate: fps
    gb_float_t              framerate;

    // the base time for fps
    tb_hong_t               fps_time;

    // the frame count for fps
    tb_size_t               fps_count;

    /* loop window
     *
     * @param window        the window
     */
    tb_void_t               (*loop)(gb_window_ref_t window);

    /* exit window
     *
     * @param window        the window
     */
    tb_void_t               (*exit)(gb_window_ref_t window);

    /*! enter or leave fullscreen
     *
     * @param window        the window
     * @param fullscreen    is fullscreen?
     */
    tb_void_t               (*fullscreen)(gb_window_ref_t window, tb_bool_t fullscreen);

    /*! enter or leave maximum
     *
     * @param window        the window
     * @param maximum       is maximum?
     */
    tb_void_t               (*maximum)(gb_window_ref_t window, tb_bool_t maximum);

    /*! enter or leave minimum
     *
     * @param window        the window
     * @param maximum       is maximum?
     */
    tb_void_t               (*minimum)(gb_window_ref_t window, tb_bool_t minimum);

    /*! show or hide the window
     *
     * @param window        the window
     * @param show          is show?
     */
    tb_void_t               (*show)(gb_window_ref_t window, tb_bool_t show);

}gb_window_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* draw window
 *
 * @param window            the window
 * @param canvas            the canvas
 */
tb_void_t                   gb_window_impl_draw(gb_window_ref_t window, gb_canvas_ref_t canvas);

/* the window event
 *
 * @param window            the window
 * @param event             the event
 */
tb_void_t                   gb_window_impl_event(gb_window_ref_t window, gb_event_ref_t event);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif


