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
 * @file        circle.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_CIRCLE_H
#define GB_CORE_PREFIX_CIRCLE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! make circle
 *
 * @param circle    the circle
 * @param x0        the x0
 * @param y0        the y0
 * @param r         the radius
 */
tb_void_t           gb_circle_make(gb_circle_ref_t circle, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! make circle with the integer value
 *
 * @param circle    the circle
 * @param x0        the x0
 * @param y0        the y0
 * @param r         the radius
 */
tb_void_t           gb_circle_imake(gb_circle_ref_t circle, tb_long_t x0, tb_long_t y0, tb_size_t r);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
