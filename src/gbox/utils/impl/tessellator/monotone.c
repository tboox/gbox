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
 * declaration
 */
static tb_void_t gb_tessellator_sweep_event(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event);

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tb_inline__ tb_uint8_t gb_tessellator_winding_is_inside(gb_tessellator_impl_t* impl, tb_int32_t winding)
{
    // done
    switch (impl->rule)
    {
        case GB_TESSELLATOR_RULE_ODD:       return (winding & 1);
        case GB_TESSELLATOR_RULE_NONZERO:   return (winding != 0);
        default:                            break;
    }

    // error
    tb_assert_abort(0);
    return 0;
}
static gb_tessellator_active_region_ref_t gb_tessellator_insert_region_at_left(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_right, gb_mesh_edge_ref_t edge_new)
{
    // check
    tb_assert_abort(impl && region_right && edge_new);

    // make region
    gb_tessellator_active_region_t region;
    region.edge     = edge_new;
    region.winding  = 0;
    region.inside   = 0;
    region.bounds   = 0;

    // insert region before the right region
    return gb_tessellator_active_regions_insert_before(impl, region_right, &region);
}
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
/* connect this top event to the processed portion of the mesh
 *
 * - the normal case:
 *
 *   - split this region to two regions by connecting this top event to 
 *     the lower destinate vertex of the left or right edge 
 *     if the contained(left) region is marked "inside"
 *
 *   - only add all edges to the mesh if the the contained(left) region is not marked "inside"
 *
 * - the degenerate case:
 *
 */
static tb_void_t gb_tessellator_connect_top_event(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->active_regions && event);

    // the edge of the event vertex
    gb_mesh_edge_ref_t edge_event = gb_mesh_vertex_edge(event);
    tb_assert_abort(edge_event);

    // the going-up edge of the event vertex
    gb_mesh_edge_ref_t edge_event_up = gb_mesh_edge_sym(edge_event);
    tb_assert_abort(edge_event_up);

    // get the left region containing this event from the going-up edge
    gb_tessellator_active_region_ref_t region_left = gb_tessellator_active_regions_find(impl, edge_event_up);
    tb_assert_abort(region_left);

    // get the right region
    gb_tessellator_active_region_ref_t region_right = gb_tessellator_active_regions_right(impl, region_left);
    tb_assert_abort(region_right);

    // the left and right edge containing this event
    gb_mesh_edge_ref_t edge_left    = region_left->edge;
    gb_mesh_edge_ref_t edge_right   = region_right->edge;
    tb_assert_abort(edge_left);
    tb_assert_abort(edge_right);

    // TODO: done degenerate
    // ...

    // get the region which edge.dst is lower and we need connect it
    gb_tessellator_active_region_ref_t region_lower = gb_tessellator_vertex_in_top(gb_mesh_edge_dst(edge_left), gb_mesh_edge_dst(edge_right))? region_right : region_left;

    // we need split it if the contained(left) region is marked "inside" 
    if (region_left->inside)
    {
        /* we need connect the top event to it if the destinate vertex of the left edge is lower
         *
         *                                                             .
         *          .                                                    .  
         *       .                                                         . edge_right
         *    .                                                            . (upper)
         *  . edge_left (lower)                                            .
         *  .   .                                                          .
         *  .      .                         region_new                    .
         *  .         .                                                    .
         *  .            . edge_new                                        .
         *  . region_left   .                                              .
         *  .   (inside)       .    (top)                                  .
         *  .            -------- . event -------------------------------- . ------------------- sweep line      
         *  .                   . . .                                      .  
         * /.\                .   .   .                                   /.\
         *  .               .    /.\                                       .
         *  .             .       .                                        .  region_right    
         *  .                     . edge_event_up                          .           
         *  .                                                              . 
         *  .                  new edges                                   .
         *  .                                                              .
         */
        gb_mesh_edge_ref_t edge_new = tb_null;
        if (region_lower == region_left)
        {
            // split the left region to two regions by connecting it
            edge_new = gb_mesh_edge_connect(impl->mesh, edge_event_up, gb_mesh_edge_rnext(edge_left));
        }
        /* we need connect the top event to it if the destinate vertex of the right edge is lower
         *
         *        .
         *     .
         *  . edge_left 
         *  .  (upper)
         *  .                                                          .
         *  .                                                            .  
         *  .           region_left (inside)                               . edge_right
         *  .                                                          .   . (lower)
         *  .                                                     .        .
         *  .                                     edge_new   .             .
         *  .                                           .                  .
         *  .                                      .                       .
         *  .                                 .             region_new     .
         *  .                            .                                 .
         *  .                       (top)                                  .
         *  .            -------- . event -------------------------------- . ------------------- sweep line      
         *  .                   . . .                                      .  
         * /.\                .   .   .                                   /.\
         *  .               .    /.\                                       .
         *  .             .       .                                        .  region_right    
         *  .                     .  edge_event_up                         .           
         *  .                                                              . 
         *  .                  new edges                                   .
         *  .                                                              .
         */
        else
        {
            // split the left region to two regions by connecting it
            edge_new = gb_mesh_edge_connect(impl->mesh, edge_event_up, gb_mesh_edge_lnext(edge_right));
        }

        // check
        tb_assert_abort(edge_new);

        // insert a new region with the new edge at the left of the right region
        gb_tessellator_active_region_ref_t region_new = gb_tessellator_insert_region_at_left(impl, region_right, edge_new);
        tb_assert_abort(region_new);

        /* check region
         *
         * region_left => region_new => region_right
         */
        tb_assert_abort(gb_tessellator_active_regions_in_left(impl, region_left, region_new));
        tb_assert_abort(gb_tessellator_active_regions_in_left(impl, region_new, region_right));

        // compute the winding of the new region
        region_new->winding = region_left->winding + gb_tessellator_edge_winding(edge_new);

        // mark it if the new region inside the polygon
        region_new->inside = gb_tessellator_winding_is_inside(impl, region_new->winding);

        // TODO
        gb_tessellator_sweep_event(impl, event);
    }
    // only add all edges to the mesh
    else
    {
        // TODO
    }
}
/* process one event vertex at the sweep line
 *
 *      event
 *        . ------------------------ sweep line
 * . --- . --- . -------------------       
 *   .  .          . 
 *     . ------------- . -----------
 *   .  .               .
 *  .     .               . --------
 * . ------ . ----------------------
 */
