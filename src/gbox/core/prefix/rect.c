/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
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
