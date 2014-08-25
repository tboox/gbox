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
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_PREFIX_H
#define GB_CORE_DEVICE_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../device.h"
#include "../../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the device impl type
typedef struct __gb_device_impl_t
{
    // the device type
    tb_uint8_t              type;

    // the pixfmt
    tb_uint16_t             pixfmt;

    // the width
    tb_uint16_t             width;

    // the height
    tb_uint16_t             height;

    /* resize
     *
     * @param device        the device
     * @param width         the width
     * @param height        the height
     */
    tb_void_t               (*resize)(gb_device_ref_t device, tb_size_t width, tb_size_t height);

    /* clear draw
     *
     * @param device        the device
     * @param color         the color
     */
    tb_void_t               (*draw_clear)(gb_device_ref_t device, gb_color_t color);

    /* exit device
     *
     * @param device        the device
     */
    tb_void_t               (*exit)(gb_device_ref_t device);

}gb_device_impl_t;

#endif