static tb_void_t gb_tessellator_sweep_event(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert_abort(impl && event);

    // update the current sweep event for the active_region.leq
    impl->event = event;

    // trace
    tb_trace_d("event: sweep: %{point}", gb_tessellator_vertex_point(event));

    // find an active region of all edges at this event 
    gb_mesh_edge_ref_t                  edge = gb_mesh_vertex_edge(event);
    gb_mesh_edge_ref_t                  scan = edge;
    gb_tessellator_active_region_ref_t  region = tb_null;
    while (!(region = gb_tessellator_edge_region(scan)))
    {
        // the next edge
        scan = gb_mesh_edge_onext(scan);

        // end?
        tb_check_break(scan != edge);
    }

    /* the active regions of all up-going edges at this event have been not finished.
     *                                       .
     *   .                 .               .
     *    .                  .   region  .
     *     . region            .       .
     *      .                    .   .
     * event . ------------------- . event ------------ sweep line
     *          .       or       .   .
     *             .           .       .
     *                .      .
     *
     */
    if (region)
    {
        /* we need close off them first if the left and right edges of these regions terminate 
         * at this event. 
         *
         * then we mark these faces "inside" or "outside" the polygon according 
         * to their winding number and remove these regions.
         *
         *                                       .
         *                     .    (close it) .
         *                       .   region1 . 
         *                         .       .  region2
         *                           .   .
         *     ----------------------- . event ------------ sweep line
         *                           .   .
         *                         .       .
         *                       .
         *
         */
        // TODO

        /* next we process all the down-going edges at this event. 
         * 
         * we will create a new active region with new edge
         *
         *                     .   (finished)  .
         *                       .   region1 . 
         *                         .       . region2
         *                           .   .
         *     ----------------------- . event ------------ sweep line
         *                           .   .
         *                         .       .
         *                       .
         */
        // TODO
    }
    /* all edges are new and go down.
     *
     * we need connect this top event to the processed portion of the mesh
     *                           
     *            (top)           
     * -------- . event ------------- sweep line
     *        . . .              
     *      .   .   .      
     *    .     .          
     *  .       .                  
     *          .           
     *                              
     *       new edges
     */
    else gb_tessellator_connect_top_event(impl, event);
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

    // make active regions
    if (!gb_tessellator_active_regions_make(impl, bounds)) return ;

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

    // remove degenerate faces
    gb_tessellator_remove_degenerate_faces(impl);

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(mesh);
#endif
}


