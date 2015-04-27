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
    tb_assert_abort(event);

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
    default:
        tb_assertf_abort(0, "unknown listener event: %lx", event->type);
        break;
    }
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_mesh_make(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_abort(impl && polygon);

    // the points
    gb_point_ref_t      points = polygon->points;
    tb_uint16_t const*  counts = polygon->counts;
    tb_assert_abort_and_check_return_val(points && counts, tb_false);

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
        gb_mesh_listener_event_add(impl->mesh, GB_MESH_EVENT_FACE_SPLIT);
    }

    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort_and_check_return_val(mesh, tb_false);

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
            tb_assertf_abort(gb_point_eq(point, point + count - 1), "this contour(%lu: %{point} => %{point}) is not closed!", count, point, point + count - 1);
            
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
    tb_assert_abort(impl && impl->mesh);

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

