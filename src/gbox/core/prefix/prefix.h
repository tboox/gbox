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
 */
#ifndef GB_CORE_PREFIX_PREFIX_H
#define GB_CORE_PREFIX_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"
#include "type.h"
#include "rect.h"
#include "float.h"
#include "color.h"
#include "pixfmt.h"
#include "matrix.h"
#include "quality.h"
#include "round_rect.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// the width maxn
#define GB_WIDTH_MAXN           (8192)

/// the height maxn
#define GB_HEIGHT_MAXN          (8192)

/*! the min-alpha
 *
 * is_transparent = alpha < GB_ALPHA_MINN? tb_true : tb_false
 */
#define GB_ALPHA_MINN           ((tb_byte_t)((GB_QUALITY_TOP - gb_quality()) << 3))

/*! the max-alpha 
 *
 * @code
 * has_alpha = alpha < GB_QUALITY_ALPHA_MAXN? tb_true : tb_false
 * @endcode
 */
#define GB_ALPHA_MAXN           ((tb_byte_t)(0xff - ((GB_QUALITY_TOP - gb_quality()) << 3)))

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init prefix
 *
 * @return  tb_true or tb_false
 */
tb_bool_t   gb_prefix_init(tb_noarg_t);

/*! exit prefix
 */
tb_void_t   gb_prefix_exit(tb_noarg_t);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif


