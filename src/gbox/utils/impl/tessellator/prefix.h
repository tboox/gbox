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
 * @file        prefix.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_PREFIX_H
#define GB_UTILS_IMPL_TESSELLATOR_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../../mesh.h"
#include "../../tessellator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the tessellator edge
#define gb_tessellator_edge(edge)                       ((gb_tessellator_edge_ref_t)gb_mesh_edge_data_fastly(edge))

// the tessellator edge winding
#define gb_tessellator_edge_winding(edge)               (gb_tessellator_edge(edge)->winding)

// set the tessellator edge winding
#define gb_tessellator_edge_winding_set(edge, val)      do { gb_tessellator_edge(edge)->winding = (val); } while (0)

// the tessellator face
#define gb_tessellator_face(face)                       ((gb_tessellator_face_ref_t)gb_mesh_face_data_fastly(face))

// the tessellator face inside
#define gb_tessellator_face_inside(face)                (gb_tessellator_face(face)->inside)

// set the tessellator face inside
#define gb_tessellator_face_inside_set(face, val)       do { gb_tessellator_face(face)->inside = (val); } while (0)

// the tessellator vertex
#define gb_tessellator_vertex(vertex)                   ((gb_tessellator_vertex_ref_t)gb_mesh_vertex_data_fastly(vertex))

// the tessellator vertex point
#define gb_tessellator_vertex_point(vertex)             (&(gb_tessellator_vertex(vertex)->point))

// set the tessellator vertex point
#define gb_tessellator_vertex_point_set(vertex, val)    do { gb_tessellator_vertex(vertex)->point = (val); } while (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the tessellator edge type
typedef struct __gb_tessellator_edge_t
{
    // the winding
    tb_int32_t              winding;

} gb_tessellator_edge_t, *gb_tessellator_edge_ref_t;

// the tessellator face type
typedef struct __gb_tessellator_face_t
{
    // is inside?
    tb_uint8_t              inside : 1;

} gb_tessellator_face_t, *gb_tessellator_face_ref_t;

// the tessellator vertex type
typedef struct __gb_tessellator_vertex_t
{
    // the point
    gb_point_t              point;

} gb_tessellator_vertex_t, *gb_tessellator_vertex_ref_t;

// the tessellator impl type
typedef struct __gb_tessellator_impl_t
{
    // the mode
    tb_size_t               mode;

    // the winding rule
    tb_size_t               rule;

    // the func
    gb_tessellator_func_t   func;

    // the user private data
    tb_cpointer_t           priv;

    // the mesh
    gb_mesh_ref_t           mesh;

    // the output points
    tb_vector_ref_t         outputs;

}gb_tessellator_impl_t;

#endif


