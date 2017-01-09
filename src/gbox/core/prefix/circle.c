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
 * @file        circle.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "circle.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_circle_make(gb_circle_ref_t circle, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // check
    tb_assert(circle);

    // make it
    circle->c.x = x0;
    circle->c.y = y0;
    circle->r   = r;
}
tb_void_t gb_circle_imake(gb_circle_ref_t circle, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    gb_circle_make(circle, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(r));
}

