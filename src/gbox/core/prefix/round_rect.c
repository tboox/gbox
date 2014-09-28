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
 * @file        rect.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "round_rect.h"
#include "rect.h"
#include "matrix.h"
#include "../impl/bounds.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_round_rect_make(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_float_t rx[], gb_float_t ry[])
{
    // check
    tb_assert_abort(rect && bounds && rx && ry);
    tb_assert_static(GB_RECT_CORNER_MAXN == 4);

    // make it
    rect->rx[0] = rx[0]; 
    rect->rx[1] = rx[1];  
    rect->rx[2] = rx[2]; 
    rect->rx[3] = rx[3]; 
    rect->ry[0] = ry[0]; 
    rect->ry[1] = ry[1]; 
    rect->ry[2] = ry[2]; 
    rect->ry[3] = ry[3]; 
}
tb_void_t gb_round_rect_make_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry)
{
    // check
    tb_assert_abort(rect && bounds);
    tb_assert_static(GB_RECT_CORNER_MAXN == 4);

    // make it
    rect->rx[0] = rx; 
    rect->rx[1] = rx; 
    rect->rx[2] = rx; 
    rect->rx[3] = rx; 
    rect->ry[0] = ry; 
    rect->ry[1] = ry; 
    rect->ry[2] = ry; 
    rect->ry[3] = ry;  
    rect->bounds = *bounds;
}
tb_void_t gb_round_rect_imake_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry)
{
    // make it
    gb_round_rect_make_same(rect, bounds, gb_long_to_float(rx), gb_long_to_float(ry));
}
tb_bool_t gb_round_rect_is_rect(gb_round_rect_ref_t rect)
{
    // TODO
    tb_trace_noimpl();
    return tb_false;
}
tb_bool_t gb_round_rect_is_ellipse(gb_round_rect_ref_t rect)
{
    // TODO
    tb_trace_noimpl();
    return tb_false;
}
