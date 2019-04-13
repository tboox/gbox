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
 * @file        prefix.h
 */
#ifndef GB_CORE_PREFIX_PREFIX_H
#define GB_CORE_PREFIX_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"
#include "type.h"
#include "arc.h"
#include "line.h"
#include "rect.h"
#include "point.h"
#include "float.h"
#include "color.h"
#include "circle.h"
#include "pixfmt.h"
#include "vector.h"
#include "matrix.h"
#include "ellipse.h"
#include "quality.h"
#include "triangle.h"
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
 * has_alpha = alpha <= GB_QUALITY_ALPHA_MAXN? tb_true : tb_false
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


