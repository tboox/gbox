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

/* make triangulation region 
 *
 * the region of this face must be horizontal monotone and counter-clockwise loop
 *                      
 * before:
 *
 *                  ccw
 *             <------------
 *            |             |
 *         
 *                   1                
 *                 .   .    right
 *      left     .       2
 *             .       .
 *           .       .  
 *         3        4
 *          .        .
 *           .        .
 *            5        .
 *           .          .  
 *          .            .
 *         .              .
 *        6                .    
 *         .                .
 *          .               7
 *           .             .
 *            8          . 
 *            .        9
 *            .        .
 *           10        11
 *             .      .
 *                .  .  
 *                 12
 *                      
 *     |                        |
 *     |                        |
 *    \ /                      \ /
 *
 *
 * after:
 *                   1                
 *                 .   .    R2
 *      L1       .     . 2
 *             .   .L4 .
 *           . .     .  R3
 *         3 . . R6 4
 *          .      . .
 *        L5 .  .L8   .
 *            5        .
 *           .   .      . R7 
 *       L9 .     R11    .
 *         .         .    .
 *        6  .  R12        .    
 *         .        .     . .
 *      L10 .             . 7
 *           .   L14 .     .
 *            8 .        . R13
 *            .  R16 . 9
 *        L15 .        .
 *           10  L18 . 11
 *              .     .
 *           L19  .  .  R17
 *                 12
 *
 */
static tb_bool_t gb_tessellator_make_triangulation_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && face);

    return tb_true;
    gb_mesh_dump(impl->mesh);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);

    // must be triangle region at least
    tb_assert_abort(edge && gb_mesh_edge_lnext(edge) != edge && gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge)) != edge);

    /* get the uppermost left edge
     *
     *          . 
     * left   .   .  right
     *      .       .
     *       .        .
     *        .       .
     *         .      .
     */
    gb_mesh_edge_ref_t left = edge;
    while (gb_tessellator_edge_go_down(left)) left = gb_mesh_edge_lprev(left);
    while (gb_tessellator_edge_go_up(left)) left = gb_mesh_edge_lnext(left);

    // get the uppermost right edge
    gb_mesh_edge_ref_t right = gb_mesh_edge_lprev(left);

    tb_trace_i("11111");
        
    tb_trace_i("left %{point} %{point}", gb_tessellator_vertex_point(gb_mesh_edge_org(left)), gb_tessellator_vertex_point(gb_mesh_edge_dst(left)));
    tb_trace_i("right %{point} %{point}", gb_tessellator_vertex_point(gb_mesh_edge_org(right)), gb_tessellator_vertex_point(gb_mesh_edge_dst(right)));

    // done
    while (gb_mesh_edge_lnext(left) != right) 
    {
        tb_trace_i("%{point} %{point}", gb_tessellator_vertex_point(gb_mesh_edge_dst(left)), gb_tessellator_vertex_point(gb_mesh_edge_org(right)));
        /* the right edge is too lower? done some left edges
         *
         *          .
         * left   .   . right 
         *  dst .       .
         *       .        . org
         *        .       . 
         *         .      .
         */
        if (gb_tessellator_vertex_in_top(gb_mesh_edge_dst(left), gb_mesh_edge_org(right))) 
        {
            /* done some left edges
             *
             * go up? connect it
             *
             *     .
             *     .    .
             *     .        .
             *     .            .
             *  L  .   L1   .    .
             *     .     .        .
             *     .  .            .
             *     .  .  .  .  . .  .
             *             L2     .
             *                  . R
             *                .
             */
            while ( gb_mesh_edge_lnext(right) != left
                &&  (   gb_tessellator_edge_go_up(gb_mesh_edge_lprev(left))
                    ||  gb_tessellator_vertex_in_edge_or_right( gb_mesh_edge_org(left)
                                                            ,   gb_mesh_edge_org(left)
                                                            ,   gb_mesh_edge_dst(gb_mesh_edge_lprev(left))))) //< FIXME
            {
                tb_trace_i("left");
                // connect it
                edge = gb_mesh_edge_connect(mesh, left, gb_mesh_edge_lprev(left));
                tb_assert_abort_and_check_return_val(edge, tb_false);

                // update the left edge
                left = gb_mesh_edge_sym(edge);
            }

                tb_trace_i("left next");
            // the next left edge
            left = gb_mesh_edge_lnext(left);
        } 
        /* the left edge is too lower? done some right edges
         *
         *           .
         *   left  .   . right  
         *       .       . org
         * dst .        .
         *     .       .  
         *     .      .
         */
        else
        {
            /* done some right edges
             * 
             * go down? connect it
             *
             *                      .
             *                 .    .
             *             .        .
             *         .            . R
             *        .   .   R1    .
             *       .       .      .
             *      .  R2       .   .
             *     . . . . . . . .  .
             *       . 
             *      L  .
             *           .
             */
            while ( gb_mesh_edge_lnext(right) != left
                &&  (   gb_tessellator_edge_go_down(gb_mesh_edge_lnext(right))
                    ||  gb_tessellator_vertex_in_edge_or_left(gb_mesh_edge_dst(right)
                                                            , gb_mesh_edge_dst(right)
                                                            , gb_mesh_edge_org(gb_mesh_edge_lnext(right))))) //< FIXME
            {
                tb_trace_i("right");
                // connect it
                edge = gb_mesh_edge_connect(mesh, gb_mesh_edge_lnext(right), right);
                tb_assert_abort_and_check_return_val(edge, tb_false);

                // update the right edge
                right = gb_mesh_edge_sym(edge);
            }

                tb_trace_i("right next");
            // the next right edge
            right = gb_mesh_edge_lprev(right);
        }
    }
    
    // the last region must be triangle at least
    tb_assert_abort(gb_mesh_edge_lnext(right) != left);

    /* tessellate the remaining region
     *
     *     . . . . 
     *   .        .
     * .           .
     *    .         .
     * left  .    . right
     *          .
     *
     */
    while (gb_mesh_edge_lnext(gb_mesh_edge_lnext(right)) != left) 
    {
        // connect it
        edge = gb_mesh_edge_connect(mesh, gb_mesh_edge_lnext(right), right);
        tb_assert_abort_and_check_return_val(edge, tb_false);

        // the next lower edge
        right = gb_mesh_edge_lprev(right);
    }

    // ok
    return tb_true;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_make_triangulation(gb_tessellator_impl_t* impl)
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
            // make triangulation for the face region
            if (!gb_tessellator_make_triangulation_face(impl, face)) return tb_false;
        }
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif

    // ok
    return tb_true;
}


