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
 * @file        arc.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_ARC_H
#define GB_CORE_PREFIX_ARC_H

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

/*! make arc
 *
 * @param arc       the arc
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_arc_make(gb_arc_ref_t arc, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! make arc with the integer value
 *
 * @param arc       the arc
 * @param x0        the x0
 * @param y0        the y0
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_arc_imake(gb_arc_ref_t arc, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
