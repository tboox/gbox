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
static tb_void_t gb_tessellator_make_convex_face(gb_tessellator_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && face);

    // the face edge
    gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);
    tb_assert_abort(edge);

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


