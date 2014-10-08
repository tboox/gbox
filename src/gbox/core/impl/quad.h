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
 * @file        quad.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_QUAD_H
#define GB_CORE_IMPL_QUAD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the max quadratic curve divided count
#define GB_QUAD_DIVIDED_MAXN          (5)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compute the approximate distance
 *
 *                  p1
 *                  .
 *                .  .
 *              .  .  .
 *            .   .    .
 *          .    .      .
 *        .     .        .
 *      .      .distance  .
 *    .       .            .
 * p0         x           p2
 *
 * distance(p1, center(p0, p2))
 *
 * @param points    the points
 *
 * @return          the approximate distance
 */
gb_float_t          gb_quad_near_distance(gb_point_t points[3]);

/* compute the approximate divided count
 *
 * @param points    the points
 *
 * @return          the approximate divided count
 */
tb_size_t           gb_quad_divided_count(gb_point_t points[3]);

/* chop the quad points at the half position
 *
 *           chop
 *            |
 *          . . .
 *      .     p1   .
 *   .               .
 * .                   .
 * p0                   p2
 *
 * quad(p0, p1, p2) => quad(o0, o1, o2) + quad(o2, o3, o4)
 *
 * @param points    the points
 * @param output    the output points
 */
tb_void_t           gb_quad_chop_at_half(gb_point_t points[3], gb_point_t output[5]);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


