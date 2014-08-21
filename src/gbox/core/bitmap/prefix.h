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


