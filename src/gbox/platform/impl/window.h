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
 */
#ifndef GB_PLATFORM_IMPL_WINDOW_H
#define GB_PLATFORM_IMPL_WINDOW_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../window.h"
#include "../../core/device.h"
#include "../../core/canvas.h"
#include "../../core/pixmap.h"
#include "../../core/bitmap.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the default framerate: 30
#define GB_WINDOW_DEFAULT_FRAMERATE         (30)

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

/* spak window
 *
 * @param window            the window
 *
 * @return                  the time before spanking
 */
tb_hong_t                   gb_window_impl_spak(gb_window_ref_t window);

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


