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
tb_void_t gb_round_rect_make(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_vector_t radius[])
{
    // check
    tb_assert_abort(rect && bounds && radius);
    tb_assert_static(GB_RECT_CORNER_MAXN == 4);

    // make it
    rect->radius[0] = radius[0]; 
    rect->radius[1] = radius[1];  
    rect->radius[2] = radius[2]; 
    rect->radius[3] = radius[3]; 
}
tb_void_t gb_round_rect_make_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry)
{
    // check
    tb_assert_abort(rect && bounds);
    tb_assert_static(GB_RECT_CORNER_MAXN == 4);

    // the radius
    gb_vector_t radius = gb_vector_make(rx, ry);

    // make it
    rect->radius[0] = radius; 
    rect->radius[1] = radius; 
    rect->radius[2] = radius; 
    rect->radius[3] = radius; 
    rect->bounds    = *bounds;
}
tb_void_t gb_round_rect_imake_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry)
{
    // make it
    gb_round_rect_make_same(rect, bounds, gb_long_to_float(rx), gb_long_to_float(ry));
}
tb_bool_t gb_round_rect_is_rect(gb_round_rect_ref_t rect)
{
    // check
    tb_assert_abort(rect);

    // is rect?
    return (    !gb_bz(rect->radius[0].x)
            &&  !gb_bz(rect->radius[0].y)
            &&  !gb_bz(rect->radius[1].x)
            &&  !gb_bz(rect->radius[1].y)
            &&  !gb_bz(rect->radius[2].x)
            &&  !gb_bz(rect->radius[2].y)
            &&  !gb_bz(rect->radius[3].x)
            &&  !gb_bz(rect->radius[3].y))? tb_true : tb_false;
}
tb_bool_t gb_round_rect_is_ellipse(gb_round_rect_ref_t rect)
{
    // check
    tb_assert_abort(rect);

    // the radius
    gb_float_t rx = gb_rsh(rect->bounds.w, 1);
    gb_float_t ry = gb_rsh(rect->bounds.h, 1);

    // is rect?
    return (    rect->radius[0].x >= rx
            &&  rect->radius[0].y >= ry
            &&  rect->radius[1].x >= rx
            &&  rect->radius[1].y >= ry
            &&  rect->radius[2].x >= rx
            &&  rect->radius[2].y >= ry
            &&  rect->radius[3].x >= rx
            &&  rect->radius[3].y >= ry)? tb_true : tb_false;
}
