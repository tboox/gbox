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
 * macros
 */

#if 1
/* walk edges in counter-clockwise order from bottom to top
 *
 * we attach the bottommost right edge to the face when we made monotone polygon.
 * so we will find the bottommost left and right edge more quickly for triangulation.
 *
 *     .      .
 *    .       .
 *      .   . right => face.edge
 * left   .
 *
 *  |            |
 *   ----------->
 *       ccw
 */
#   define gb_tessellator_edge_go_up_(a)                        gb_tessellator_edge_go_down(a)
#   define gb_tessellator_edge_go_down_(a)                      gb_tessellator_edge_go_up(a)
#   define gb_tessellator_vertex_in_top_or_horizontal_(a, b)    (!gb_tessellator_vertex_in_top_or_horizontal(a, b))
#   define gb_tessellator_vertex_on_edge_or_left_(a, b, c)      gb_tessellator_vertex_on_edge_or_right(a, c, b)
#   define gb_tessellator_vertex_on_edge_or_right_(a, b, c)     gb_tessellator_vertex_on_edge_or_left(a, c, b)
#else
/* walk edges in counter-clockwise order from top to bottom
 *
 *          ccw
 *    <------------
 *   |             |
 *
 *          . 
 * left   .   .  right
 *      .       .
 *       .        .
 *        .       .
 */
#   define gb_tessellator_edge_go_up_(a)                        gb_tessellator_edge_go_up(a)
#   define gb_tessellator_edge_go_down_(a)                      gb_tessellator_edge_go_down(a)
#   define gb_tessellator_vertex_in_top_or_horizontal_(a, b)    gb_tessellator_vertex_in_top_or_horizontal(a, b)
#   define gb_tessellator_vertex_on_edge_or_left_(a, b, c)      gb_tessellator_vertex_on_edge_or_left(a, b, c)
#   define gb_tessellator_vertex_on_edge_or_right_(a, b, c)     gb_tessellator_vertex_on_edge_or_right(a, b, c)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

/* make triangulation region 
 *
 * TODO need optimization, maybe generate some degenerated triangles
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
static tb_void_t gb_tessellator_triangulation_make_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    tb_assert(impl && face);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert(mesh);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);

    // must be triangle region at least
    tb_assert(edge && gb_mesh_edge_lnext(edge) != edge && gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge)) != edge);

    /* get the uppermost left edge
     *
     * @note the face edge has been optimizated when we made monotone polygon.
     *
     *          . 
     * left   .   .  right
     *      .       .
     *       .        .
     *        .       .
     *         .      .
     */
    gb_mesh_edge_ref_t left = edge;
    while (gb_tessellator_edge_go_down_(left)) left = gb_mesh_edge_lprev(left);
    while (gb_tessellator_edge_go_up_(left)) left = gb_mesh_edge_lnext(left);

    // get the uppermost right edge
    gb_mesh_edge_ref_t right = gb_mesh_edge_lprev(left);

    // done
    while (gb_mesh_edge_lnext(left) != right) 
    {
        /* the right edge is too lower? done some left edges
         *
         *          .
         * left   .   . right 
         *  dst .       .
         *       .        . org
         *        .       . 
         *         .      .
         */
        if (gb_tessellator_vertex_in_top_or_horizontal_(gb_mesh_edge_dst(left), gb_mesh_edge_org(right))) 
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
             *
             *
             * on left? connect it
             *
             * TODO: will generate some degenerated triangles
             *
             *             .
             *           . .
             * on left?. . .
             *       .     .
             *     .    .  .
             *     .       .
             *   L .  .    . R
             *     ..      .
             *     .       .
             *    .        .
             *   .         .
             *             .
             *             .
             */
            while ( gb_mesh_edge_lnext(right) != left
                &&  (   gb_tessellator_edge_go_up_(gb_mesh_edge_lprev(left))
                    ||  gb_tessellator_vertex_on_edge_or_left_( gb_mesh_edge_org(left)
                                                            ,   gb_mesh_edge_org(gb_mesh_edge_lprev(left))
                                                            ,   gb_mesh_edge_dst(left))))
            {
                // connect it
                edge = gb_mesh_edge_connect(mesh, left, gb_mesh_edge_lprev(left));
                tb_assert_and_check_return(edge);

                // update the left edge
                left = gb_mesh_edge_sym(edge);
            }

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
             *
             * on right? connect it
             *
             * TODO: will generate some degenerated triangles
             *
             *     . 
             *     .  .
             *     .  .  . 
             *     .   .   .
             *     .    .    . on right?
             *     .     .   .
             *   L .      .  . R
             *     .       . .
             *     .         .
             *     .           . 
             *     .             .
             *     .            
             *     .
             */
            while ( gb_mesh_edge_lnext(right) != left
                &&  (   gb_tessellator_edge_go_down_(gb_mesh_edge_lnext(right))
                    ||  gb_tessellator_vertex_on_edge_or_right_(gb_mesh_edge_dst(right)
                                                            , gb_mesh_edge_dst(gb_mesh_edge_lnext(right))
                                                            , gb_mesh_edge_org(right))))
            {
                // connect it
                edge = gb_mesh_edge_connect(mesh, gb_mesh_edge_lnext(right), right);
                tb_assert_and_check_return(edge);

                // update the right edge
                right = gb_mesh_edge_sym(edge);
            }

            // the next right edge
            right = gb_mesh_edge_lprev(right);
        }
    }
    
    // the last region must be triangle at least
    tb_assert(gb_mesh_edge_lnext(right) != left);

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
        tb_assert_and_check_return(edge);

        // the next edge
        right = gb_mesh_edge_sym(edge);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_tessellator_triangulation_make(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert(impl && impl->mesh);

    // the new face must be inserted to the head of faces
    tb_assert(gb_mesh_face_order(impl->mesh) == GB_MESH_ORDER_INSERT_HEAD);

    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_face_itor(impl->mesh);
    tb_assert(iterator);

    // done
    tb_size_t           itor = tb_iterator_head(iterator);
    tb_size_t           tail = tb_iterator_tail(iterator);
    gb_mesh_face_ref_t  face = tb_null;
    while (itor != tail)
    {
        // the face
        face = (gb_mesh_face_ref_t)tb_iterator_item(iterator, itor);
        tb_assert(face);

        /* the next face
         *
         * @note we don't process the new faces at the head 
         */
        itor = tb_iterator_next(iterator, itor);

        // the face is inside?
        if (gb_tessellator_face_inside(face)) 
        {
            // make triangulation for the face region
            gb_tessellator_triangulation_make_face(impl, face);
        }
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif
}


