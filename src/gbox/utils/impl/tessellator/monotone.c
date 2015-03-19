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
/* find the left region of the leftmost edge with the same origin(event)
 *
 *  . edge_left                                                    . edge_right
 *  .                                                              .
 *  .                                                              .
 *  .          (leftmost)              .                           .
 *  .          edge_first             .                            .
 *  .                . region_first  .             . edge_last     .
 *  .                  .            .  region  .                   .
 *  . region_left        .         .       .                       . region_right
 *  .                      .      .    .  region_last              .     
 * /.\                       .   . .                              /.\
 *  .  ------------------------ . event -------------------------- . ------- sweep line   
 *  .                         .   . (org)                          .
 *  .                       .       .                              .
 *  .                     .                                        .
 *  .                                                              .
 */
static gb_tessellator_active_region_ref_t gb_tessellator_find_left_region(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && region && region->edge);

    // get the origin of the region
    gb_mesh_vertex_ref_t org = gb_mesh_edge_org(region->edge);
    tb_assert_abort(org);

    // find the left region of the leftmost edge with the same origin
    do 
    {
        // get the left region
        region = gb_tessellator_active_regions_left(impl, region);
        tb_assert_abort(region && region->edge);
    
    } while (gb_mesh_edge_org(region->edge) == org);

    // ok?
    return region;
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
/* insert the down-going edges from the given range: [head, tail) and new regions
 * and update winding numbers and mesh connectivity appropriately.
 *
 * the origin vertices of all down-going edges must be same.
 *
 * the left-top edge must be setted if the event vertex has any up-going edges already processed.
 *
 *        .                                                    .
 *     .                                                         .
 *  . edge_left                                                    . edge_right
 *  .                                                              .
 *  .                                                              .  
 *  .                                                              .
 *  .                                                              .
 *  .     (prev)  .                  .                             .
 *  . edge_left_top . (finished)  .                                .
 *  .  (maybe null)   .        .                                   .
 *  .                   .(top)                                     .
 *  .            -------- . event -------------------------------- . ------------------- sweep line      
 *  .                   . . .                                      .  
 * /.\                .   .   .                                   /.\
 *  . region_left   .    \./    . region_new2                      .
 *  .   (prev)    .       .       .                                .  region_right    
 *  .           .         .                                        .
 *  .         .           . region_new1                            .
 *  .       . region_new3 .                                        .
 *  .                     .                                        .           
 *  .                 edge_head                                    . 
 *  .                 edge_tail                                    . 
 *  .                                                              .
 *  .                                                              .
 *  .                 new edges                                    .
 *  .                                                              .
 *
 */
