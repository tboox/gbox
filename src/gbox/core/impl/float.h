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
 * @file        float.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_FLOAT_H
#define GB_CORE_IMPL_FLOAT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// is in the unit range(0, 1)?
#define gb_float_in_unit_range(x)       (gb_bz(x) && (x) < GB_ONE)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compute numer / denom and check it
 *
 * @param numer     the numerator
 * @param denom     the denominator
 * @param result    the result, must be in range: (0, 1)
 *
 * @return          the result is in range(0, 1): 1, otherwise: 0
 */
tb_size_t           gb_float_unit_divide(gb_float_t numer, gb_float_t denom, gb_float_t* result);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


