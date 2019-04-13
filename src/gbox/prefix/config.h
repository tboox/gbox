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
 * @file        config.h
 *
 */
#ifndef GB_PREFIX_CONFIG_H
#define GB_PREFIX_CONFIG_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../config.h"
#include "tbox/tbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/*! @def __gb_small__
 *
 * small mode
 */
#ifdef GB_CONFIG_SMALL
#   define __gb_small__
#endif

/*! @def __gb_debug__
 *
 * debug mode
 */
#ifdef __tb_debug__
#   define __gb_debug__
#endif

#endif