static tb_void_t gb_tessellator_insert_down_going_edges(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_left, gb_tessellator_active_region_ref_t region_right, gb_mesh_edge_ref_t edge_head, gb_mesh_edge_ref_t edge_tail, gb_mesh_edge_ref_t edge_left_top)
{
    // check
    tb_assert_abort(impl && impl->mesh && edge_head && edge_tail);

    // insert the down-going edges from the given range: [head, tail) and new regions
    gb_mesh_edge_ref_t edge = edge_head;
    do 
    {
        // the edge must be down-going
        tb_assert_abort(gb_tessellator_edge_go_down(edge));

        // trace
        tb_trace_d("insert down-going edge: %{mesh_edge}", edge);

        // insert a new region with the new edge at the left of the right region
        gb_tessellator_insert_region_at_left(impl, region_right, gb_mesh_edge_sym(edge));

        // the next inserted edge
        edge = gb_mesh_edge_onext(edge);

    } while (edge != edge_tail);

    // find it if the left-top edge is null
    if (!edge_left_top) 
    {
        // get the leftmost new region
        gb_tessellator_active_region_ref_t region_new_leftmost = gb_tessellator_active_regions_right(impl, region_left);
        tb_assert_abort(region_new_leftmost);

        // get the left-top edge
        edge_left_top = gb_mesh_edge_lnext(region_new_leftmost->edge);
    }

    // done
    gb_tessellator_active_region_ref_t  region_new  = tb_null;
    gb_tessellator_active_region_ref_t  region_prev = region_left;
    gb_mesh_edge_ref_t                  edge_new    = tb_null;
    gb_mesh_edge_ref_t                  edge_prev   = edge_left_top;
    while (1)
    {
        // get the next new region
        region_new = gb_tessellator_active_regions_right(impl, region_prev);
        tb_assert_abort(region_new && region_new->edge);

        // get the left down-going edge of the new region
        edge_new = gb_mesh_edge_sym(region_new->edge);

        // end? the origin vertices of all down-going edges must be same
        tb_check_break(gb_mesh_edge_org(edge_new) == gb_mesh_edge_org(edge_prev));

        /* joins the two edges if edge_prev and edge_new are disjoining but the original vertices are same
         *
         * before(edge_prev.org == edge_new.org):
         *
         *     . . . . org . . . . . .
         *          .    
         *       .      org . . . .
         *    .           .  .
         * .               .    .
         * edge_prev        .
         *                   .
         *                  edge_new
         *
         *
         * after:
         * 
         *     . . . . org . . . . . .
         *          .   .
         *       .       .     . . . .
         *    .           .      .
         * .               .        .
         * edge_prev        .
         *                   .
         *                  edge_new
         */
        if (gb_mesh_edge_onext(edge_prev) != edge_new)
        {
            // trace
            tb_trace_d("joins down-going edges with the same origin");

            /* 
             *     . . . . org . . . . . .
             *          .            face 
             *       .      org     . . . .
             *    .   face    .       .
             * .               . face   .
             * edge_prev        .
             *                   .
             *                  edge_new
             */
            gb_mesh_edge_splice(impl->mesh, edge_new, gb_mesh_edge_oprev(edge_new));

            /* 
             *     . . . . org . . . . . .
             *          .   .
             *       .       .     . . . .
             *    .           .      .
             * .               .       .
             * edge_prev        .
             *                   .
             *                  edge_new
             */
            gb_mesh_edge_splice(impl->mesh, edge_new, edge_prev);
        }

        // check
        tb_assert_abort(gb_mesh_edge_onext(edge_prev) == edge_new);

        /* compute the winding of the new region
         *
         * region_new.winding = region_prev.winding + region_new.edge.winding
         *                                       => - region_new.edge.sym.winding
         *                                       => - edge_new.winding
         */
        region_new->winding = region_prev->winding - gb_tessellator_edge_winding(edge_new);

        // mark it if the new region is inside
        region_new->inside = gb_tessellator_winding_is_inside(impl, region_new->winding);

        // update the previous edge and region 
        edge_prev   = edge_new;
        region_prev = region_new;
    }

    // check winding
    tb_assert_abort(region_new->winding == region_prev->winding - gb_tessellator_edge_winding(edge_new));
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
 *     if the contained(left) region is inside 
 *
 *   - only add all edges to the mesh if the the contained(left) region is outside 
 *
 * - the degenerate case:
 *
 *   - we merge it into that edge chain if the event vertex close enough to the left edge or right edge
 */
static tb_void_t gb_tessellator_connect_top_event(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->active_regions && event);

    // trace
    tb_trace_d("connect top event: %{point}", gb_tessellator_vertex_point(event));

    // the edge of the event vertex
    gb_mesh_edge_ref_t edge_event = gb_mesh_vertex_edge(event);
    tb_assert_abort(edge_event);

    // the up-going edge of the event vertex
    gb_mesh_edge_ref_t edge_event_up = gb_mesh_edge_sym(edge_event);
    tb_assert_abort(edge_event_up);

    // get the left region containing this event from the up-going edge
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

    /* we merge it into that edge chain if the event vertex close enough to the left edge 
     *
     *        .                                                    .
     *     .                                                         .
     *  . edge_left                                                    . edge_right
     *  .                                                              .
     *  .                     region_left                              .
     *  .                      (inside)                                .
     *  .                                                              . 
     *  . (top)                                                        .
     *  . event ------------------------------------------------------ . ------------------ sweep line      
     *  .  .   .                                                       .  
     * /.\   .     .                                                  /.\
     *  .      .       .                                               .
     *  .        .         .                                           .  region_right    
     *  .                      .                                       .           
     *  .          new_edges                                           . 
     *  .                                                              .
     *  .                                                              .
     */
    if (gb_tessellator_vertex_on_edge(event, gb_mesh_edge_dst(edge_left), gb_mesh_edge_org(edge_left)))
    {
        // TODO
        tb_assert_abort(0);
    }

    // get the region which edge.dst is lower and we need connect it
    gb_tessellator_active_region_ref_t region_lower = gb_tessellator_vertex_in_top(gb_mesh_edge_dst(edge_left), gb_mesh_edge_dst(edge_right))? region_right : region_left;

    // we need split it if the contained(left) region is inside
    if (region_left->inside)
    {
        // trace
        tb_trace_d("split the region: %{tessellator_region}", region_left);

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
         *  .   (inside)       .(top)                                      .
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
            edge_new = gb_mesh_edge_connect(impl->mesh, edge_event_up, gb_mesh_edge_rprev(edge_left));
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
         *  .                    (top)                                     .
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

        // mark it if the new region is inside
        region_new->inside = gb_tessellator_winding_is_inside(impl, region_new->winding);

        // continue to sweep this event for the new region
        gb_tessellator_sweep_event(impl, event);
    }
    // only add all edges to the mesh if the the contained(left) region is outside 
    else
    {
        /* insert all down-going edges and new regions
         * and update winding numbers and mesh connectivity appropriately
         *
         *        .                                                    .
         *     .                                                         .
         *  . edge_left                                                    . edge_right
         *  .                                                              .
         *  .                                                              .  
         *  .           region_left (outside)                              . 
         *  .                                                              .
         *  .                                                              .
         *  .            x (no edge_left_top)                              .
         *  .              x                                               .
         *  .                x                                             .
         *  .                   (top)                                      .
         *  .            -------- . event -------------------------------- . ------------------- sweep line      
         *  .                   . . .                                      .  
         * /.\                .   .   .                                   /.\
         *  .               .    \./                                       .
         *  .             .       .                                        .  region_right    
         *  .                     .  edge_event                            .           
         *  .                                                              . 
         *  .                  new edges                                   .
         *  .                                                              .
         *
         */
        gb_tessellator_insert_down_going_edges(impl, region_left, region_right, edge_event, edge_event, tb_null);
    }
}
/* finish one top region of the current event
 *
 * we mark the it's face "inside" if this region is inside
 * and remove this region.
 *
 *       face        .
 * .               .
 *   .  region   .
 *     .       .
 *  edge .   .
 *         .
 */
