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


