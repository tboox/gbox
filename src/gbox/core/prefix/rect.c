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
#include "../impl/bounds.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_rect_make(gb_rect_ref_t rect, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    // check
    tb_assert(rect);

    // make it
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}
tb_void_t gb_rect_imake(gb_rect_ref_t rect, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    gb_rect_make(rect, gb_long_to_float(x), gb_long_to_float(y), gb_long_to_float(w), gb_long_to_float(h));
}
tb_void_t gb_rect_apply(gb_rect_ref_t rect, gb_matrix_ref_t matrix)
{
    gb_rect_apply2(rect, rect, matrix);
}
tb_void_t gb_rect_apply2(gb_rect_ref_t rect, gb_rect_ref_t applied, gb_matrix_ref_t matrix)
{
    // check
    tb_assert(rect && applied && matrix);

    // done
    gb_point_t points[2];
    gb_point_make(&points[0], rect->x, rect->y);
    gb_point_make(&points[1], rect->x + rect->w, rect->y + rect->h);
    gb_matrix_apply_points(matrix, points, tb_arrayn(points));
    gb_bounds_make(applied, points, tb_arrayn(points));
}
tb_void_t gb_rect_inflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert(rect && dx > 0 && dy > 0);

    // inflate it
    rect->x -= dx;
    rect->y -= dy;
    rect->w += gb_lsh(dx, 1);
    rect->h += gb_lsh(dy, 1);
}
tb_void_t gb_rect_deflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert(rect && dx > 0 && dy > 0);
    tb_assert(rect->w >= gb_lsh(dx, 1) && rect->h >= gb_lsh(dy, 1));

    // deflate it
    rect->x += dx;
    rect->y += dy;
    rect->w -= gb_lsh(dx, 1);
    rect->h -= gb_lsh(dy, 1);
}
