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
 * @file        ellipse.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_ELLIPSE_H
#define GB_CORE_PREFIX_ELLIPSE_H

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

/*! make ellipse
 *
 * @param ellipse   the ellipse
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_ellipse_make(gb_ellipse_ref_t ellipse, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry);

/*! make ellipse with the integer value
 *
 * @param ellipse   the ellipse
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_ellipse_imake(gb_ellipse_ref_t ellipse, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry);

/*! make ellipse from rect
 *
 * @param ellipse   the ellipse
 * @param rect      the rect
 */
tb_void_t           gb_ellipse_make_from_rect(gb_ellipse_ref_t ellipse, gb_rect_ref_t rect);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
