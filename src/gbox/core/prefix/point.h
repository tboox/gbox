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
 * @file        point.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_POINT_H
#define GB_CORE_PREFIX_POINT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// points is equal?
#define gb_point_eq(a, b)       ((a)->x == (b)->x && (a)->y == (b)->y)

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! make point
 *
 * @param point     the point
 * @param x         the x
 * @param y         the y
 */
tb_void_t           gb_point_make(gb_point_ref_t point, gb_float_t x, gb_float_t y);

/*! make point with the integer value
 *
 * @param point     the point
 * @param x         the x
 * @param y         the y
 */
tb_void_t           gb_point_imake(gb_point_ref_t point, tb_long_t x, tb_long_t y);

/*! apply matrix to point
 *
 * @param point      the point 
 * @param matrix    the matrix
 */
tb_void_t           gb_point_apply(gb_point_ref_t point, gb_matrix_ref_t matrix);

/*! apply matrix to point
 *
 * @param point      the point 
 * @param applied   the applied point 
 * @param matrix    the matrix
 */
tb_void_t           gb_point_apply2(gb_point_ref_t point, gb_point_ref_t applied, gb_matrix_ref_t matrix);

/*! the distance of the point to the other point
 *
 * @param point     the point
 * @param other     the other point
 */
gb_float_t          gb_point_distance(gb_point_ref_t point, gb_point_ref_t other);

/*! be equal to the other point?
 *
 * @param point     the point
 * @param other     the other point
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_point_near_eq(gb_point_ref_t point, gb_point_ref_t other);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
