/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        cache_stack.h
 * @ingroup     core
 */
#ifndef GB_CORE_PRIVATE_CACHE_STACK_H
#define GB_CORE_PRIVATE_CACHE_STACK_H

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
