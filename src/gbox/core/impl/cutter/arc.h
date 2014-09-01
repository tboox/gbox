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
#ifndef GB_CORE_IMPL_CUTTER_ARC_H
#define GB_CORE_IMPL_CUTTER_ARC_H

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

/* done arc cutter
 *
 * @param arc       the arc
 * @param func      the cutter func
 * @param priv      the cutter func private data for user
 *
 * * <pre>
 *
 *                       *
 *                     * |
 *                   *   |
 *                 *     |
 *           r   *       |
 *             *         |
 *           *           |
 *         *             |
 *       *               |
 *     *                 |
 *   *  q                |
 * **********************|********
 *         r * cos(q)        d
 *
 * q = an / n;
 *
 * d = r * (1 - cos(q)) ~= r * q * q / 2 < e
 *
 * an * sqrt(r) * sqrt(1 / (2 * e)) < n
 *
 * n > an * sqrt(r) * sqrt(1 / (2 * e))
 *
 * e = 0.125: n > 2 * an * sqrt(r)
 *
 * y = 2 * an * sqrt(x):
 * x: [0, 90]:      y = 0.21 * x * a
 * x: [90, 1000]:   y = (0.05 * x + 15) * a
 *
 * r: [0, 90]:      n > 0.25 * x * a + 16
 * r: [90, 1000]:   n > (0.0625 * x + 15) * a
 *
 * rx = ry = 1:
 * xx = x * cos(q) - y * sin(q)
 * yy = x * sin(q) + y * cos(q)
 * 
 * if (q < 1)
 * a: cos(q) ~= 1 - q * q / 2
 * b: sin(q) ~= q - q * q * q / 6
 *
 * </pre>
 */
tb_void_t           gb_cutter_arc_done(gb_arc_ref_t arc, gb_cutter_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


