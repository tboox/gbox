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
 * @file        rect.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_RECT_H
#define GB_CORE_PREFIX_RECT_H

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

/*! make rect
 *
 * @param rect      the rect
 * @param x         the x
 * @param y         the y
 * @param w         the width
 * @param h         the height
 *
 */
tb_void_t           gb_rect_make(gb_rect_ref_t rect, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h);

/*! make rect with the integer value
 *
 * @param rect      the rect
 * @param x         the x
 * @param y         the y
 * @param w         the width
 * @param h         the height
 *
 */
tb_void_t           gb_rect_imake(gb_rect_ref_t rect, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h);

/*! apply matrix to rect
 *
 * @param rect      the rect 
 * @param matrix    the matrix
 */
tb_void_t           gb_rect_apply(gb_rect_ref_t rect, gb_matrix_ref_t matrix);

/*! apply matrix to rect
 *
 * @param rect      the rect 
 * @param applied   the applied rect 
 * @param matrix    the matrix
 */
tb_void_t           gb_rect_apply2(gb_rect_ref_t rect, gb_rect_ref_t applied, gb_matrix_ref_t matrix);

/*! inflate rect
 *
 * @param rect      the rect 
 * @param dx        the x-delta value and must be larger than zero
 * @param dy        the x-delta value and must be larger than zero
 */
tb_void_t           gb_rect_inflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy);

/*! deflate rect
 *
 * @param rect      the rect 
 * @param dx        the x-delta value and must be smaller than zero
 * @param dy        the x-delta value and must be smaller than zero
 */
tb_void_t           gb_rect_deflate(gb_rect_ref_t rect, gb_float_t dx, gb_float_t dy);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
