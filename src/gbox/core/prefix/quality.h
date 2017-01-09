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
 * @file        quality.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_QUALITY_H
#define GB_CORE_PREFIX_QUALITY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the quality enum
typedef enum __gb_quality_e
{
    GB_QUALITY_LOW  = 0
,   GB_QUALITY_MID  = 1
,   GB_QUALITY_TOP  = 2

}gb_quality_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! get quality
 *
 * @return          the quality
 */
tb_size_t           gb_quality(tb_noarg_t);

/*! set quality
 *
 * @param quality   the quality 
 */
tb_void_t           gb_quality_set(tb_size_t quality);

__tb_extern_c_leave__
#endif


