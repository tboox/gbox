/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
 * @file        touch.h
 * @ingroup     platform
 *
 */
#ifndef GB_PLATFORM_TOUCH_H
#define GB_PLATFORM_TOUCH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the touch type
typedef struct __gb_touch_t
{
    /// the point
    gb_point_t          point;

    /// the prev point
    gb_point_t          prev;

    /// the start point
    gb_point_t          start;

}gb_touch_t;

/// the touch ref type
typedef gb_touch_t*     gb_touch_ref_t;

#endif
