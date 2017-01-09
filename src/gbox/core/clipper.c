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
 * @file        clipper.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "clipper"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "clipper.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the clipper impl type
typedef struct __gb_clipper_impl_t
{


}gb_clipper_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

gb_clipper_ref_t gb_clipper_init()
{
    return (gb_clipper_ref_t)1;
}
tb_void_t gb_clipper_exit(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
}
tb_size_t gb_clipper_size(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
    return 0;
}
tb_void_t gb_clipper_clear(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_copy(gb_clipper_ref_t clipper, gb_clipper_ref_t copied)
{
    tb_trace_noimpl();
}
gb_matrix_ref_t gb_clipper_matrix(gb_clipper_ref_t clipper)
{
    tb_trace_noimpl();
    return tb_null;
}
tb_void_t gb_clipper_matrix_set(gb_clipper_ref_t clipper, gb_matrix_ref_t matrix)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_path(gb_clipper_ref_t clipper, tb_size_t mode, gb_path_ref_t path)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_triangle(gb_clipper_ref_t clipper, tb_size_t mode, gb_triangle_ref_t triangle)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_rect_ref_t rect)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_round_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_round_rect_ref_t rect)
{
}
tb_void_t gb_clipper_add_circle(gb_clipper_ref_t clipper, tb_size_t mode, gb_circle_ref_t circle)
{
    tb_trace_noimpl();
}
tb_void_t gb_clipper_add_ellipse(gb_clipper_ref_t clipper, tb_size_t mode, gb_ellipse_ref_t ellipse)
{
    tb_trace_noimpl();
}

