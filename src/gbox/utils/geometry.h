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
 * @file        geometry.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_GEOMETRY_H
#define GB_UTILS_GEOMETRY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* the three points are counter-clockwise?
 *
 *                   p1
 *  . <-------------- .
 * p2                / \
 *                    |
 *                    |
 *                    |
 *                    .
 *                   p0
 *
 * @param p0            the first point
 * @param p1            the second point
 * @param p2            the last point
 *
 */
tb_long_t               gb_points_is_ccw(gb_point_ref_t p0, gb_point_ref_t p1, gb_point_ref_t p2);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


