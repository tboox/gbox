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
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        mesh.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh.h"
#include "impl/mesh_edge.h"
#include "impl/mesh_face.h"
#include "impl/mesh_vertex.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh impl type 
typedef struct __gb_mesh_impl_t
{
    // the edges
    gb_mesh_edge_list_ref_t         edges;

    // the faces
    gb_mesh_face_list_ref_t         faces;

    // the vertices
    gb_mesh_vertex_list_ref_t       vertices;

}gb_mesh_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
#if 0
static tb_void_t gb_mesh_orbit_set_org(gb_mesh_edge_ref_t edge, gb_mesh_vertex_ref_t org)
{
    // check
    tb_assert_abort(edge && org);

    // done
    gb_mesh_edge_ref_t scan = edge;
    do
    {
        // set org
        gb_mesh_edge_org_set(scan, org);

        // the next edge
        scan = gb_mesh_edge_onext(scan);
    }
    while (scan != edge);
}
static tb_void_t gb_mesh_orbit_set_lface(gb_mesh_edge_ref_t edge, gb_mesh_face_ref_t lface)
{
    // check
    tb_assert_abort(edge && lface);

    // done
    gb_mesh_edge_ref_t scan = edge;
    do
    {
        // set lface
        gb_mesh_edge_lface_set(scan, lface);

        // the next edge
        scan = gb_mesh_edge_onext(scan);
    }
    while (scan != edge);
}
#endif
static gb_mesh_edge_ref_t gb_mesh_orbit_get_edge(gb_mesh_edge_ref_t edge, gb_mesh_face_ref_t lface)
{
    // check
    tb_assert_abort(edge && lface);

    // done
    gb_mesh_edge_ref_t scan = edge;
    do
    {
        // found?
        if (gb_mesh_edge_lface(scan) == lface) return scan;

        // the next edge
        scan = gb_mesh_edge_onext(scan);
    }
    while (scan != edge);

    // failed
    return tb_null;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_ref_t gb_mesh_init(tb_item_func_t edge_func, tb_item_func_t face_func, tb_item_func_t vertex_func)
{
    // done
    tb_bool_t           ok = tb_false;
    gb_mesh_impl_t*     impl = tb_null;
    do
    {
        // make mesh
        impl = tb_malloc0_type(gb_mesh_impl_t);
        tb_assert_and_check_break(impl);
    
        // init edges
        impl->edges = gb_mesh_edge_list_init(edge_func);
        tb_assert_and_check_break(impl->edges);
  
        // init faces
        impl->faces = gb_mesh_face_list_init(face_func);
        tb_assert_and_check_break(impl->faces);

        // init vertices
        impl->vertices = gb_mesh_vertex_list_init(vertex_func);
        tb_assert_and_check_break(impl->vertices);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_mesh_exit((gb_mesh_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_mesh_ref_t)impl;
}
tb_void_t gb_mesh_exit(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl);
   
    // exit edges
    if (impl->edges) gb_mesh_edge_list_exit(impl->edges);
    impl->edges = tb_null;

    // exit faces
    if (impl->faces) gb_mesh_face_list_exit(impl->faces);
    impl->faces = tb_null;

    // exit vertices
    if (impl->vertices) gb_mesh_vertex_list_exit(impl->vertices);
    impl->vertices = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_mesh_clear(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl);

    // clear edges
    if (impl->edges) gb_mesh_edge_list_clear(impl->edges);

    // clear faces
    if (impl->faces) gb_mesh_face_list_clear(impl->faces);

    // clear vertices
    if (impl->vertices) gb_mesh_vertex_list_clear(impl->vertices);
}
tb_iterator_ref_t gb_mesh_face_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl, tb_null);

    // the face iterator
    return gb_mesh_face_list_itor(impl->faces);
}
tb_iterator_ref_t gb_mesh_vertex_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl, tb_null);

    // the vertex iterator
    return gb_mesh_vertex_list_itor(impl->vertices);
}
gb_mesh_edge_ref_t gb_mesh_make_edge(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_mesh_edge_ref_t      edge = tb_null;
    gb_mesh_face_ref_t      face = tb_null;
    gb_mesh_vertex_ref_t    org = tb_null;
    gb_mesh_vertex_ref_t    dst = tb_null;
    do
    {
        // make the org
        org = gb_mesh_vertex_list_make(impl->vertices);
        tb_assert_and_check_break(org);

        // make the dst
        dst = gb_mesh_vertex_list_make(impl->vertices);
        tb_assert_and_check_break(dst);

        // make the face
        face = gb_mesh_face_list_make(impl->faces);
        tb_assert_and_check_break(face);

        // make the edge
        edge = gb_mesh_edge_list_make(impl->edges);
        tb_assert_and_check_break(edge);

        // the sym edge
        gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge);
        tb_assert_and_check_break(edge_sym);

        // init the edge
        gb_mesh_edge_org_set  (edge, org);
        gb_mesh_edge_lface_set(edge, face);
        gb_mesh_edge_onext_set(edge, edge);
        gb_mesh_edge_lnext_set(edge, edge_sym);

        // init the sym edge
        gb_mesh_edge_org_set  (edge_sym, dst);
        gb_mesh_edge_lface_set(edge_sym, face);
        gb_mesh_edge_onext_set(edge_sym, edge_sym);
        gb_mesh_edge_lnext_set(edge_sym, edge);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // kill the org
        if (org) gb_mesh_vertex_list_kill(impl->vertices, org);
        org = tb_null;

        // kill the dst
        if (dst) gb_mesh_vertex_list_kill(impl->vertices, dst);
        dst = tb_null;

        // kill the face
        if (face) gb_mesh_face_list_kill(impl->faces, face);
        face = tb_null;

        // kill the edge
        if (edge) gb_mesh_edge_list_kill(impl->edges, edge);
        edge = tb_null;
    }

    // ok?
    return edge;
}
gb_mesh_edge_ref_t gb_mesh_make_loop_edge(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_mesh_edge_ref_t      edge = tb_null;
    gb_mesh_face_ref_t      lface = tb_null;
    gb_mesh_face_ref_t      rface = tb_null;
    gb_mesh_vertex_ref_t    vertex = tb_null;
    do
    {
        // make the vertex
        vertex = gb_mesh_vertex_list_make(impl->vertices);
        tb_assert_and_check_break(vertex);

        // make the left face
        lface = gb_mesh_face_list_make(impl->faces);
        tb_assert_and_check_break(lface);

        // make the right face
        rface = gb_mesh_face_list_make(impl->faces);
        tb_assert_and_check_break(rface);

        // make the edge
        edge = gb_mesh_edge_list_make(impl->edges);
        tb_assert_and_check_break(edge);

        // the sym edge
        gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge);
        tb_assert_and_check_break(edge_sym);

        // init the edge
        gb_mesh_edge_org_set  (edge, vertex);
        gb_mesh_edge_lface_set(edge, lface);
        gb_mesh_edge_onext_set(edge, edge_sym);
        gb_mesh_edge_lnext_set(edge, edge);

        // init the sym edge
        gb_mesh_edge_org_set  (edge_sym, vertex);
        gb_mesh_edge_lface_set(edge_sym, rface);
        gb_mesh_edge_onext_set(edge_sym, edge);
        gb_mesh_edge_lnext_set(edge_sym, edge_sym);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // kill the vertex
        if (vertex) gb_mesh_vertex_list_kill(impl->vertices, vertex);
        vertex = tb_null;

        // kill the left face
        if (lface) gb_mesh_face_list_kill(impl->faces, lface);
        lface = tb_null;

        // kill the right face
        if (rface) gb_mesh_face_list_kill(impl->faces, rface);
        rface = tb_null;

        // kill the edge
        if (edge) gb_mesh_edge_list_kill(impl->edges, edge);
        edge = tb_null;
    }

    // ok?
    return edge;
}
gb_mesh_face_ref_t gb_mesh_make_face_edge(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, gb_mesh_vertex_ref_t org, gb_mesh_vertex_ref_t dst, gb_mesh_edge_ref_t* pedge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl, tb_null);

    return tb_null;
}
tb_void_t gb_mesh_kill_face_edge(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge)
{
}
gb_mesh_vertex_ref_t gb_mesh_make_vertex_edge(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, gb_mesh_face_ref_t lface, gb_mesh_face_ref_t rface, gb_mesh_edge_ref_t* pedge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && vertex && lface && rface, tb_null);

    // get the vertex edge
    gb_mesh_edge_ref_t edge = gb_mesh_vertex_edge(vertex);
    tb_assert_abort_and_check_return_val(edge, tb_null);

    // get the edge with lface
    gb_mesh_edge_ref_t edge_lf = gb_mesh_orbit_get_edge(edge, lface);
    tb_assert_abort_and_check_return_val(edge_lf, tb_null);

    // get the edge with rface
    gb_mesh_edge_ref_t edge_rf = gb_mesh_orbit_get_edge(edge, rface);
    tb_assert_abort_and_check_return_val(edge_rf, tb_null);

    /* 
     * 
     *                                   .   .   .
     *     .    .    .                     . . .
     *       .  .  .                     vertex_new(dst)
     *         . .                           .
     * lface  vertex  rface    =>  lface     .        rface
     *         . .                           .
     *       .     .                        /.\ edge_new
     *                                       . 
     *                                       .
     *                                    vertex(org)
     *                                      . .
     *                                    .     . 
     * 
     */

    return tb_null;
}
tb_void_t gb_mesh_kill_vertex_edge(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge)
{
}
