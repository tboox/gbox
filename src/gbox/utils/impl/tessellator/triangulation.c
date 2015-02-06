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
 * @file        triangulation.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "triangulation"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "geometry.h"
#include "triangulation.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_tessellator_done_triangulation_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && face);

    tb_trace_d("before:");
    gb_mesh_dump(mesh);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);
    tb_assert_abort(edge && gb_mesh_edge_lnext(edge) != edge && gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge)) != edge);

    // get the rightmost upper edge
    gb_mesh_edge_ref_t up = edge;
    while (gb_tessellator_edge_go_left(up)) up = gb_mesh_edge_lprev(up);
    while (gb_tessellator_edge_go_right(up)) up = gb_mesh_edge_lnext(up);

    // get the rightmost lower edge
    gb_mesh_edge_ref_t lo = gb_mesh_edge_lprev(up);

    // done
    while (gb_mesh_edge_lnext(up) != lo) 
    {
        if (gb_tessellator_vertex_le_v(gb_mesh_edge_dst(up), gb_mesh_edge_org(lo))) 
        {
            while ( gb_mesh_edge_lnext(lo) != up
                &&  (   gb_tessellator_edge_go_left(gb_mesh_edge_lnext(lo))
                    ||  gb_tessellator_position_v(gb_mesh_edge_dst(lo), gb_mesh_edge_org(lo), gb_mesh_edge_dst(gb_mesh_edge_lnext(lo))) <= 0)) 
            {
                // connect it
                edge = gb_mesh_edge_connect(mesh, gb_mesh_edge_lnext(lo), lo);
                tb_assert_abort_and_check_return_val(edge, tb_false);

                // update the lower edge
                lo = gb_mesh_edge_sym(edge);
            }

            // the next lower edge
            lo = gb_mesh_edge_lprev(lo);
        } 
        else
        {
            while ( gb_mesh_edge_lnext(lo) != up
                && (    gb_tessellator_edge_go_right(gb_mesh_edge_lprev(up))
                    ||  gb_tessellator_position_v(gb_mesh_edge_org(up), gb_mesh_edge_dst(up), gb_mesh_edge_org(gb_mesh_edge_lprev(up))) >= 0)) 
            {
                // connect it
                edge = gb_mesh_edge_connect(mesh, up, gb_mesh_edge_lprev(up));
                tb_assert_abort_and_check_return_val(edge, tb_false);

                // update the upper edge
                up = gb_mesh_edge_sym(edge);
            }

            // the next upper edge
            up = gb_mesh_edge_lnext(up);
        }
    }

    tb_assert_abort(gb_mesh_edge_lnext(lo) != up);

    while (gb_mesh_edge_lnext(gb_mesh_edge_lnext(lo)) != up) 
    {
        // connect it
        edge = gb_mesh_edge_connect(mesh, gb_mesh_edge_lnext(lo), lo);
        tb_assert_abort_and_check_return_val(edge, tb_false);

        // the next lower edge
        lo = gb_mesh_edge_lprev(lo);
    }

#if 1
    tb_trace_d("after:");
    gb_mesh_dump(mesh);
#endif

    // ok
    return tb_true;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_done_triangulation(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->mesh);

    // the new face must be inserted to the head of faces
    tb_assert_abort(gb_mesh_face_order(impl->mesh) == GB_MESH_ORDER_INSERT_HEAD);

    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_face_itor(impl->mesh);
    tb_assert_abort(iterator);

    // done
    tb_size_t           itor = tb_iterator_head(iterator);
    tb_size_t           tail = tb_iterator_tail(iterator);
    gb_mesh_face_ref_t  face = tb_null;
    while (itor != tail)
    {
        // the face
        face = tb_iterator_item(iterator, itor);
        tb_assert_abort(face);

        /* the next face
         *
         * @note we don't process the new faces at the head 
         */
        itor = tb_iterator_next(iterator, itor);

        // the face is inside?
        if (gb_tessellator_face_inside(face)) 
        {
            // done triangulation for the face
            if (!gb_tessellator_done_triangulation_face(impl, face)) return tb_false;
        }
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif

    // ok
    return tb_true;
}


