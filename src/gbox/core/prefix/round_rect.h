/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        round_rect.h
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
