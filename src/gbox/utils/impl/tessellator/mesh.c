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
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_char_t const* gb_tessellator_edge_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_edge_ref_t edge = (gb_tessellator_edge_ref_t)data;
    tb_assert_and_check_return_val(edge, tb_null);

    // the edge base
    gb_mesh_edge_ref_t edge_base = ((gb_mesh_edge_ref_t)edge) - 1;

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "%{point}", gb_tessellator_vertex_point(gb_mesh_edge_org(edge_base)));
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
static tb_char_t const* gb_tessellator_face_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
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
static tb_char_t const* gb_tessellator_vertex_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_vertex_ref_t vertex = (gb_tessellator_vertex_ref_t)data;
    tb_assert_and_check_return_val(vertex, tb_null);

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "%{point}", &vertex->point);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
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
        tb_item_func_t edge_func    = tb_item_func_mem(sizeof(gb_tessellator_edge_t), tb_null, tb_null);
        tb_item_func_t face_func    = tb_item_func_mem(sizeof(gb_tessellator_face_t), tb_null, tb_null);
        tb_item_func_t vertex_func  = tb_item_func_mem(sizeof(gb_tessellator_vertex_t), tb_null, tb_null);

        // init func cstr
        edge_func.cstr      = gb_tessellator_edge_cstr;
        face_func.cstr      = gb_tessellator_face_cstr;
        vertex_func.cstr    = gb_tessellator_vertex_cstr;

        // init mesh
        impl->mesh = gb_mesh_init(edge_func, face_func, vertex_func);

        /* init the order
         *
         * the new edges/faces/vertice will be inserted to the head of list
         */
        gb_mesh_edge_order_set(impl->mesh,      GB_MESH_ORDER_INSERT_HEAD);
        gb_mesh_face_order_set(impl->mesh,      GB_MESH_ORDER_INSERT_HEAD);
        gb_mesh_vertex_order_set(impl->mesh,    GB_MESH_ORDER_INSERT_HEAD);
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
            // not closed contour? skip it
            if (!gb_point_eq(point, point + count - 1))
            {
                // trace
                tb_assertf_abort(0, "this contour(%lu: %{point} => %{point}) is not closed!", count, point, point + count - 1);

                // seek to the next contour
                points = point + count;
                count = *counts++;

                // continue 
                continue ;
            }

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

            // init face.inside
            gb_tessellator_face_inside_set(gb_mesh_edge_lface(edge), 0);
            gb_tessellator_face_inside_set(gb_mesh_edge_rface(edge), 0);

            // clear the first edge
            edge_first = tb_null;
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

            // init edge.dst
            gb_tessellator_vertex_point_set(gb_mesh_edge_dst(edge), *point);
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



