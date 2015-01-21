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
 * macros
 */

// check
#ifdef __gb_debug__
#   define gb_mesh_check_vertex(vertex)         tb_assertf_abort((vertex) && (vertex)->id && (vertex)->edge, "invalid vertex: %p, id: %lu", vertex, (vertex)? (vertex)->id : 0)
#   define gb_mesh_check_face(face)             tb_assertf_abort((face) && (face)->id && (face)->edge, "invalid face: %p, id: %lu", face, (face)? (face)->id : 0)
#   define gb_mesh_check_edge(edge)             do { tb_assertf_abort((edge) && (edge)->sym && (edge)->id && (edge)->sym->id, "invalid edge: %p => %p, id: %lu => %lu", edge, (edge)? (edge)->sym : tb_null, (edge)? (edge)->id : 0, (edge)->sym? (edge)->sym->id : 0); gb_mesh_check_face((edge)->lface); gb_mesh_check_vertex((edge)->org); } while (0)
#else
#   define gb_mesh_check_vertex(vertex)
#   define gb_mesh_check_face(face)
#   define gb_mesh_check_edge(edge)
#endif

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
static tb_void_t gb_mesh_orbit_edge_set_org(gb_mesh_edge_ref_t edge, gb_mesh_vertex_ref_t org)
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
static tb_void_t gb_mesh_orbit_edge_set_lface(gb_mesh_edge_ref_t edge, gb_mesh_face_ref_t lface)
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
        scan = gb_mesh_edge_lnext(scan);
    }
    while (scan != edge);
}
static gb_mesh_edge_ref_t gb_mesh_orbit_edge_with_lface(gb_mesh_edge_ref_t edge, gb_mesh_face_ref_t lface)
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
static gb_mesh_edge_ref_t gb_mesh_orbit_edge_with_org(gb_mesh_edge_ref_t edge, gb_mesh_vertex_ref_t org)
{
    // check
    tb_assert_abort(edge && org);

    // done
    gb_mesh_edge_ref_t scan = edge;
    do
    {
        // found?
        if (gb_mesh_edge_org(scan) == org) return scan;

        // the next edge
        scan = gb_mesh_edge_lnext(scan);
    }
    while (scan != edge);

    // failed
    return tb_null;
}
/* splice(a, b)
 *
 * refer to the paper of Guibas and Stolfi, simplified since we don't use flips (p98) 
 *     - Primitives for the Manipulation of General Subdivisions and the Computation of Voronoi Diagrams
 *
 * This operation affects the two edge rings a Org and b Org and, independently, 
 * the two edge rings a Left and b Left. In each case,
 *     - if the two rings are distinct, Splice will combine them into one;
 *     - if the two are exactly the same ring, Splice will break it in two separate pieces;
 *     - if the two are the same ring taken with opposite orientations, 
 *       Splice will Flip (and reverse the order) of a segment of that ring.
 *
 * c = a.onext.rot 
 * d = b.onext.rot
 *
 * a.onext' = b.onext
 * b.onext' = a.onext
 *
 * c.onext' = d.onext
 * d.onext' = c.onext
 *
 * =>
 *
 * x = a.onext
 * y = b.onext
 *
 * c = x.rot 
 * d = y.rot
 *
 * a.onext' = b.onext = y
 * b.onext' = a.onext = x
 *
 * x.rot.onext' = d.onext = y.rot.onext
 * y.rot.onext' = c.onext = x.rot.onext
 *
 * =>
 *
 * x = a.onext
 * y = b.onext
 *
 * a.onext' = y
 * b.onext' = x
 *
 * x.rot.onext' = b.onext.rot.onext
 * y.rot.onext' = a.onext.rot.onext
 *
 * TODO
 * why ?=>
 *
 * x = a.onext
 * y = b.onext
 *
 * a.onext' = y
 * b.onext' = x
 *
 * x.rot.onext'.rot = b.onext.rot.onext.rot
 * y.rot.onext'.rot = a.onext.rot.onext.rot
 *
 * => 
 *
 * x = a.onext
 * y = b.onext
 *
 * a.onext' = y
 * b.onext' = x
 *
 * x.oprev' = b
 * y.oprev' = a
 */
