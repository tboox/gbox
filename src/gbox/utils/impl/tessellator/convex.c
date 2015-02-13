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
#if 0
static tb_size_t gb_tessellator_vertex_count_for_face(gb_mesh_face_ref_t face)
{
    // check
    tb_assert_abort(face);

    // the face edge
    gb_mesh_edge_ref_t  edge = gb_mesh_face_edge(face);
    tb_assert_abort(edge);

    // done
    tb_size_t           count = 0;
    gb_mesh_edge_ref_t  scan = edge;
    do
    {
        // update count
        count++;

        // the next edge
        scan = gb_mesh_edge_lnext(scan);
    }
    while (scan != edge);

    // ok?
    return count;
}
#endif
static tb_void_t gb_tessellator_make_convex_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && face);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);
    tb_assert_abort(edge);

#if 0
    // done
    gb_mesh_edge_ref_t edge_sym = tb_null;
    gb_mesh_edge_ref_t edge_next = tb_null;
    gb_mesh_vertex_ref_t vertex_start = gb_mesh_edge_org(edge);
    while (1)
    {
        edge_next = gb_mesh_edge_lnext(edge);
        edge_sym = gb_mesh_edge_sym(edge);

        if (edge_sym && gb_mesh_edge_lface(edge_sym) && gb_tessellator_face_inside(gb_mesh_edge_lface(edge_sym)))
        {
            tb_long_t vertex_count = gb_tessellator_vertex_count_for_face(face);
            tb_long_t vertex_count_sym = gb_tessellator_vertex_count_for_face(gb_mesh_edge_lface(edge_sym));

            if ((vertex_count + vertex_count_sym - 2) <= 1024)
            {
                if (    gb_tessellator_vertex_is_ccw( gb_mesh_edge_org(gb_mesh_edge_lprev(edge))
                                                    , gb_mesh_edge_org(edge)
                                                    , gb_mesh_edge_org(gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge))))
                    &&  gb_tessellator_vertex_is_ccw( gb_mesh_edge_org(gb_mesh_edge_lprev(edge_sym))
                                                    , gb_mesh_edge_org(edge_sym)
                                                    , gb_mesh_edge_org(gb_mesh_edge_lnext(gb_mesh_edge_lnext(edge)))))
                {
                    edge_next = gb_mesh_edge_lnext(edge_sym);
                    gb_mesh_edge_delete( mesh, edge_sym );
                    edge = tb_null;
                }
            }
        }
        
        if (edge && gb_mesh_edge_org(gb_mesh_edge_lnext(edge)) == vertex_start)
            break;
            
        edge = edge_next;
    }
#endif
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_tessellator_make_convex(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->mesh);

    // done
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(impl->mesh), face)
    {
        // the face must be inside
        tb_check_continue(gb_tessellator_face_inside(face));

        // make convex region for face
        gb_tessellator_make_convex_face(impl, face);
    }

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif
}


