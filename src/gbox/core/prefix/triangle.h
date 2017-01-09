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
 * @file        triangle.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_TRIANGLE_H
#define GB_CORE_PREFIX_TRIANGLE_H

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

/*! make triangle
 *
 * @param triangle  the triangle
 * @param x0        the x0
 * @param y0        the y0
 * @param x1        the x1
 * @param y1        the y1
 * @param x2        the x2
 * @param y2        the y2
 */
tb_void_t           gb_triangle_make(gb_triangle_ref_t triangle, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! make triangle with the integer value
 *
 * @param triangle  the triangle
 * @param x0        the x0
 * @param y0        the y0
 * @param x1        the x1
 * @param y1        the y1
 * @param x2        the x2
 * @param y2        the y2
 */
tb_void_t           gb_triangle_imake(gb_triangle_ref_t triangle, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! apply matrix to triangle
 *
 * @param triangle      the triangle 
 * @param matrix    the matrix
 */
tb_void_t           gb_triangle_apply(gb_triangle_ref_t triangle, gb_matrix_ref_t matrix);

/*! apply matrix to triangle
 *
 * @param triangle      the triangle 
 * @param applied   the applied triangle 
 * @param matrix    the matrix
 */
tb_void_t           gb_triangle_apply2(gb_triangle_ref_t triangle, gb_triangle_ref_t applied, gb_matrix_ref_t matrix);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
