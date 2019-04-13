/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
 * @file        convex.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "convex"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "convex.h"
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_tessellator_convex_make_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    tb_assert(impl && face);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert(mesh);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);
    tb_assert(edge);

    // the first vertex
    gb_mesh_vertex_ref_t vertex_first = gb_mesh_edge_org(edge);
    tb_assert(vertex_first);

    // done
    gb_mesh_face_ref_t face_sym = tb_null;
    gb_mesh_edge_ref_t edge_sym = tb_null;
    gb_mesh_edge_ref_t edge_next = tb_null;
    while (1)
    {
        // save the next edge
        edge_next = gb_mesh_edge_lnext(edge);

        // the sym edge
        edge_sym = gb_mesh_edge_sym(edge);

        // the neighbour face
        face_sym = gb_mesh_edge_lface(edge_sym);

        // the neighbour face is inside too? merge it
        if (edge_sym && face_sym && gb_tessellator_face_inside(face_sym))
        {
            /* merge it if the result region is convex
             *       
             *       a
             *       .
             *       . .
             *       .   .
             *       .     .
             *       .       .            
             *       .  face   . 
             *       .           .
             *       .    edge ->  . 
             *     b . . . . . . . . . d
             *         .   edge_sym  .
             *           .           .
             *             . face_sym.
             *               .       .
             *                 .     .
             *                   .   .
             *                     . .
             *                       .
             *                       c
             *
             * is_ccw(a, b, c) and is_ccw(c, d, a)? 
             *
             * some degenerate triangles may be not merged 
             * because the is_ccw is not reliable for almost-degenerate situations
             */
            if (    gb_tessellator_vertex_is_ccw( gb_mesh_edge_org(gb_mesh_edge_lprev(edge))
                                                , gb_mesh_edge_org(edge)
                                                , gb_mesh_edge_dst(gb_mesh_edge_lnext(edge_sym)))
                &&  gb_tessellator_vertex_is_ccw( gb_mesh_edge_org(gb_mesh_edge_lprev(edge_sym))
                                                , gb_mesh_edge_org(edge_sym)
                                                , gb_mesh_edge_dst(gb_mesh_edge_lnext(edge))))
            {
                // save the next edge
                edge_next = gb_mesh_edge_lnext(edge_sym);

                // delete edge(d, b) and merge two faces
                gb_mesh_edge_delete(mesh, edge_sym);
                edge = tb_null;
            }
        }
        
        // end?
        if (edge && gb_mesh_edge_org(gb_mesh_edge_lnext(edge)) == vertex_first)
            break;
            
        // the next edge
        edge = edge_next;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_tessellator_convex_make(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert(impl && impl->mesh);

    // done
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(impl->mesh), face)
    {
        // the face must be inside
        tb_check_continue(gb_tessellator_face_inside(face));

        // make convex region for face
        gb_tessellator_convex_make_face(impl, face);
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif
}


