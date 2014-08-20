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
 * @file        quality.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_QUALITY_H
#define GB_CORE_QUALITY_H

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


