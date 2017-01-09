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
 * @file        profiler.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_PROFILER_H
#define GB_UTILS_IMPL_TESSELLATOR_PROFILER_H

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

/*! init profiler file to the temporary directory, name: profiler_xxx.html 
 *
 * this is a html file using raphael.js(www.raphaeljs.com) and 
 * it will draw the details of making monotone mesh.
 *
 * we can profile and debug the monotone implementation using the generated html file.
 *
 * files:
 * - the temporary directory
 *   - raphael.js
 *   - profiler_0.html
 *   - profiler_1.html
 *   - profiler_2.html
 *   - ...
 *
 * @param bounds    the polygon bounds
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_tessellator_profiler_init(gb_rect_ref_t bounds);

/// exit the profiler
tb_void_t           gb_tessellator_profiler_exit(tb_noarg_t);

/*! add edge to the profiler
 *
 * @param edge      the edge
 */
tb_void_t           gb_tessellator_profiler_add_edge(gb_mesh_edge_ref_t edge);

/*! add split to the profiler
 *
 * @param edge      the edge
 */
tb_void_t           gb_tessellator_profiler_add_split(gb_mesh_edge_ref_t edge);

/*! add patch to the profiler
 *
 * @param edge      the edge
 */
tb_void_t           gb_tessellator_profiler_add_patch(gb_mesh_edge_ref_t edge);

/*! add intersection to the profiler
 *
 * @param inter     the intersection
 */
tb_void_t           gb_tessellator_profiler_add_inter(gb_mesh_vertex_ref_t inter);

/*! finish region
 *
 * @param region    the region
 */
tb_void_t           gb_tessellator_profiler_finish_region(gb_tessellator_active_region_ref_t region);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