static tb_void_t gb_tessellator_finish_top_region(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && region);

    // trace
    tb_trace_d("finish region: %{tessellator_region}", region);

    // get the edge of this region
    gb_mesh_edge_ref_t edge = region->edge;
    tb_assert_abort(edge);

    // get the face of this region
    gb_mesh_face_ref_t face = gb_mesh_edge_rface(edge);
    tb_assert_abort(face);

    // mark the face "inside" if the region is inside
    gb_tessellator_face_inside_set(face, region->inside);

    /* TODO optimization for triangulation
     */
    // face->edge = edge;

    // remove this region
    gb_tessellator_active_regions_remove(impl, region);
}
/* finish all top regions of the current event
 *
 * we need close off them first if the left and right edges of these regions terminate 
 * at this event. 
 *
 * then we mark these faces "inside" or "outside" the polygon according 
 * to their winding number and remove these regions.
 *
 *  . edge_left                                                    . edge_right
 *  .                                                              .
 *  .            (prev)                                            .
 *  .          (leftmost)              .                           .
 *  .          edge_first             .                            .
 *  .                . region_first  .             . edge_last     .
 *  .                  .            .  region  .                   .
 *  . region_left        .         .       .                       . region_right
 *  .                      .      .    .  region_last              .     
 * /.\                       .   . .                              /.\
 *  .  ------------------------ . event -------------------------- . ------- sweep line   
 *  .                         .   .                                .
 *  .                       .       .                              .
 *  .                     .                                        .
 *  .                                                              .
 */
