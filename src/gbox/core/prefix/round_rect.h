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
 * @file        rect.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_ROUND_RECT_H
#define GB_CORE_PREFIX_ROUND_RECT_H

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

/*! make round rect
 *
 * @param rect      the rect 
 * @param bounds    the bounds 
 * @param radius    the radius of the four corners
 */
tb_void_t           gb_round_rect_make(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_vector_t radius[]);

/*! make round rect using the same x and y radius
 *
 * @param rect      the rect 
 * @param bounds    the bounds 
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_round_rect_make_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry);

/*! make round rect using the same integer x and y radius
 *
 * @param rect      the rect 
 * @param bounds    the bounds 
 * @param rx        the x-radius
 * @param ry        the y-radius
 */
tb_void_t           gb_round_rect_imake_same(gb_round_rect_ref_t rect, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry);

/*! the round rect is rect?
 *
 * @param rect      the rect 
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_round_rect_is_rect(gb_round_rect_ref_t rect);

/*! the round rect is ellipse?
 *
 * @param rect      the rect 
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_round_rect_is_ellipse(gb_round_rect_ref_t rect);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
