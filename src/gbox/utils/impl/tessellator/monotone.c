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
#include "event_queue.h"
#include "active_region.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_tessellator_remove_degenerate_edges(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);
 
    // done
    gb_mesh_edge_ref_t edge_next    = tb_null;
    gb_mesh_edge_ref_t edge_lnext   = tb_null;
    gb_mesh_edge_ref_t edge         = gb_mesh_edge_head(mesh);
    gb_mesh_edge_ref_t tail         = gb_mesh_edge_tail(mesh);
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
    // check
    tb_assert_abort(impl);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);

    // done
    gb_mesh_edge_ref_t edge = tb_null;
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(mesh), face)
    {
        // the edge
        edge = gb_mesh_face_edge(face);
        tb_assert_abort(edge && gb_mesh_edge_lnext(edge) != edge);

        // the face is inside and degenerate? only with two edges?
        if (gb_tessellator_face_inside(face) && gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge)) == edge) 
        {
            // trace
            tb_trace_d("remove degenerate face");

            // clear inside
            gb_tessellator_face_inside_set(face, 0);
        }
    }
}
static tb_void_t gb_tessellator_sweep_event(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert_abort(impl && event);

    // update the current sweep event for the active_region.leq
    impl->event = event;

    // trace
    tb_trace_d("event: sweep: %{point}", gb_tessellator_vertex_point(event));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_tessellator_monotone_make(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && bounds);

    // trace
    tb_trace_d("bounds: %{rect}", bounds);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);

    // remove degenerate edges
    gb_tessellator_remove_degenerate_edges(impl);

    // make event queue
    if (!gb_tessellator_event_queue_make(impl)) return ;

    // the event queue
    tb_priority_queue_ref_t event_queue = impl->event_queue;
    tb_assert_abort(event_queue);

    // done
    while (tb_priority_queue_size(event_queue))
    {
        // get the minimum vertex event
        gb_mesh_vertex_ref_t event = tb_priority_queue_get(event_queue);
        tb_assert_abort(event);

        // pop it from the event queue first
        tb_priority_queue_pop(event_queue);

        // attempt to merge all vertices at same position as mush as possible
        while (tb_priority_queue_size(event_queue))
        {
            // get the next vertex event
            gb_mesh_vertex_ref_t event_next = tb_priority_queue_get(event_queue);
            tb_assert_abort(event_next);

            // two vertices are exactly same?
            tb_check_break(gb_tessellator_vertex_eq(event, event_next));

            // pop the next event from the event queue
            tb_priority_queue_pop(event_queue);

            // trace
            tb_trace_d("event: merge: %{point}", gb_tessellator_vertex_point(event));

            /* merge them if two vertices have same position and remove the next event vertex
             *
             * this is more efficient than processing them one at a time
             * and is also important for correct handling of certain degenerate cases.
             *
             * we can merge them safely because all zero-length edges have been removed.
             *
             *  .
             *   .      .
             *    .   .
             *     .. merge 
             *   .   .
             * .      .
             *         .
             */
            gb_mesh_edge_splice(mesh, gb_mesh_vertex_edge(event), gb_mesh_vertex_edge(event_next));
        }

        // sweep this event
        gb_tessellator_sweep_event(impl, event);
    }

#if 0
    // for testing single contour first
    gb_tessellator_face_inside_set(gb_mesh_face_last(impl->mesh), 1);
#endif

    // remove degenerate faces
    gb_tessellator_remove_degenerate_faces(impl);

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(mesh);
#endif
}


