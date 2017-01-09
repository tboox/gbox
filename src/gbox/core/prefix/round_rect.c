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
 * @file        round_rect.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "round_rect.h"
#include "rect.h"
#include "float.h"
#include "matrix.h"
#include "../impl/bounds.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_round_rect_make(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_vector_t radius[])
{
    // check
    tb_assert(rect && bounds && radius);
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
    tb_assert(rect && bounds);
    tb_assert_static(GB_RECT_CORNER_MAXN == 4);

    // the radius
    gb_vector_t radius;
    gb_vector_make(&radius, rx, ry);

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
    tb_assert(rect);

    // is rect?
    return (    (rect->radius[0].x <= 0)
            &&  (rect->radius[0].y <= 0)
            &&  (rect->radius[1].x <= 0)
            &&  (rect->radius[1].y <= 0)
            &&  (rect->radius[2].x <= 0)
            &&  (rect->radius[2].y <= 0)
            &&  (rect->radius[3].x <= 0)
            &&  (rect->radius[3].y <= 0))? tb_true : tb_false;
}
tb_bool_t gb_round_rect_is_ellipse(gb_round_rect_ref_t rect)
{
    // check
    tb_assert(rect);

    // the radius
    gb_float_t rx = gb_half(rect->bounds.w);
    gb_float_t ry = gb_half(rect->bounds.h);

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
