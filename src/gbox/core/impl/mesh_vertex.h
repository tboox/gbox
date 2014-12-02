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
 * @file        mesh_vertex.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_MESH_VERTEX_H
#define GB_CORE_IMPL_MESH_VERTEX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// declaration
struct __gb_mesh_edge_t;

// the mesh vertex type
typedef struct __gb_mesh_vertex_t
{
    // the next vertex for the doubly-linked list
    struct __gb_mesh_vertex_t*  next;

    // the prev vertex for the doubly-linked list
    struct __gb_mesh_vertex_t*  prev;

    // an arbitrary edge of the vertex
    struct __gb_mesh_edge_t*    edge;

}gb_mesh_vertex_t, *gb_mesh_vertex_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif

