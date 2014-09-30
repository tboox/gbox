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
 * @file        arc.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_ARC_H
#define GB_CORE_PREFIX_ARC_H

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

/*! make arc
 *
 * @param arc       the arc
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_arc_make(gb_arc_ref_t arc, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! make arc with the integer value
 *
 * @param arc       the arc
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_arc_imake(gb_arc_ref_t arc, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
