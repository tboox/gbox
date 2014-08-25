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
 * types
 */

// the window impl type
typedef struct __gb_window_impl_t
{
    // the window type
    tb_uint8_t              type;

    // the window mode
    tb_uint8_t              mode;

    // the window pixfmt
    tb_uint16_t             pixfmt;

    // the window width
    tb_uint16_t             width;

    // the window height
    tb_uint16_t             height;

    // the window info
    gb_window_info_t        info;

    // the bitmap for the bitmap mode
    gb_bitmap_ref_t         bitmap;

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

}gb_window_impl_t;


#endif