static gb_tessellator_active_region_ref_t gb_tessellator_finish_top_regions(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_first, gb_tessellator_active_region_ref_t region_last)
{
    // check
    tb_assert_abort(impl && region_first);

    // we walk as far as possible if region_last is null 
    gb_mesh_edge_ref_t                  edge            = region_first->edge;
    gb_mesh_edge_ref_t                  edge_next       = tb_null;
    gb_tessellator_active_region_ref_t  region          = region_first;
    gb_tessellator_active_region_ref_t  region_next     = tb_null;
    while (region != region_last) 
    {
        // get the next right region
        region_next = gb_tessellator_active_regions_right(impl, region);
        tb_assert_abort(region_next);

        // get the next right edge
        edge_next = region_next->edge;
        tb_assert_abort(edge_next);

        // end?
        if (gb_mesh_edge_org(edge_next) != gb_mesh_edge_org(edge))
        {
            // finish the top region
            gb_tessellator_finish_top_region(impl, region);

            // end
            break;
        }

        /* joins the two edges if edge and edge_next are disjoining but the original vertices are same
         *
         * before(edge.org == edge_next.org):
         *
         *  
         *                 edge_next
         *                   .
         *                  .
         * edge            .
         * .              .     .
         *    .          .    .
         *       .      org . . . .
         *          .
         *     . . . . org . . . . . .
         *
         *
         * after:
         *  
         *                 edge_next
         *                   .
         *                  .
         * edge            .
         * .              .        .
         *    .          .      .
         *       .      .     . . . .
         *          .  .
         *     . . . . org . . . . . .
  
         */
        if (gb_mesh_edge_onext(edge_next) != edge)
        {
            // trace
            tb_trace_d("joins up-going edges with the same origin");

            /* 
             *
             *                 edge_next
             *                   .
             *                  .
             * edge      face  .
             * .              .         .
             *    .          .   face .
             *       .      org     . . . .
             *          .        face
             *     . . . . org . . . . . .
             */
            gb_mesh_edge_splice(impl->mesh, edge_next, gb_mesh_edge_oprev(edge_next));

            /* 
             *                 edge_next
             *                   .
             *                  .
             * edge            .
             * .              .        .
             *    .          .      .
             *       .      .     . . . .
             *          .  .
             *     . . . . org . . . . . .
             */
            gb_mesh_edge_splice(impl->mesh, edge_next, gb_mesh_edge_oprev(edge));
        }

        // check
        tb_assert_abort(gb_mesh_edge_onext(edge_next) == edge);

        // finish the top region
        gb_tessellator_finish_top_region(impl, region);

        // update the edge and region, region_next->edge may be changed
        edge    = region_next->edge;
        region  = region_next;
    }

    // return the last region
    return region;
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
     *
     *  . edge_left                                                    . edge_right
     *  .                                                              .
     *  .                                                              .
     *  .          (leftmost)              .                           .
     *  .          edge_first             .                            .
     *  .                . region_first  .             . edge_last     .
     *  .                  .            .  region  .                   .
     *  . region_left        .         .       .                       . region_right
     *  .                      .      .    .  region_last              .     
     * /.\                       .   . .                              /.\
     *  .  ------------------------ . event -------------------------- . ------- sweep line   
     *  .                         .   .                                .
     *  .                       .       .                              .
     *  .                     .                                        .
     *  .                                                              .
     *
     */
    if (region)
    {
        // find the left region of the leftmost edge with the same origin
        gb_tessellator_active_region_ref_t region_left = gb_tessellator_find_left_region(impl, region);
        tb_assert_abort(region_left);

        // get the first(leftmost) top region of this event
        gb_tessellator_active_region_ref_t region_first = gb_tessellator_active_regions_right(impl, region_left);
        tb_assert_abort(region_first);

        // finish all top regions of this event and return the last region
        gb_tessellator_active_region_ref_t region_last = gb_tessellator_finish_top_regions(impl, region_first, tb_null);
        tb_assert_abort(region_last);

        // get the right region
        gb_tessellator_active_region_ref_t region_right = gb_tessellator_active_regions_right(impl, region_last);
        tb_assert_abort(region_right);

        // get the first(leftmost) top edge of this event
        gb_mesh_edge_ref_t edge_first = region_first->edge;
        tb_assert_abort(edge_first);

        // get the last(rightmost) top edge of this event
        gb_mesh_edge_ref_t edge_last = region_last->edge;
        tb_assert_abort(edge_last);

        /* connect the bottom event if no down-going edges
         *
         *          .             .
         *            .         .
         *              .     .
         * --------------- . ------------------------------------ sweep line
         *               event
         *              (bottom)
         */
        if (gb_mesh_edge_onext(edge_first) == edge_last)
        {
            // TODO
        }
        // insert all down-going edges at this event and create new active regions
        else gb_tessellator_insert_down_going_edges(impl, region_left, region_right, gb_mesh_edge_onext(edge_first), edge_last, edge_first);
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


