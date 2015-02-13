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
 * @file        geometry.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "geometry"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_long_t gb_points_is_ccw(gb_point_ref_t p0, gb_point_ref_t p1, gb_point_ref_t p2)
{
    // check
    tb_assert_abort(p0 && p1 && p2);

    // the coordinates
    tb_fixed_t x0 = gb_float_to_fixed(p0->x);
    tb_fixed_t y0 = gb_float_to_fixed(p0->y);
    tb_fixed_t x1 = gb_float_to_fixed(p1->x);
    tb_fixed_t y1 = gb_float_to_fixed(p1->y);
    tb_fixed_t x2 = gb_float_to_fixed(p2->x);
    tb_fixed_t y2 = gb_float_to_fixed(p2->y);
  
    /* compute the cross value of the vectors (p1, p0) and (p1, p2)
     *
     * axis:
     *         > 0
     * O --------> 
     * |
     * |
     * | > 0
     *
     * cross[(p1, p0), (p1, p2)] > 0
     */
    return ((tb_hong_t)(x0 - x1) * (y2 - y1) - (tb_hong_t)(y0 - y1) * (x2 - x1)) > 0;
}
