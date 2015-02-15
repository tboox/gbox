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
 * @file        monotone.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "monotone"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "monotone.h"
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_tessellator_remove_degenerate_edges(gb_tessellator_impl_t* impl)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh);

    // done
    gb_mesh_edge_ref_t edge_next    = tb_null;
    gb_mesh_edge_ref_t edge_lnext   = tb_null;
    gb_mesh_edge_ref_t edge = gb_mesh_edge_head(mesh);
    gb_mesh_edge_ref_t tail = gb_mesh_edge_tail(mesh);
    for (; edge != tail; edge = edge_next) 
    {
        // the next edge
        edge_next = gb_mesh_edge_next(edge);

        // the lnext edge
        edge_lnext = gb_mesh_edge_lnext(edge);

        // the length of edge is zero and the contour has at least three edges?
        if (    gb_tessellator_vertex_eq(gb_mesh_edge_org(edge), gb_mesh_edge_dst(edge))
            &&  gb_mesh_edge_lnext(edge_lnext) != edge) 
        {
            // trace
            tb_trace_d("remove degenerate edge");

            /* remove this edge
             *
             * before:
             *
             *          edge
             * <--------.<-.<--------
             *
             * after:
             *
             * <--------<--------
             */
            gb_mesh_edge_remove(mesh, edge);

            // update the next edge
            edge = edge_lnext;

            // update the lnext edge
            edge_lnext = gb_mesh_edge_lnext(edge);
        }

        // the contour is degenerate, only has one or two edges?
        if (gb_mesh_edge_lnext(edge_lnext) == edge) 
        {
            // trace
            tb_trace_d("remove degenerate contour");

            // two edges?
            if (edge_lnext != edge) 
            {
                // the next edge will be deleted? update the next edge
                if (edge_lnext == edge_next || edge_lnext == gb_mesh_edge_sym(edge_next))
                    edge_next = gb_mesh_edge_next(edge_next);
                
                // delete the lnext edge
                gb_mesh_edge_delete(mesh, edge_lnext);
            }

            // the next edge will be deleted? update the next edge
            if (edge == edge_next || edge == gb_mesh_edge_sym(edge_next))
                edge_next = gb_mesh_edge_next(edge_next);
            
            // delete this edge
            gb_mesh_edge_delete(mesh, edge);
        }
    }
}
static tb_void_t gb_tessellator_remove_degenerate_faces(gb_tessellator_impl_t* impl)
{
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_tessellator_make_monotone(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && impl->mesh && bounds);

    // remove degenerate edges
    gb_tessellator_remove_degenerate_edges(impl);

    // TODO
    tb_trace_noimpl();

    // for testing single contour first
    gb_tessellator_face_inside_set(gb_mesh_face_last(impl->mesh), 1);

    // remove degenerate faces
    gb_tessellator_remove_degenerate_faces(impl);

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif
}


