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
 * @file        cache_stack.h
 * @ingroup     core
 */
#ifndef GB_CORE_IMPL_CACHE_STACK_H
#define GB_CORE_IMPL_CACHE_STACK_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the cache stack type enum
typedef enum __gb_cache_stack_type_e
{
    GB_CACHE_STACK_TYPE_NONE        = 0
,   GB_CACHE_STACK_TYPE_PATH        = 1
,   GB_CACHE_STACK_TYPE_PAINT       = 2
,   GB_CACHE_STACK_TYPE_CLIPPER     = 3
,   GB_CACHE_STACK_TYPE_MAXN        = 4

}gb_cache_stack_type_e;

// the stack cache ref type
typedef struct{}*           gb_cache_stack_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the cache stack for path, paint and clipper
 *
 * @param grow          the grow size
 * @param type          the object type
 *
 * @return              the stack
 */
gb_cache_stack_ref_t    gb_cache_stack_init(tb_size_t grow, tb_size_t type);

/* exit the cache stack
 *
 * @param stack         the stack
 */
tb_void_t               gb_cache_stack_exit(gb_cache_stack_ref_t stack);

/* save the current object to the top object
 *
 * @param stack         the stack
 *
 * @return              the current object
 */
tb_handle_t             gb_cache_stack_save(gb_cache_stack_ref_t stack);

/* load the top object to the current object
 *
 * @param stack         the stack
 */
tb_void_t               gb_cache_stack_load(gb_cache_stack_ref_t stack);

/* get the current object
 *
 * @param stack         the stack
 *
 * @return              the top object
 */
tb_handle_t             gb_cache_stack_object(gb_cache_stack_ref_t stack);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
