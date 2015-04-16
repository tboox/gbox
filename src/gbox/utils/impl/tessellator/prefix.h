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

// the winding of tessellator edge 
#define gb_tessellator_edge_winding(edge)               (gb_tessellator_edge(edge)->winding)

// set the winding of tessellator edge 
#define gb_tessellator_edge_winding_set(edge, val)      do { gb_tessellator_edge(edge)->winding = (val); } while (0)

// add the winding of tessellator edge 
#define gb_tessellator_edge_winding_add(edge, val)      do { gb_tessellator_edge(edge)->winding += (val); } while (0)

/* merge the winding of two tessellator edges
 *
 * when we merge two edges into one, we need to compute the combined winding of the new edge.
 */
#define gb_tessellator_edge_winding_merge(edge, other)  do { gb_tessellator_edge_winding_add(edge, gb_tessellator_edge_winding(other)); gb_tessellator_edge_winding_add(gb_mesh_edge_sym(edge), gb_tessellator_edge_winding(gb_mesh_edge_sym(other))); } while (0)

// the region of tessellator edge 
#define gb_tessellator_edge_region(edge)                (gb_tessellator_edge(edge)->region)

// set the region of tessellator edge 
#define gb_tessellator_edge_region_set(edge, val)       do { gb_tessellator_edge(edge)->region = (val); } while (0)

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
#define gb_tessellator_vertex_point_set(vertex, val)    do { gb_tessellator_vertex(vertex)->point = *(val); } while (0)

// define the local tessellator vertex variable
#define gb_tessellator_vertex_local(name) \
    tb_byte_t __name##_data[sizeof(gb_mesh_vertex_t) + sizeof(gb_tessellator_vertex_t)]; \
    gb_mesh_vertex_ref_t name = (gb_mesh_vertex_ref_t)__name##_data;

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the active region type
 *
 *  /.\                                     .              /.\
 *   .                 .                   .   .            .
 *   .               .   .                .       .         .
 *   .  region1    .       .             .           .      .
 *   .           .           .  region3 .                   .
 *   . ------- . - region2 --- . ---- event --------------- . ----- sweep line
 *   .       .                   .     .                    .
 *   .    edge2                edge3   .    region4         . region5
 *   .                                 .                    .
 * edge1                             edge4                edge5
 *(bound)                                                (bound)
 */
typedef struct __gb_tessellator_active_region_t
{
    // the region position
    tb_size_t                           position;

    // the left edge and it goes up
    gb_mesh_edge_ref_t                  edge;

    // the winding
    tb_long_t                           winding;

    // is inside?
    tb_uint8_t                          inside  : 1;

    // is bounds?
    tb_uint8_t                          bounds  : 1;

    /* the left edge need fix?
     *
     * see gb_tessellator_connect_bottom_event
     */
    tb_uint8_t                          fixedge : 1;

}gb_tessellator_active_region_t, *gb_tessellator_active_region_ref_t;

// the tessellator edge type
typedef struct __gb_tessellator_edge_t
{
    // the winding
    tb_int32_t                          winding;

    // the action region of the edge
    gb_tessellator_active_region_ref_t  region;

} gb_tessellator_edge_t, *gb_tessellator_edge_ref_t;

// the tessellator face type
typedef struct __gb_tessellator_face_t
{
    // is inside?
    tb_uint8_t                          inside      : 1;

} gb_tessellator_face_t, *gb_tessellator_face_ref_t;

// the tessellator vertex type
typedef struct __gb_tessellator_vertex_t
{
    // the point
    gb_point_t                          point;

} gb_tessellator_vertex_t, *gb_tessellator_vertex_ref_t;

// the tessellator impl type
typedef struct __gb_tessellator_impl_t
{
    // the mode
    tb_size_t                           mode;

    // the winding rule
    tb_size_t                           rule;

    // the func
    gb_tessellator_func_t               func;

    // the user private data
    tb_cpointer_t                       priv;

    // the mesh
    gb_mesh_ref_t                       mesh;

    // the current sweep event for the active_region.leq
    gb_mesh_vertex_ref_t                event;

    // the output points
    tb_vector_ref_t                     outputs;

    // the event queue for vertex
    tb_priority_queue_ref_t             event_queue;

    // the active regions
    tb_list_ref_t                       active_regions;

}gb_tessellator_impl_t;

#endif


