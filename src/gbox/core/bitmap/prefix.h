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
 * @file        prefix.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_BITMAP_PREFIX_H
#define GB_CORE_BITMAP_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../pixmap.h"
#include "../bitmap.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the bitmap type
typedef enum __gb_bitmap_type_t
{
    GB_BITMAP_TYPE_NONE  = 0
,   GB_BITMAP_TYPE_BMP   = 1
,   GB_BITMAP_TYPE_GIF   = 2
,   GB_BITMAP_TYPE_JPG   = 3
,   GB_BITMAP_TYPE_PNG   = 4

}gb_bitmap_type_t;

#endif


