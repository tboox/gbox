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
 * @file        path_cache.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "path_cache.h"
#include "../path.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_path_cache_ref_t gb_path_cache_init()
{
    return tb_null;
}
tb_void_t gb_path_cache_exit(gb_path_cache_ref_t cache)
{
}
tb_void_t gb_path_cache_clear(gb_path_cache_ref_t cache)
{
}
gb_path_ref_t gb_path_cache_get(gb_path_cache_ref_t cache, gb_shape_ref_t shape)
{
    return tb_null;
}
gb_path_ref_t gb_path_cache_add(gb_path_cache_ref_t cache, gb_shape_ref_t shape)
{
    return tb_null;
}
