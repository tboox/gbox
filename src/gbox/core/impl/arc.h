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
 * @file        arc.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_ARC_H
#define GB_CORE_IMPL_ARC_H

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

// tan(pi/8) * 4 / 3
#ifdef GB_CONFIG_FLOAT_FIXED
#   define GB_ARC_MAKE_CUBIC_FACTOR     (36195)
#else
#   define GB_ARC_MAKE_CUBIC_FACTOR     (0.5522847498f)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the arc quad func type
 * 
 * @param ctrl      the ctrl point, the point is first if be null
 * @param point     the point 
 * @param priv      the user private data
 */
typedef tb_void_t   (*gb_arc_quad_func_t)(gb_point_ref_t ctrl, gb_point_ref_t point, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make the quad curves for the arc
 *
 * @param arc       the arc
 * @param func      the make func
 * @param priv      the make func private data for user
 */
tb_void_t           gb_arc_make_quad(gb_arc_ref_t arc, gb_arc_quad_func_t func, tb_cpointer_t priv);

/* make the quad curves for the arc
 *
 * @param start     the start unit vector
 * @param stop      the stop unit vector
 * @param matrix    the user matrix
 * @param func      the make func
 * @param priv      the make func private data for user
 */
tb_void_t           gb_arc_make_quad2(gb_vector_ref_t start, gb_vector_ref_t stop, gb_matrix_ref_t matrix, tb_size_t direction, gb_arc_quad_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


