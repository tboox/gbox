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
