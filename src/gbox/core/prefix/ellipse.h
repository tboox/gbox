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
 * @file        ellipse.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_ELLIPSE_H
#define GB_CORE_PREFIX_ELLIPSE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! make ellipse
 *
 * @param ellipse   the ellipse
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_ellipse_make(gb_ellipse_ref_t ellipse, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry);

/*! make ellipse with the integer value
 *
 * @param ellipse   the ellipse
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_ellipse_imake(gb_ellipse_ref_t ellipse, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry);

/*! make ellipse from rect
 *
 * @param ellipse   the ellipse
 * @param rect      the rect
 */
tb_void_t           gb_ellipse_make_from_rect(gb_ellipse_ref_t ellipse, gb_rect_ref_t rect);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
