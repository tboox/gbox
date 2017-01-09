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
 * @file        float.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_size_t gb_float_unit_divide(gb_float_t numer, gb_float_t denom, gb_float_t* result)
{
    // check
    tb_assert(result);

    // negate it
    if (numer < 0) 
    {
        numer = -numer;
        denom = -denom;
    }

    // must be valid numerator and denominator
    if (0 == denom || 0 == numer || numer >= denom) 
        return 0;

    // the result: numer / denom
    gb_float_t r = gb_div(numer, denom);

    // must be finite value
    tb_assert_and_check_return_val(gb_isfinite(r), 0);

    // must be in range: [0, 1)
    tb_assert_and_check_return_val(r >= 0 && r < GB_ONE, 0);

    // too smaller? not save result
    tb_check_return_val(r != 0, 0);

    // save result
    *result = r;

    // ok
    return 1;
}

