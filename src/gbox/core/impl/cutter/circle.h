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
#ifndef GB_CORE_IMPL_CUTTER_CIRCLE_H
#define GB_CORE_IMPL_CUTTER_CIRCLE_H

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

/* done circle cutter
 *
 * @param circle        the circle
 * @param func          the cutter func
 * @param priv          the cutter func private data for user
 *
 * <pre>
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
 * q = 2 * pi / n;
 *
 * d = r * (1 - cos(q)) ~= r * q * q / 2 < e
 *
 * n > pi * sqrt(2 / e) * sqrt(r)
 *
 * e = 0.125: n > 4 * pi * sqrt(r)
 *
 * y = pi * sqrt(x):
 * x: [0, 90]:      y = (1 / 3) * x
 * x: [90, 1000]:   y = (0.08) * x + 23
 *
 * r: [0, 90]:      n > (4 / 3) * r [+ 16]
 * r: [90, 1000]:   n > (1 / 3) * r + 128 [- 32]
 *
 * r = 1:
 * xx = x * cos(q) - y * sin(q)
 * yy = x * sin(q) + y * cos(q)
 * 
 * if (q < 1)
 * a: cos(q) ~= 1 - q * q / 2
 * b: sin(q) ~= q - q * q * q / 6
 
 * </pre>
 */
tb_void_t               gb_cutter_circle_done(gb_circle_ref_t circle, gb_cutter_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