static tb_void_t gb_mesh_splice_edge(gb_mesh_edge_ref_t a, gb_mesh_edge_ref_t b)
{
    // check
    tb_assert_abort(a && b);

    /* x = a.onext
     * y = b.onext
     */
	gb_mesh_edge_ref_t x = gb_mesh_edge_onext(a);
	gb_mesh_edge_ref_t y = gb_mesh_edge_onext(b);

    /* a.onext' = y
     * b.onext' = x
     */
    gb_mesh_edge_onext_set(a, y);
	gb_mesh_edge_onext_set(b, x);

    /* x.oprev' = b
     * y.oprev' = a
     */
	gb_mesh_edge_oprev_set(x, b);
	gb_mesh_edge_oprev_set(y, a);
}
static tb_bool_t gb_mesh_kill_isolated_edge(gb_mesh_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return_val(impl && impl->vertices && impl->edges && impl->faces && edge, tb_false);

    // is isolated edge?
    if (gb_mesh_edge_is_isolated(edge))
    {
        // check
        tb_assert_abort(gb_mesh_edge_org(edge) != gb_mesh_edge_dst(edge));
        tb_assert_abort(gb_mesh_edge_lface(edge) == gb_mesh_edge_rface(edge));

        // kill the origin and destination vertices
        gb_mesh_vertex_list_kill(impl->vertices, gb_mesh_edge_org(edge));
        gb_mesh_vertex_list_kill(impl->vertices, gb_mesh_edge_dst(edge));

        // kill the face
        gb_mesh_face_list_kill(impl->faces, gb_mesh_edge_lface(edge));

        // kill the edge
        gb_mesh_edge_list_kill(impl->edges, edge);

        // ok
        return tb_true;
    }
    // is isolated loop edge?
    else if (gb_mesh_edge_is_isolated_loop(edge))
    {
        // check
        tb_assert_abort(gb_mesh_edge_org(edge) == gb_mesh_edge_dst(edge));
        tb_assert_abort(gb_mesh_edge_lface(edge) != gb_mesh_edge_rface(edge));

        // kill the vertex
        gb_mesh_vertex_list_kill(impl->vertices, gb_mesh_edge_org(edge));

        // kill the left and right face
        gb_mesh_face_list_kill(impl->faces, gb_mesh_edge_lface(edge));
        gb_mesh_face_list_kill(impl->faces, gb_mesh_edge_rface(edge));

        // kill the edge
        gb_mesh_edge_list_kill(impl->edges, edge);

        // ok
        return tb_true;
    }

    // no isolated
    return tb_false;
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
    tb_assert_and_check_return_val(impl && impl->faces, tb_null);

    // the face iterator
    return gb_mesh_face_list_itor(impl->faces);
}
tb_char_t const* gb_mesh_face_cstr(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->faces, tb_null);
  
    // the info
    return gb_mesh_face_list_cstr(impl->faces, face, data, maxn);
}
tb_cpointer_t gb_mesh_face_data(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->faces && face, tb_null);

    // the face data
    return gb_mesh_face_list_data(impl->faces, face);
}
tb_void_t gb_mesh_face_data_set(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, tb_cpointer_t data)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->faces && face);

    // set the face data
    gb_mesh_face_list_data_set(impl->faces, face, data);
}
tb_iterator_ref_t gb_mesh_vertex_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices, tb_null);

    // the vertex iterator
    return gb_mesh_vertex_list_itor(impl->vertices);
}
tb_char_t const* gb_mesh_vertex_cstr(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices, tb_null);
  
    // the info
    return gb_mesh_vertex_list_cstr(impl->vertices, vertex, data, maxn);
}
tb_cpointer_t gb_mesh_vertex_data(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices && vertex, tb_null);

    // the vertex data
    return gb_mesh_vertex_list_data(impl->vertices, vertex);
}
tb_void_t gb_mesh_vertex_data_set(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, tb_cpointer_t data)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->vertices && vertex);

    // set the vertex data
    gb_mesh_vertex_list_data_set(impl->vertices, vertex, data);
}
tb_iterator_ref_t gb_mesh_edge_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->edges, tb_null);

    // the edge iterator
    return gb_mesh_edge_list_itor(impl->edges);
}
tb_char_t const* gb_mesh_edge_cstr(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->edges, tb_null);
  
    // the info
    return gb_mesh_edge_list_cstr(impl->edges, edge, data, maxn);
}
tb_cpointer_t gb_mesh_edge_data(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->edges && edge, tb_null);

    // the edge data
    return gb_mesh_edge_list_data(impl->edges, edge);
}
tb_void_t gb_mesh_edge_data_set(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge, tb_cpointer_t data)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->edges && edge);

    // set the edge data
    gb_mesh_edge_list_data_set(impl->edges, edge, data);
}
gb_mesh_edge_ref_t gb_mesh_edge_make(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices && impl->faces && impl->edges, tb_null);

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

        // init the sym edge
        gb_mesh_edge_org_set  (edge_sym, dst);
        gb_mesh_edge_lface_set(edge_sym, face);

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
gb_mesh_edge_ref_t gb_mesh_edge_make_loop(gb_mesh_ref_t mesh, tb_bool_t is_ccw)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices && impl->faces && impl->edges, tb_null);

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
        edge = gb_mesh_edge_list_make_loop(impl->edges, is_ccw);
        tb_assert_and_check_break(edge);

        // the sym edge
        gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge);
        tb_assert_and_check_break(edge_sym);

        // init the edge
        gb_mesh_edge_org_set  (edge, vertex);
        gb_mesh_edge_lface_set(edge, lface);

        // init the sym edge
        gb_mesh_edge_org_set  (edge_sym, vertex);
        gb_mesh_edge_lface_set(edge_sym, rface);

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
gb_mesh_face_ref_t gb_mesh_edge_make_at_face(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, gb_mesh_vertex_ref_t org, gb_mesh_vertex_ref_t dst, gb_mesh_edge_ref_t* pedge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->faces && impl->edges && face && org && dst, tb_null);

    // done
    tb_bool_t           ok = tb_false;
    gb_mesh_edge_ref_t  edge_new = tb_null;
    gb_mesh_face_ref_t  face_new = tb_null;
    do
    {
        // check
        gb_mesh_check_face(face);
        gb_mesh_check_vertex(org);
        gb_mesh_check_vertex(dst);

        // get the face edge
        gb_mesh_edge_ref_t edge = gb_mesh_face_edge(face);
        tb_assert_and_check_break(edge);

        // get the edge with edge.org == org
        gb_mesh_edge_ref_t edge_org = gb_mesh_orbit_edge_with_org(edge, org);
        tb_assert_abort_and_check_break(edge_org);

        // get the edge with edge.org == dst
        gb_mesh_edge_ref_t edge_dst = gb_mesh_orbit_edge_with_org(edge, dst);
        tb_assert_abort_and_check_break(edge_dst);

        // make the new face
        face_new = gb_mesh_face_list_make(impl->faces);
        tb_assert_and_check_break(face_new);

        // make the new non-loop edge
        edge_new = gb_mesh_edge_list_make(impl->edges);
        tb_assert_and_check_break(edge_new);

        // the new sym edge
        gb_mesh_edge_ref_t edge_sym_new = gb_mesh_edge_sym(edge_new);
        tb_assert_and_check_break(edge_sym_new);

        /* insert the edge at face
         *
         * before:
         *                    
         *           edge_org                    / \
         *  org ------------------->              |
         *   |                      |             |
         *   |         face         |             | edge_new
         *   |                      |             |
         *    <------------------- dst            |
         *           edge_dst
         *
         * gb_mesh_splice_edge(edge_dst, edge_sym_new):
         *
         *
         *           edge_org
         *  org ------------------->
         *   |                      |
         *   |         face         |
         *   |                      |
         *    <------------------- dst
         *           edge_dst      / \
         *                          |
         *                          | edge_new
         *                          |
         *                          |
         *
         * gb_mesh_splice_edge(edge_org, edge_new):
         * 
         *
         *           edge_org
         *  org ------------------->
         *   |  .                    |
         *   |  face    .  face_new  |
         *   |                 .     |
         *    <------------------- dst
         *           edge_dst      
         */
        gb_mesh_splice_edge(edge_dst, edge_sym_new);
        gb_mesh_splice_edge(edge_org, edge_new);

        // init the new edge
        gb_mesh_edge_org_set  (edge_new, gb_mesh_edge_org(edge_org));
        gb_mesh_edge_dst_set  (edge_new, gb_mesh_edge_org(edge_dst));
        gb_mesh_edge_lface_set(edge_new, gb_mesh_edge_lface(edge_dst));

        // update left face for all edges in the right orbit of the new edge
        gb_mesh_orbit_edge_set_lface(edge_sym_new, face_new);

        // save edge
        if (pedge) *pedge = edge_new;

        // ok
        ok = tb_true;

    } while (0);

    // failed
    if (!ok)
    {
        // kill the new edge
        if (edge_new) gb_mesh_edge_list_kill(impl->edges, edge_new);
        edge_new = tb_null;

        // kill the new face
        if (face_new) gb_mesh_face_list_kill(impl->faces, face_new);
        face_new = tb_null;
    }

    // ok?
    return face_new;
}
tb_void_t gb_mesh_edge_kill_at_face(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->faces && impl->edges && edge);

    // check
    gb_mesh_check_edge(edge);

    // isolated edge? kill it directly
    if (gb_mesh_kill_isolated_edge(impl, edge)) return ;

    // get the edge with edge.org == org
    gb_mesh_edge_ref_t edge_org = gb_mesh_edge_oprev(edge);
    tb_assert_abort_and_check_return(edge_org);

    // get the edge with edge.dst == dst
    gb_mesh_edge_ref_t edge_dst = gb_mesh_edge_lnext(edge);
    tb_assert_abort_and_check_return(edge_dst);

    // the sym edge
    gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge);
    tb_assert_and_check_return(edge_sym);

    /* use edge_org for edge_dst if the right face is inside a loop
     *
     * before
     *         
     *  --> dst/org -------------------->          
     * |  rface |  .
     *  --------      .
     *  edge_org'        . edge_dst
     *             edge_org .
     *                         .
     *
     */
    if (edge_org == edge_sym) edge_org = edge_dst;

    /* kill edge at face
     *
     * before:
     *         
     *            
     *          org -------------------->
     *           |  .                   /|\
     * edge_org  | rface    e    lface   | edge_dst
     *          \|/                .     |
     *            <------------------- dst
     *                            
     * gb_mesh_splice_edge(edge_dst, edge_sym):
     *
     * 
     *          org -------------------->
     *           |  .                   /|\
     * edge_org  | rface    e    lface   | edge_dst
     *          \|/                .     |
     *            <------------------X dst
     *
     * gb_mesh_splice_edge(edge_org, edge):
     *            
     *          org -------------------->
     *           |  X                   /|\
     * edge_org  | rface    e    lface   | edge_dst
     *          \|/                X     |
     *            <------------------- dst
     *
     *          org -------------------->
     *           |                      /|\
     * edge_org  |         lface         | edge_dst
     *          \|/                      |
     *            <------------------- dst
     */
    gb_mesh_splice_edge(edge_dst, edge_sym);
    gb_mesh_splice_edge(edge_org, edge);

    // update left face for all edges leaving the right orbit of the deleted edge
    gb_mesh_orbit_edge_set_lface(edge_org, gb_mesh_edge_lface(edge_dst));

    // update the reference edge, the old reference edge may have been deleted
    gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_org),     edge_org);
    gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_dst),     edge_dst);
    gb_mesh_face_edge_set  (gb_mesh_edge_lface(edge_dst),   edge_dst);

    // kill the edge and the right face of the edge
    gb_mesh_face_list_kill(impl->faces, gb_mesh_edge_rface(edge));
    gb_mesh_edge_list_kill(impl->edges, edge);
}
gb_mesh_vertex_ref_t gb_mesh_edge_make_at_vertex(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, gb_mesh_face_ref_t lface, gb_mesh_face_ref_t rface, gb_mesh_edge_ref_t* pedge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices && impl->edges && vertex && lface && rface, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_mesh_edge_ref_t      edge_new = tb_null;
    gb_mesh_vertex_ref_t    vertex_new = tb_null;
    do
    {
        // check
        gb_mesh_check_face(lface);
        gb_mesh_check_face(rface);
        gb_mesh_check_vertex(vertex);

        // get the vertex edge
        gb_mesh_edge_ref_t edge = gb_mesh_vertex_edge(vertex);
        tb_assert_abort_and_check_break(edge);

        // get the edge with edge.lface == lface
        gb_mesh_edge_ref_t edge_lf = gb_mesh_orbit_edge_with_lface(edge, lface);
        tb_assert_abort_and_check_break(edge_lf);

        // get the edge with edge.lface == rface
        gb_mesh_edge_ref_t edge_rf = gb_mesh_orbit_edge_with_lface(edge, rface);
        tb_assert_abort_and_check_break(edge_rf);

        // make the new vertex
        vertex_new = gb_mesh_vertex_list_make(impl->vertices);
        tb_assert_and_check_break(vertex_new);

        // make the new clockwise self-loop edge
        edge_new = gb_mesh_edge_list_make_loop(impl->edges, tb_false);
        tb_assert_and_check_break(edge_new);

        // the new sym edge
        gb_mesh_edge_ref_t edge_sym_new = gb_mesh_edge_sym(edge_new);
        tb_assert_and_check_break(edge_sym_new);

        /* insert the edge at vertex
         *
         * before:
         *
         *        edge_new
         *          ----
         *         |    |
         *          <---
         *
         *
         *      .                         lface
         *          .
         *              .    
         *      edge_rf     .               edge_lf
         *  <----------------- vertex ------------------->
         *                  .         .        
         *   rface     .                   . 
         *         .                            . 
         *     .                                     .
         *
         * splice(edge_lf, edge_new):
         *
         *
         *      .                         lface
         *          .         edge_new
         *              .       --->
         *      edge_rf     .  |    |        edge_lf
         *  <----------------- vertex ------------------->
         *                  .         .        
         *   rface     .                   . 
         *         .                            . 
         *     .                                     .
         *
         * splice(edge_rf, edge_sym_new):
         *
         *
         *
         *      .                                     lface
         *          .              edge_sym_new
         *              .       <----------------
         *      edge_rf     .  |                 |           edge_lf
         *  <----------------- vertex       vertex_new  ------------------->
         *                  .                           .        
         *   rface     .                                   . 
         *         .                                          . 
         *     .                                                 .
         *
         *
         *
         *      .                                     lface
         *          .               
         *              .        
         *      edge_rf     .          edge_new                   edge_lf
         *  <----------------- vertex ----------> vertex_new ------------------->
         *                  .                                  .        
         *   rface     .                                          . 
         *         .                                                 . 
         *     .                                                        .
         */       
        gb_mesh_splice_edge(edge_lf, edge_new);
        gb_mesh_splice_edge(edge_rf, edge_sym_new);

        // init the new edge
        gb_mesh_edge_org_set  (edge_new, vertex);
        gb_mesh_edge_lface_set(edge_new, lface);
        gb_mesh_edge_rface_set(edge_new, rface);

        // update origin for all edges leaving the destination orbit of the new edge
        gb_mesh_orbit_edge_set_org(edge_sym_new, vertex_new);

        // save edge
        if (pedge) *pedge = edge_new;

        // ok
        ok = tb_true;

    } while (0);

    // failed
    if (!ok)
    {
        // kill the new edge
        if (edge_new) gb_mesh_edge_list_kill(impl->edges, edge_new);
        edge_new = tb_null;

        // kill the new vertex
        if (vertex_new) gb_mesh_vertex_list_kill(impl->vertices, vertex_new);
        vertex_new = tb_null;
    }

    // ok?
    return vertex_new;
}
tb_void_t gb_mesh_edge_kill_at_vertex(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->vertices && impl->edges && edge);

    // check
    gb_mesh_check_edge(edge);

    // isolated edge? kill it directly
    if (gb_mesh_kill_isolated_edge(impl, edge)) return ;

    // get the edge with edge.lface == lface
    gb_mesh_edge_ref_t edge_lf = gb_mesh_edge_lnext(edge);
    tb_assert_abort_and_check_return(edge_lf);

    // get the edge with edge.lface == rface
    gb_mesh_edge_ref_t edge_rf = gb_mesh_edge_oprev(edge);
    tb_assert_abort_and_check_return(edge_rf);

    // the sym edge
    gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge);
    tb_assert_and_check_return(edge_sym);

    /* use edge_rf for edge_lf if the destination vertex is isolated
     *
     * before:
     *
     *      .                           lface  
     *          .               
     *              .            edge_lf'
     *                 .       <---------        
     *  <----------------- org ----------> dst 
     *                  .         edge            
     *             .                              
     *         . edge_rf        rface                
     *     .  edge_lf                                 
     *
     *
     */
    if (edge_lf == edge_sym) edge_lf = edge_rf;

    /* kill edge at vertex
     *
     * before:
     *
     *      .                                 lface  
     *          .               
     *              .        
     *                 .         edge              edge_lf
     *  <----------------- org ----------> dst ------------------->
     *                  .                      .        
     *             .                              . 
     *         . edge_rf        rface                . 
     *     .                                            .
     *
     *
     * gb_mesh_splice_edge(edge_rf, edge_sym):
     *
     *      .                                 lface  
     *          .               
     *              .        
     *                 .        edge_sym           edge_lf
     *  <----------------- org <---------- dst ------------------->
     *                  .                      .        
     *             .                              . 
     *         . edge_rf        rface                . 
     *     .                                            .
     *
     *
     *      .                                 lface  
     *          .               
     *              .        
     *                 .    edge_sym      edge_lf
     *  <----------------- org/dst ------------------->
     *                  .   |    |   .        
     *             .        <----       . 
     *         . edge_rf        rface      . 
     *     .                                  .
     *
     *
     * gb_mesh_splice_edge(edge_lf, edge):
     *
     *      .                                 lface  
     *          .               
     *              .        
     *                 .     edge       edge_lf
     *  <----------------- org/dst ------------------->
     *                  .   |    |   .        
     *             .         --->       . 
     *         . edge_rf        rface      . 
     *     .                                  .
     *
     */       
    gb_mesh_splice_edge(edge_rf, edge_sym);
    gb_mesh_splice_edge(edge_lf, edge);

    // update origin for all edges leaving the destination orbit of the deleted edge
    gb_mesh_orbit_edge_set_org(edge_lf, gb_mesh_edge_org(edge_rf));

    // update the reference edge, the old reference edge may have been deleted
    gb_mesh_vertex_edge_set (gb_mesh_edge_org(edge_rf),     edge_rf);
    gb_mesh_face_edge_set   (gb_mesh_edge_lface(edge_rf),   edge_rf);
    gb_mesh_face_edge_set   (gb_mesh_edge_lface(edge_lf),   edge_lf);

    // kill the edge and the destination vertex of the edge
    gb_mesh_vertex_list_kill(impl->vertices, gb_mesh_edge_dst(edge));
    gb_mesh_edge_list_kill(impl->edges, edge);
}
#ifdef __gb_debug__
tb_void_t gb_mesh_dump(gb_mesh_ref_t mesh)
{
    // trace
    tb_trace_i("");
    tb_trace_i("edges:");

    // dump edges
    tb_char_t data[8192];
    tb_for_all_if (gb_mesh_edge_ref_t, edge, gb_mesh_edge_itor(mesh), edge)
    {
        // trace
        tb_trace_i("    %s", gb_mesh_edge_cstr(mesh, edge, data, sizeof(data)));
    }

    // trace
    tb_trace_i("faces:");

    // dump faces
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(mesh), face)
    {
        // trace
        tb_trace_i("    face: %s", gb_mesh_face_cstr(mesh, face, data, sizeof(data)));

        // dump face.edges
        gb_mesh_edge_ref_t head = gb_mesh_face_edge(face);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // trace
            tb_trace_i("        %s", gb_mesh_edge_cstr(mesh, edge, data, sizeof(data)));

            // the next edge
            edge = gb_mesh_edge_lnext(edge);

        } while (edge != head);
    }

    // trace
    tb_trace_i("vertices:");

    // dump vertices
    tb_for_all_if (gb_mesh_vertex_ref_t, vertex, gb_mesh_vertex_itor(mesh), vertex)
    {
        // trace
        tb_trace_i("    vertex: %s", gb_mesh_vertex_cstr(mesh, vertex, data, sizeof(data)));

        // dump vertex.edges
        gb_mesh_edge_ref_t head = gb_mesh_vertex_edge(vertex);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // trace
            tb_trace_i("        %s", gb_mesh_edge_cstr(mesh, edge, data, sizeof(data)));

            // the next edge
            edge = gb_mesh_edge_onext(edge);

        } while (edge != head);
    }
}
tb_void_t gb_mesh_check(gb_mesh_ref_t mesh)
{
    // check edges
    tb_for_all_if (gb_mesh_edge_ref_t, edge, gb_mesh_edge_itor(mesh), edge)
    {
        // check edge
        gb_mesh_check_edge(edge);
        tb_assert_abort(gb_mesh_edge_sym(edge) != edge);
        tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_sym(edge)) == edge);
        tb_assert_abort(gb_mesh_edge_org(edge));
        tb_assert_abort(gb_mesh_edge_dst(edge));
        tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_onext(gb_mesh_edge_lnext(edge))) == edge);
        tb_assert_abort(gb_mesh_edge_lnext(gb_mesh_edge_sym(gb_mesh_edge_onext(edge))) == edge);
    }

    // check faces
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(mesh), face)
    {
        // check face
        gb_mesh_check_face(face);

        // check face.edges
        gb_mesh_edge_ref_t head = gb_mesh_face_edge(face);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // check edge
            gb_mesh_check_edge(edge);
            tb_assert_abort(gb_mesh_edge_sym(edge) != edge);
            tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_sym(edge)) == edge);
            tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_onext(gb_mesh_edge_lnext(edge))) == edge);
            tb_assert_abort(gb_mesh_edge_lnext(gb_mesh_edge_sym(gb_mesh_edge_onext(edge))) == edge);
            tb_assert_abort(gb_mesh_edge_lface(edge) == face);

            // the next edge
            edge = gb_mesh_edge_lnext(edge);

        } while (edge != head);
    }

    // check vertices
    tb_for_all_if (gb_mesh_vertex_ref_t, vertex, gb_mesh_vertex_itor(mesh), vertex)
    {
        // check vertex
        gb_mesh_check_vertex(vertex);

        // check vertex.edges
        gb_mesh_edge_ref_t head = gb_mesh_vertex_edge(vertex);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // check edge
            gb_mesh_check_edge(edge);
            tb_assert_abort(gb_mesh_edge_sym(edge) != edge);
            tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_sym(edge)) == edge);
            tb_assert_abort(gb_mesh_edge_sym(gb_mesh_edge_onext(gb_mesh_edge_lnext(edge))) == edge);
            tb_assert_abort(gb_mesh_edge_lnext(gb_mesh_edge_sym(gb_mesh_edge_onext(edge))) == edge);
            tb_assert_abort(gb_mesh_edge_org(edge) == vertex);

            // the next edge
            edge = gb_mesh_edge_onext(edge);

        } while (edge != head);
    }
}
#endif
