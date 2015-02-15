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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        monotone.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_MONOTONE_H
#define GB_UTILS_IMPL_TESSELLATOR_MONOTONE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make horizontal monotone region for mesh
 *   
 * monotone:
 *
 *   . . .
 *  .     .
 * . ----- .
 * .      .
 * . -----  .
 * .           .
 * . . . . . . . .
 *
 * not monotone:
 *
 *   . . .
 *  .     .
 * . ----- .
 * .      .    ........
 * . -----  .  . ---- .  <= two segments 
 * .           .      .
 * .                  .
 * . . . . . . . . . ..
 *
 * @param impl      the tessellator impl
 * @param bounds    the polygon bounds
 */
tb_void_t           gb_tessellator_make_monotone(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


