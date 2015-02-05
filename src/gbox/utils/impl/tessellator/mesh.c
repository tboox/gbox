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
 * includes
 */
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
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
        // init vertex func
        tb_item_func_t vertex_func = tb_item_func_mem(sizeof(gb_tessellator_vertex_t), tb_null, tb_null);
        vertex_func.cstr = gb_tessellator_vertex_cstr;

        // init mesh
        impl->mesh = gb_mesh_init(  tb_item_func_mem(sizeof(gb_tessellator_edge_t), tb_null, tb_null)
                                ,   tb_item_func_mem(sizeof(gb_tessellator_face_t), tb_null, tb_null)
                                ,   vertex_func);

        // init the face order for triangulation
        gb_mesh_face_order_set(impl->mesh, GB_MESH_ORDER_INSERT_HEAD);
    }

    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort_and_check_return_val(mesh, tb_false);

    // clear mesh first
    gb_mesh_clear(mesh);

    // done
    tb_bool_t           ok          = tb_true;
    gb_point_ref_t      point       = tb_null;
    gb_point_ref_t      point_first = tb_null;
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
            // make a non-loop edge
            edge = gb_mesh_edge_make(mesh);

            // save the first point
            point_first = point;

            // save the first edge
            edge_first = edge;
        }
        // closed?
        else if (index + 1 == count && point_first->x == point->x && point_first->y == point->y)
        {
            // connect an edge to the first edge
            edge = gb_mesh_edge_connect(mesh, edge, edge_first);

            // init face.inside
            gb_tessellator_face_inside_set(gb_mesh_edge_lface(edge), 0);
            gb_tessellator_face_inside_set(gb_mesh_edge_rface(edge), 0);
        }
        else 
        {
            // append an edge
            edge = gb_mesh_edge_append(mesh, edge);
        }

        // check
        tb_assert_abort_and_check_break_state(edge, ok, tb_false);

        // init edge.winding
        gb_tessellator_edge_winding_set(edge, 1);
        gb_tessellator_edge_winding_set(gb_mesh_edge_sym(edge), -1);

        // init edge.org
        gb_tessellator_vertex_point_set(gb_mesh_edge_org(edge), *point);

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

    // failed? clear mesh
    if (!ok) gb_mesh_clear(mesh);

    // ok?
    return ok && !gb_mesh_is_empty(mesh);
}



