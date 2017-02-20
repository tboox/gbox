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
 * @file        mesh.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "tessellator_mesh"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
#ifdef __gb_debug__
static tb_char_t const* gb_tessellator_edge_cstr(tb_element_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_edge_ref_t edge = (gb_tessellator_edge_ref_t)data;
    tb_assert_and_check_return_val(edge, tb_null);

    // the edge base
    gb_mesh_edge_ref_t edge_base = ((gb_mesh_edge_ref_t)edge) - 1;

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "e%lu.w%d", edge_base->id, edge->winding);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
static tb_char_t const* gb_tessellator_face_cstr(tb_element_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_face_ref_t face = (gb_tessellator_face_ref_t)data;
    tb_assert_and_check_return_val(face, tb_null);

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "inside: %d", face->inside);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
static tb_char_t const* gb_tessellator_vertex_cstr(tb_element_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_vertex_ref_t vertex = (gb_tessellator_vertex_ref_t)data;
    tb_assert_and_check_return_val(vertex, tb_null);

    // the vertex base
    gb_mesh_vertex_ref_t vertex_base = ((gb_mesh_vertex_ref_t)vertex) - 1;

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "v%lu: %{point}", vertex_base->id, &vertex->point);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
#endif
static tb_void_t gb_tessellator_listener(gb_mesh_event_ref_t event)
{
    // check
    tb_assert(event);

    // done
    switch (event->type)
    {
    case GB_MESH_EVENT_FACE_SPLIT:
        {
            // the org and dst face
            gb_mesh_face_ref_t face_org = (gb_mesh_face_ref_t)event->org;
            gb_mesh_face_ref_t face_dst = (gb_mesh_face_ref_t)event->dst;

            /* split(face_org) => (face_org, face_dst)
             *
             * the new face will inherit the inside attribute of the old face
             */
            gb_tessellator_face_inside_set(face_dst, gb_tessellator_face_inside(face_org));
        }
        break;
    case GB_MESH_EVENT_EDGE_SPLIT:
        {
            // the org and dst edge
            gb_mesh_edge_ref_t edge_org = (gb_mesh_edge_ref_t)event->org;
            gb_mesh_edge_ref_t edge_dst = (gb_mesh_edge_ref_t)event->dst;

            /* split(edge_org) => (edge_org, edge_dst)
             *
             * the new edge will inherit the winding attribute of the old edge
             */
            gb_tessellator_edge_winding_set(edge_dst, gb_tessellator_edge_winding(edge_org));
            gb_tessellator_edge_winding_set(gb_mesh_edge_sym(edge_dst), gb_tessellator_edge_winding(gb_mesh_edge_sym(edge_org)));
        }
        break;
    default:
        tb_assertf(0, "unknown listener event: %lx", event->type);
        break;
    }
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_mesh_make(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon)
{
    // check
    tb_assert(impl && polygon);

    // the points
    gb_point_ref_t      points = polygon->points;
    tb_uint16_t const*  counts = polygon->counts;
    tb_assert_and_check_return_val(points && counts, tb_false);

    // not exists mesh?
    if (!impl->mesh) 
    {
        // init func
        tb_element_t edge_element    = tb_element_mem(sizeof(gb_tessellator_edge_t), tb_null, tb_null);
        tb_element_t face_element    = tb_element_mem(sizeof(gb_tessellator_face_t), tb_null, tb_null);
        tb_element_t vertex_element  = tb_element_mem(sizeof(gb_tessellator_vertex_t), tb_null, tb_null);

#ifdef __gb_debug__
        // init func cstr for gb_mesh_dump
        edge_element.cstr      = gb_tessellator_edge_cstr;
        face_element.cstr      = gb_tessellator_face_cstr;
        vertex_element.cstr    = gb_tessellator_vertex_cstr;
#endif

        // init mesh
        impl->mesh = gb_mesh_init(edge_element, face_element, vertex_element);

        /* init the order
         *
         * the new edges/faces/vertice will be inserted to the head of list
         */
        gb_mesh_edge_order_set(impl->mesh,      GB_MESH_ORDER_INSERT_HEAD);
        gb_mesh_face_order_set(impl->mesh,      GB_MESH_ORDER_INSERT_HEAD);
        gb_mesh_vertex_order_set(impl->mesh,    GB_MESH_ORDER_INSERT_HEAD);

        // init listener
        gb_mesh_listener_set(impl->mesh, gb_tessellator_listener, impl->mesh);
        gb_mesh_listener_event_add(impl->mesh, GB_MESH_EVENT_FACE_SPLIT | GB_MESH_EVENT_EDGE_SPLIT);
    }

    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_and_check_return_val(mesh, tb_false);

    // clear mesh first
    gb_mesh_clear(mesh);

    // done
    gb_point_ref_t      point       = tb_null;
    tb_uint16_t         count       = *counts++;
    tb_size_t           index       = 0;
    gb_mesh_edge_ref_t  edge        = tb_null;
    gb_mesh_edge_ref_t  edge_first  = tb_null;
    while (index < count)
    {
        // the point
        point = points++;

        // first point?
        if (!index) 
        {
            // must be closed contour
            tb_assertf(gb_point_eq(point, point + count - 1), "this contour(%lu: %{point} => %{point}) is not closed!", count, point, point + count - 1);
            
            // clear the edge
            edge = tb_null;

            // clear the first edge
            edge_first = tb_null;

            // trace
            tb_trace_d("move_to: %{point}", point);
        }
        // closed?
        else if (index + 1 == count)
        {
            // trace
            tb_trace_d("closed: %{point}", point);

            // connect an edge to the first edge
            edge = gb_mesh_edge_connect(mesh, edge, edge_first);

            // init edge.faces.inside
            gb_tessellator_face_inside_set(gb_mesh_edge_lface(edge), 0);
            gb_tessellator_face_inside_set(gb_mesh_edge_rface(edge), 0);
        }
        else 
        {
            // trace
            tb_trace_d("line_to: %{point}", point);

            // exists the first edge?
            if (edge_first)
            {
                // append an edge
                edge = gb_mesh_edge_append(mesh, edge);
            }
            else
            {
                // make a new non-loop edge
                edge = gb_mesh_edge_make(mesh);

                // save the first edge
                edge_first = edge;
            }
        }

        // has new edge?
        if (edge)
        {
            // init edge.winding
            gb_tessellator_edge_winding_set(edge, 1);
            gb_tessellator_edge_winding_set(gb_mesh_edge_sym(edge), -1);

            // init edge.region
            gb_tessellator_edge_region_set(edge, tb_null);
            gb_tessellator_edge_region_set(gb_mesh_edge_sym(edge), tb_null);

            // init edge.dst
            gb_tessellator_vertex_point_set(gb_mesh_edge_dst(edge), point);
        }

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(mesh);
#endif

    // ok?
    return !gb_mesh_is_empty(mesh);
}
gb_mesh_edge_ref_t gb_tessellator_mesh_make_edge(gb_tessellator_impl_t* impl, gb_point_ref_t org, gb_point_ref_t dst)
{
    // check
    tb_assert(impl && impl->mesh);

    // make edge
    gb_mesh_edge_ref_t edge = gb_mesh_edge_make(impl->mesh);
    tb_assert_and_check_return_val(edge, tb_null);

    // init edge.winding
    gb_tessellator_edge_winding_set(edge, 0);
    gb_tessellator_edge_winding_set(gb_mesh_edge_sym(edge), 0);

    // init edge.region
    gb_tessellator_edge_region_set(edge, tb_null);
    gb_tessellator_edge_region_set(gb_mesh_edge_sym(edge), tb_null);

    // init edge.faces.inside, lface == rface
    gb_tessellator_face_inside_set(gb_mesh_edge_lface(edge), 0);

    // init edge.org
    if (org) gb_tessellator_vertex_point_set(gb_mesh_edge_org(edge), org);

    // init edge.dst
    if (dst) gb_tessellator_vertex_point_set(gb_mesh_edge_dst(edge), dst);

    // ok
    return edge;
}

