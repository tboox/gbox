/*!The Graphic 2D Box Library
 * 
 * GBox2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox2; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author		ruki
 * @file		prefix.h
 *
 */
#ifndef GB_CORE_IMPL_CUTTER_PREFIX_H
#define GB_CORE_IMPL_CUTTER_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../../path.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the cutter func type
 * 
 * @param code      the path code
 * @param point     the point 
 * @param priv      the user private data
 */
typedef tb_void_t   (*gb_cutter_func_t)(tb_size_t code, gb_point_ref_t point, tb_cpointer_t priv);


#endif


