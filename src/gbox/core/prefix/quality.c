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
 * @file        quality.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "quality.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the quality
static tb_atomic_t g_quality = GB_QUALITY_TOP;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_size_t gb_quality(tb_noarg_t)
{
    return (tb_size_t)tb_atomic_get(&g_quality);
}
tb_void_t gb_quality_set(tb_size_t quality)
{
    // check
    tb_assert_and_check_return(quality <= GB_QUALITY_TOP);

    // save quality
    tb_atomic_set(&g_quality, quality);
}

