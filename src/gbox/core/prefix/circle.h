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
 * @file        circle.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_CIRCLE_H
#define GB_CORE_PREFIX_CIRCLE_H

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

/*! make circle
 *
 * @param circle    the circle
 * @param x0        the x0
 * @param y0        the y0
 * @param r         the radius
 */
tb_void_t           gb_circle_make(gb_circle_ref_t circle, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! make circle with the integer value
 *
 * @param circle    the circle
 * @param x0        the x0
 * @param y0        the y0
 * @param r         the radius
 */
tb_void_t           gb_circle_imake(gb_circle_ref_t circle, tb_long_t x0, tb_long_t y0, tb_size_t r);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
