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
 * @file        ellipse.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ellipse.h"
#include "point.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_ellipse_make(gb_ellipse_ref_t ellipse, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // check
    tb_assert(ellipse);

    // make it
    ellipse->c.x    = x0;
    ellipse->c.y    = y0;
    ellipse->rx     = rx;
    ellipse->ry     = ry;
}
tb_void_t gb_ellipse_imake(gb_ellipse_ref_t ellipse, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    gb_ellipse_make(ellipse, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(rx), gb_long_to_float(ry));
}
tb_void_t gb_ellipse_make_from_rect(gb_ellipse_ref_t ellipse, gb_rect_ref_t rect)
{
    // check
    tb_assert(ellipse && rect);

    // the radius
    gb_float_t rx = gb_half(rect->w);
    gb_float_t ry = gb_half(rect->h);

    // make it
    gb_ellipse_make(ellipse, rect->x + rx, rect->y + ry, rx, ry);
}

