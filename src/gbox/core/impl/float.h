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
 * @file        float.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_FLOAT_H
#define GB_CORE_IMPL_FLOAT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// is in the unit range(0, 1)?
#define gb_float_in_unit_range(x)       ((x) > 0 && (x) < GB_ONE)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compute numer / denom and check it
 *
 * @param numer     the numerator
 * @param denom     the denominator
 * @param result    the result, must be in range: (0, 1)
 *
 * @return          the result is in range(0, 1): 1, otherwise: 0
 */
tb_size_t           gb_float_unit_divide(gb_float_t numer, gb_float_t denom, gb_float_t* result);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


