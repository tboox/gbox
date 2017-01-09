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
 * @file        path_cache.h
 * @ingroup     core
 */
#ifndef GB_CORE_IMPL_PATH_CACHE_H
#define GB_CORE_IMPL_PATH_CACHE_H

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

// the cache cache ref type
typedef struct{}*       gb_path_cache_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the path cache
 *
 * cache: shape => path
 *
 * @return              the path cache
 */
gb_path_cache_ref_t     gb_path_cache_init(tb_noarg_t);

/* exit the path cache
 *
 * @param cache         the path cache
 */
tb_void_t               gb_path_cache_exit(gb_path_cache_ref_t cache);

/* clear the path cache
 *
 * @param cache         the cache
 */
tb_void_t               gb_path_cache_clear(gb_path_cache_ref_t cache);

/* get path from the given shape
 *
 * @param cache         the cache
 * @param shape         the shape
 * 
 * @return              the shape path
 */
gb_path_ref_t           gb_path_cache_get(gb_path_cache_ref_t cache, gb_shape_ref_t shape);

/* add shape and make path to cache
 *
 * @param cache         the cache
 * @param shape         the shape
 *
 * @return              the shape path
 */
gb_path_ref_t           gb_path_cache_add(gb_path_cache_ref_t cache, gb_shape_ref_t shape);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
