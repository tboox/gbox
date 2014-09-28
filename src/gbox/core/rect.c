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
#include "rect.h"
#include "matrix.h"
#include "impl/bounds.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_rect_apply(gb_rect_ref_t rect, gb_matrix_ref_t matrix)
{
    gb_rect_apply2(rect, rect, matrix);
}
tb_void_t gb_rect_apply2(gb_rect_ref_t rect, gb_rect_ref_t output, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_and_check_return(rect && output && matrix);

    // done
    gb_point_t points[2];
    points[0] = gb_point_make(rect->x, rect->y);
    points[1] = gb_point_make(rect->x + rect->w, rect->y + rect->h);
    gb_matrix_apply_points(matrix, points, tb_arrayn(points));
    gb_bounds_make(output, points, tb_arrayn(points));
}
tb_void_t gb_rect_inflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert_and_check_return(rect && gb_bz(dx) && gb_bz(dy));

    // inflate it
    rect->x -= dx;
    rect->y -= dy;
    rect->w += gb_lsh(dx, 1);
    rect->h += gb_lsh(dy, 1);
}
tb_void_t gb_rect_deflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert_and_check_return(rect && gb_bz(dx) && gb_bz(dy));
    tb_assert_abort(rect->w >= gb_lsh(dx, 1) && rect->h >= gb_lsh(dy, 1));

    // deflate it
    rect->x += dx;
    rect->y += dy;
    rect->w -= gb_lsh(dx, 1);
    rect->h -= gb_lsh(dy, 1);
}
