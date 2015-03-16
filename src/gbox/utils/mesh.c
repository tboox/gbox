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
#include "impl/mesh/mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// set the face edge
#define gb_mesh_face_edge_set(face, val)        do { tb_assert(face); (face)->edge = (val); } while (0)

// set the vertex edge
#define gb_mesh_vertex_edge_set(vertex, val)    do { tb_assert(vertex); (vertex)->edge = (val); } while (0)

// set the edge sym
#define gb_mesh_edge_sym_set(edge, val)         do { tb_assert(edge); (edge)->sym = (val); } while (0)

// set the edge org
#define gb_mesh_edge_org_set(edge, val)         do { tb_assert(edge); (edge)->org = (val); if (val) gb_mesh_vertex_edge_set(val, edge); } while (0)

// set the edge dst
#define gb_mesh_edge_dst_set(edge, val)         do { tb_assert((edge) && (edge)->sym); (edge)->sym->org = (val); if (val) gb_mesh_vertex_edge_set(val, (edge)->sym); } while (0)

// set the edge lface
#define gb_mesh_edge_lface_set(edge, val)       do { tb_assert(edge); (edge)->lface = (val); if (val) gb_mesh_face_edge_set(val, edge); } while (0)

// set the edge lface
#define gb_mesh_edge_rface_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->lface = (val); if (val) gb_mesh_face_edge_set(val, (edge)->sym); } while (0)

// set the edge onext
#define gb_mesh_edge_onext_set(edge, val)       do { tb_assert(edge); (edge)->onext = (val); } while (0)

// set the edge oprev
#define gb_mesh_edge_oprev_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->lnext = (val); } while (0)

// set the edge lnext
#define gb_mesh_edge_lnext_set(edge, val)       do { tb_assert(edge); (edge)->lnext = (val); } while (0)

// set the edge lprev
#define gb_mesh_edge_lprev_set(edge, val)       do { tb_assert((edge) && (edge)->onext); (edge)->onext->sym = (val); } while (0)

// set the edge rnext
#define gb_mesh_edge_rnext_set(edge, val)       do { tb_assert(gb_mesh_edge_oprev(edge)); gb_mesh_edge_oprev(edge)->sym = (val); } while (0)

// set the edge rprev
#define gb_mesh_edge_rprev_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->onext = (val); } while (0)

// set the edge dnext
#define gb_mesh_edge_dnext_set(edge, val)       do { tb_assert(gb_mesh_edge_rprev(edge)); gb_mesh_edge_rprev(edge)->sym = (val); } while (0)

// set the edge rprev
#define gb_mesh_edge_dprev_set(edge, val)       do { tb_assert((edge) && (edge)->lnext); (edge)->lnext->sym = (val); } while (0)

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

    // the listener
    gb_mesh_listener_t              listener;

    // the user private data of the listener 
    tb_cpointer_t                   listener_priv;

    // the observed listener events
    tb_size_t                       listener_events;

}gb_mesh_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
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
 *          .                                          .
 * .        .        .                        .        .        .
 *   .      .      .                            .      .      .
 *     .    .    .                                .    .    .
 *    a  .  .  . x                               a  .  .  . x
 *         ...                                        ...
 *          .                              a.lface     .       b.lface
 *                                                y  .   .
 *  a.lface      b.lface                           .       .
 *          .                                    .           . b
 *     y  .   .                                                .
 *      .       .                                                .
 *    .           . b                                              .
 *                  .
 *                    .
 *
 *                             splice
 *  a.lface == b.lface          <=>             a.lface != b.lface
 */
static tb_void_t gb_mesh_splice_edge(gb_mesh_edge_ref_t a, gb_mesh_edge_ref_t b)
{
    // check
    tb_assert_abort(a && b && a != b);

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
static tb_void_t gb_mesh_save_face_at_orbit(gb_mesh_edge_ref_t edge, gb_mesh_face_ref_t lface)
{
    // check
    tb_assert_abort(edge);

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
static tb_void_t gb_mesh_save_vertex_at_orbit(gb_mesh_edge_ref_t edge, gb_mesh_vertex_ref_t org)
{
    // check
    tb_assert_abort(edge);

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
static __tb_inline__ tb_void_t gb_mesh_post_event(gb_mesh_impl_t* impl, tb_size_t type, tb_pointer_t org, tb_pointer_t dst)
{
    // this event is observing? done listener
    if ((impl->listener_events & type) && impl->listener)
    {
        // init event
        gb_mesh_event_t event = {type, org, dst, impl->listener_priv};

        // done event
        impl->listener(&event);
    }
}
static __tb_inline__ gb_mesh_edge_ref_t gb_mesh_make_edge(gb_mesh_impl_t* impl, tb_bool_t is_loop, tb_bool_t is_ccw)
{
    // check
    tb_assert_and_check_return_val(impl && impl->edges, tb_null);

    // make the edge
    gb_mesh_edge_ref_t edge = is_loop? gb_mesh_edge_list_make_loop(impl->edges, is_ccw) : gb_mesh_edge_list_make(impl->edges);
    tb_assert_and_check_return_val(edge, tb_null);

    // post the init event
    gb_mesh_post_event(impl, GB_MESH_EVENT_EDGE_INIT, edge, tb_null);

    // ok
    return edge;
}
static __tb_inline__ gb_mesh_face_ref_t gb_mesh_make_face(gb_mesh_impl_t* impl)
{
    // check
    tb_assert_and_check_return_val(impl && impl->faces, tb_null);

    // make the face
    gb_mesh_face_ref_t face = gb_mesh_face_list_make(impl->faces);

    // post the init event
    gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_INIT, face, tb_null);

    // ok
    return face;
}
static __tb_inline__ gb_mesh_face_ref_t gb_mesh_make_face_at_orbit(gb_mesh_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return_val(impl && edge, tb_null);

    // make the new face
    gb_mesh_face_ref_t face_new = gb_mesh_make_face(impl);
    tb_assert_and_check_return_val(face_new, tb_null);

    // update left face for all edges in the orbit of the edge
    gb_mesh_save_face_at_orbit(edge, face_new);

    // ok
    return face_new;
}
static __tb_inline__ gb_mesh_vertex_ref_t gb_mesh_make_vertex(gb_mesh_impl_t* impl)
{
    // check
    tb_assert_and_check_return_val(impl && impl->vertices, tb_null);

    // make the vertex
    gb_mesh_vertex_ref_t vertex = gb_mesh_vertex_list_make(impl->vertices);

    // post the init event
    gb_mesh_post_event(impl, GB_MESH_EVENT_VERTEX_INIT, vertex, tb_null);

    // ok
    return vertex;
}
static __tb_inline__ gb_mesh_vertex_ref_t gb_mesh_make_vertex_at_orbit(gb_mesh_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return_val(impl && edge, tb_null);

    // make the new vertex
    gb_mesh_vertex_ref_t vertex_new = gb_mesh_make_vertex(impl);
    tb_assert_and_check_return_val(vertex_new, tb_null);

    // update origin for all edges leaving the orbit of the edge
    gb_mesh_save_vertex_at_orbit(edge, vertex_new);

    // ok
    return vertex_new;
}
static __tb_inline__ tb_void_t gb_mesh_kill_edge(gb_mesh_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return(impl && impl->edges && edge);

    // post the exit event
    gb_mesh_post_event(impl, GB_MESH_EVENT_EDGE_EXIT, edge, tb_null);

    // kill the edge
    gb_mesh_edge_list_kill(impl->edges, edge);
}
static __tb_inline__ tb_void_t gb_mesh_kill_face(gb_mesh_impl_t* impl, gb_mesh_face_ref_t face)
{
    // check
    tb_assert_and_check_return(impl && impl->faces && face);

    // post the exit event
    gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_EXIT, face, tb_null);

    // kill the face
    gb_mesh_face_list_kill(impl->faces, face);
}
static __tb_inline__ tb_void_t gb_mesh_kill_face_at_orbit(gb_mesh_impl_t* impl, gb_mesh_face_ref_t face, gb_mesh_face_ref_t face_new)
{
    // check
    tb_assert_and_check_return(impl && face);

    // update lface for all edges leaving the deleted face
    gb_mesh_save_face_at_orbit(gb_mesh_face_edge(face), face_new);

    // kill the face
    gb_mesh_kill_face(impl, face);
}
static __tb_inline__ tb_void_t gb_mesh_kill_vertex(gb_mesh_impl_t* impl, gb_mesh_vertex_ref_t vertex)
{
    // check
    tb_assert_and_check_return(impl && impl->vertices && vertex);

    // post the exit event
    gb_mesh_post_event(impl, GB_MESH_EVENT_VERTEX_EXIT, vertex, tb_null);

    // kill the vertex
    gb_mesh_vertex_list_kill(impl->vertices, vertex);
}
static __tb_inline__ tb_void_t gb_mesh_kill_vertex_at_orbit(gb_mesh_impl_t* impl, gb_mesh_vertex_ref_t vertex, gb_mesh_vertex_ref_t org_new)
{
    // check
    tb_assert_and_check_return(impl && vertex);

    // update origin for all edges leaving the deleted vertex
    gb_mesh_save_vertex_at_orbit(gb_mesh_vertex_edge(vertex), org_new);

    // kill the vertex
    gb_mesh_kill_vertex(impl, vertex);
}
static tb_bool_t gb_mesh_kill_isolated_edge(gb_mesh_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return_val(impl && edge, tb_false);

    // is isolated edge?
    if (gb_mesh_edge_is_isolated(edge))
    {
        // check
        tb_assert_abort(gb_mesh_edge_org(edge) != gb_mesh_edge_dst(edge));
        tb_assert_abort(gb_mesh_edge_lface(edge) == gb_mesh_edge_rface(edge));

        // kill the origin and destination vertices
        gb_mesh_kill_vertex(impl, gb_mesh_edge_org(edge));
        gb_mesh_kill_vertex(impl, gb_mesh_edge_dst(edge));

        // kill the face
        gb_mesh_kill_face(impl, gb_mesh_edge_lface(edge));

        // kill the edge
        gb_mesh_kill_edge(impl, edge);

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
        gb_mesh_kill_vertex(impl, gb_mesh_edge_org(edge));

        // kill the left and right face
        gb_mesh_kill_face(impl, gb_mesh_edge_lface(edge));
        gb_mesh_kill_face(impl, gb_mesh_edge_rface(edge));

        // kill the edge
        gb_mesh_kill_edge(impl, edge);

        // ok
        return tb_true;
    }

    // no isolated
    return tb_false;
}
#ifdef __gb_debug__
static tb_long_t gb_mesh_printf_edge(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the edge
    gb_mesh_edge_ref_t edge = (gb_mesh_edge_ref_t)object;

    // make info
    return gb_mesh_edge_list_cstr(edge->list, edge, cstr, maxn);
}
static tb_long_t gb_mesh_printf_face(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the face
    gb_mesh_face_ref_t face = (gb_mesh_face_ref_t)object;

    // make info
    return gb_mesh_face_list_cstr(face->list, face, cstr, maxn);
}
static tb_long_t gb_mesh_printf_vertex(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the vertex
    gb_mesh_vertex_ref_t vertex = (gb_mesh_vertex_ref_t)object;

    // make info
    return gb_mesh_vertex_list_cstr(vertex->list, vertex, cstr, maxn);
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_ref_t gb_mesh_init(tb_element_t edge_element, tb_element_t face_element, tb_element_t vertex_element)
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
        impl->edges = gb_mesh_edge_list_init(edge_element);
        tb_assert_and_check_break(impl->edges);
  
        // init faces
        impl->faces = gb_mesh_face_list_init(face_element);
        tb_assert_and_check_break(impl->faces);

        // init vertices
        impl->vertices = gb_mesh_vertex_list_init(vertex_element);
        tb_assert_and_check_break(impl->vertices);

#ifdef __gb_debug__
        /* register printf("%{mesh_face}",      face);
         * register printf("%{mesh_edge}",      edge);
         * register printf("%{mesh_vertex}",    vertex);
         */
        tb_printf_object_register("mesh_edge",      gb_mesh_printf_edge);
        tb_printf_object_register("mesh_face",      gb_mesh_printf_face);
        tb_printf_object_register("mesh_vertex",    gb_mesh_printf_vertex);
#endif

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
tb_bool_t gb_mesh_is_empty(gb_mesh_ref_t mesh)
{
    // the edges is non-empty?
    if (tb_iterator_size(gb_mesh_edge_itor(mesh))) return tb_false;

    // the faces is non-empty?
    if (tb_iterator_size(gb_mesh_face_itor(mesh))) return tb_false;
 
    // the vertices is non-empty?
    if (tb_iterator_size(gb_mesh_vertex_itor(mesh))) return tb_false;

    // is empty
    return tb_true;
}
tb_void_t gb_mesh_listener_set(gb_mesh_ref_t mesh, gb_mesh_listener_t listener, tb_cpointer_t priv)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl);

    // set listener
    impl->listener      = listener;
    impl->listener_priv = priv;
}
tb_void_t gb_mesh_listener_event_add(gb_mesh_ref_t mesh, tb_size_t events)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl);

    // add listener events
    impl->listener_events |= events;
}
tb_void_t gb_mesh_listener_event_del(gb_mesh_ref_t mesh, tb_size_t events)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl);

    // delete listener events
    impl->listener_events &= ~events;
}
tb_iterator_ref_t gb_mesh_vertex_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices, tb_null);

    // the vertex iterator
    return gb_mesh_vertex_list_itor(impl->vertices);
}
gb_mesh_vertex_ref_t gb_mesh_vertex_head(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_vertex_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the head vertex
    return (gb_mesh_vertex_ref_t)tb_iterator_item(iterator, tb_iterator_head(iterator));
}
gb_mesh_vertex_ref_t gb_mesh_vertex_last(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_vertex_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the last vertex
    return (gb_mesh_vertex_ref_t)tb_iterator_item(iterator, tb_iterator_last(iterator));
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
tb_size_t gb_mesh_vertex_order(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->vertices, GB_MESH_ORDER_INSERT_TAIL);

    // the vertex order
    return gb_mesh_vertex_list_order(impl->vertices);
}
tb_void_t gb_mesh_vertex_order_set(gb_mesh_ref_t mesh, tb_size_t order)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->vertices);

    // set the vertex order
    gb_mesh_vertex_list_order_set(impl->vertices, order);
}
tb_iterator_ref_t gb_mesh_face_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->faces, tb_null);

    // the face iterator
    return gb_mesh_face_list_itor(impl->faces);
}
gb_mesh_face_ref_t gb_mesh_face_head(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_face_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the head face
    return (gb_mesh_face_ref_t)tb_iterator_item(iterator, tb_iterator_head(iterator));
}
gb_mesh_face_ref_t gb_mesh_face_last(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_face_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the last face
    return (gb_mesh_face_ref_t)tb_iterator_item(iterator, tb_iterator_last(iterator));
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
tb_size_t gb_mesh_face_order(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->faces, GB_MESH_ORDER_INSERT_TAIL);

    // the face order
    return gb_mesh_face_list_order(impl->faces);
}
tb_void_t gb_mesh_face_order_set(gb_mesh_ref_t mesh, tb_size_t order)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->faces);

    // set the face order
    gb_mesh_face_list_order_set(impl->faces, order);
}
tb_iterator_ref_t gb_mesh_edge_itor(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->edges, tb_null);

    // the edge iterator
    return gb_mesh_edge_list_itor(impl->edges);
}
gb_mesh_edge_ref_t gb_mesh_edge_head(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_edge_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the head edge
    return (gb_mesh_edge_ref_t)tb_iterator_item(iterator, tb_iterator_head(iterator));
}
gb_mesh_edge_ref_t gb_mesh_edge_last(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_edge_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the last edge
    return (gb_mesh_edge_ref_t)tb_iterator_item(iterator, tb_iterator_last(iterator));
}
gb_mesh_edge_ref_t gb_mesh_edge_tail(gb_mesh_ref_t mesh)
{
    // the iterator
    tb_iterator_ref_t iterator = gb_mesh_edge_itor(mesh);
    tb_assert_and_check_return_val(iterator, tb_null);

    // the tail edge
    return (gb_mesh_edge_ref_t)tb_iterator_item(iterator, tb_iterator_tail(iterator));
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
tb_size_t gb_mesh_edge_order(gb_mesh_ref_t mesh)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && impl->edges, GB_MESH_ORDER_INSERT_TAIL);

    // the edge order
    return gb_mesh_edge_list_order(impl->edges);
}
tb_void_t gb_mesh_edge_order_set(gb_mesh_ref_t mesh, tb_size_t order)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && impl->edges);

    // set the edge order
    gb_mesh_edge_list_order_set(impl->edges, order);
}
gb_mesh_edge_ref_t gb_mesh_edge_make(gb_mesh_ref_t mesh)
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
        org = gb_mesh_make_vertex(impl);
        tb_assert_and_check_break(org);

        // make the dst
        dst = gb_mesh_make_vertex(impl);
        tb_assert_and_check_break(dst);

        // make the face
        face = gb_mesh_make_face(impl);
        tb_assert_and_check_break(face);

        // make the edge
        edge = gb_mesh_make_edge(impl, tb_false, tb_false);
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
        if (org) gb_mesh_kill_vertex(impl, org);
        org = tb_null;

        // kill the dst
        if (dst) gb_mesh_kill_vertex(impl, dst);
        dst = tb_null;

        // kill the face
        if (face) gb_mesh_kill_face(impl, face);
        face = tb_null;

        // kill the edge
        if (edge) gb_mesh_kill_edge(impl, edge);
        edge = tb_null;
    }

    // ok?
    return edge;
}
gb_mesh_edge_ref_t gb_mesh_edge_make_loop(gb_mesh_ref_t mesh, tb_bool_t is_ccw)
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
        vertex = gb_mesh_make_vertex(impl);
        tb_assert_and_check_break(vertex);

        // make the left face
        lface = gb_mesh_make_face(impl);
        tb_assert_and_check_break(lface);

        // make the right face
        rface = gb_mesh_make_face(impl);
        tb_assert_and_check_break(rface);

        // make the edge
        edge = gb_mesh_make_edge(impl, tb_true, is_ccw);
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
        if (vertex) gb_mesh_kill_vertex(impl, vertex);
        vertex = tb_null;

        // kill the left face
        if (lface) gb_mesh_kill_face(impl, lface);
        lface = tb_null;

        // kill the right face
        if (rface) gb_mesh_kill_face(impl, rface);
        rface = tb_null;

        // kill the edge
        if (edge) gb_mesh_kill_edge(impl, edge);
        edge = tb_null;
    }

    // ok?
    return edge;
}
tb_void_t gb_mesh_edge_splice(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org, gb_mesh_edge_ref_t edge_dst)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && edge_org && edge_dst);

    // check edges
    gb_mesh_check_edge(edge_org);
    gb_mesh_check_edge(edge_dst);

    // done
    tb_bool_t joining_faces = tb_false;
    tb_bool_t joining_vertices = tb_false;
    do
    {
        // is same? ok
        tb_check_break(edge_org != edge_dst);

        // two vertices are disjoint? 
        if (gb_mesh_edge_org(edge_org) != gb_mesh_edge_org(edge_dst))
        {
            // joins the two vertices
            joining_vertices = tb_true;

            // remove the edge_dst.org first
            gb_mesh_kill_vertex_at_orbit(impl, gb_mesh_edge_org(edge_dst), gb_mesh_edge_org(edge_org));
        }
    
        // two faces are disjoint? 
        if (gb_mesh_edge_lface(edge_org) != gb_mesh_edge_lface(edge_dst))
        {
            // joins the two faces
            joining_faces = tb_true;

            // post the merge event, merge(edge_dst.lface, edge_org.lface) => edge_org.lface
            gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_MERGE, gb_mesh_edge_lface(edge_dst), gb_mesh_edge_lface(edge_org));

            // remove the edge_dst.lface first
            gb_mesh_kill_face_at_orbit(impl, gb_mesh_edge_lface(edge_dst), gb_mesh_edge_lface(edge_org));
        }

        // splice two edges
        gb_mesh_splice_edge(edge_dst, edge_org);

        // two vertices are disjoint?
        if (!joining_vertices) 
        {
            /* make new vertex at edge_dst.org
             * and update origin for all edges leaving the origin orbit of the edge_dst
             */
            gb_mesh_vertex_ref_t vertex_new = gb_mesh_make_vertex_at_orbit(impl, edge_dst);
            tb_assert_and_check_break(vertex_new);

            // update the reference edge, the old reference edge may have been deleted
            gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_org), edge_org);
        }

        // two faces are disjoint? 
        if (!joining_faces)
        {
            /* make new face at edge_dst.lface
             * and update lface for all edges leaving the left orbit of the edge_dst
             */
            gb_mesh_face_ref_t face_new = gb_mesh_make_face_at_orbit(impl, edge_dst);
            tb_assert_and_check_break(face_new);

            // post the split event, split(edge_org.lface) => (edge_org.lface, face_new)
            gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_SPLIT, gb_mesh_edge_lface(edge_org), face_new);

            // update the reference edge, the old reference edge may have been deleted
            gb_mesh_face_edge_set(gb_mesh_edge_lface(edge_org), edge_org);
        }

    } while (0);
}
gb_mesh_edge_ref_t gb_mesh_edge_append(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && edge_org, tb_null);

    // done
    tb_bool_t           ok = tb_false;
    gb_mesh_edge_ref_t  edge_new = tb_null;
    do
    {
        // check edge
        gb_mesh_check_edge(edge_org);

        // make the new non-loop edge
        edge_new = gb_mesh_make_edge(impl, tb_false, tb_false);
        tb_assert_and_check_break(edge_new);

        // the new sym edge
        gb_mesh_edge_ref_t edge_sym_new = gb_mesh_edge_sym(edge_new);
        tb_assert_and_check_break(edge_sym_new);

        /* append edge 
         *
         * before:
         *
         *                                  
         *                                lface
         *                       
         *       edge_org                    
         *  -----------------> vertex  
         *                       .    .        
         *          rface     .            . 
         *                .                     . 
         *             .                             .
         *          .
         *
         * after:
         *
         *                                lface
         *                       
         *        edge_org                edge_new
         *  -----------------> vertex ----------------> vertex_new
         *                       .    .        
         *          rface     .            . 
         *                .                     . 
         *             .                             .
         *          .
         *
         *
         */
        gb_mesh_splice_edge(edge_new, gb_mesh_edge_lnext(edge_org));

        // init the new edge
        gb_mesh_edge_org_set  (edge_new,        gb_mesh_edge_dst(edge_org));
        gb_mesh_edge_lface_set(edge_new,        gb_mesh_edge_lface(edge_org));
        gb_mesh_edge_lface_set(edge_sym_new,    gb_mesh_edge_lface(edge_org));

        /* make the new vertex
         * and update origin for all edges leaving the destination orbit of the new edge
         */
        if (!gb_mesh_make_vertex_at_orbit(impl, edge_sym_new)) break;

        // ok
        ok = tb_true;

    } while (0);

    // failed
    if (!ok)
    {
        // kill the new edge
        if (edge_new) gb_mesh_kill_edge(impl, edge_new);
        edge_new = tb_null;
    }

    // ok?
    return edge_new;
}
gb_mesh_edge_ref_t gb_mesh_edge_insert(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org, gb_mesh_edge_ref_t edge_dst)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && edge_org && edge_dst, tb_null);

    // done
    tb_bool_t           ok = tb_false;
    gb_mesh_edge_ref_t  edge_new = tb_null;
    do
    {
        // check edges 
        gb_mesh_check_edge(edge_org);
        gb_mesh_check_edge(edge_dst);

        // make the new clockwise self-loop edge
        edge_new = gb_mesh_make_edge(impl, tb_true, tb_false);
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
         *        .                         
         *             .                  edge_dst.lface
         *                  .    
         *      edge_org         .        edge_dst
         *  -----------------> vertex ------------------->
         *                       .    .        
         * edge_org.rface     .            . 
         *                .                     . 
         *             .                             .
         *          .
         *
         * splice(edge_dst, edge_new):
         *
         *        .                         
         *             .          edge_new   edge_dst.lface
         *                  .       --->
         *      edge_org         . |    |    edge_dst
         *  ---------------------> vertex ------------------->
         *                       .        .        
         * edge_org.rface     .                . 
         *                 .                        . 
         *              .                                .
         *           .
         * 
         * splice(gb_mesh_edge_sym(edge_org), edge_sym_new):
         *
         *
         *
         *      .                                     edge_dst.lface
         *          .              edge_sym_new
         *              .       <----------------
         *     edge_sym_org .  |                 |           edge_dst
         *  <----------------- vertex       vertex_new  ------------------->
         *                                  .           .        
         *                               .                 . 
         *          edge_org.rface    .                       . 
         *                         .                             .
         *                      .
         *
         *
         *      .                                            edge_dst.lface
         *          .              
         *              .      
         *       edge_org   .         edge_new                   edge_dst
         *  -----------------> vertex --------> vertex_new  ------------------->
         *                                    .           .        
         *                                 .                 . 
         *          edge_org.rface      .                       . 
         *                           .                             .
         *                        .
         */       
        gb_mesh_splice_edge(edge_dst, edge_new);
        gb_mesh_splice_edge(gb_mesh_edge_sym(edge_org), edge_sym_new);

        // init the new edge
        gb_mesh_edge_org_set  (edge_new, gb_mesh_edge_dst(edge_org));
        gb_mesh_edge_lface_set(edge_new, gb_mesh_edge_lface(edge_dst));
        gb_mesh_edge_rface_set(edge_new, gb_mesh_edge_rface(edge_org));

        /* make the new vertex
         * and update origin for all edges leaving the destination orbit of the new edge
         */
        if (!gb_mesh_make_vertex_at_orbit(impl, edge_sym_new)) break;

        // ok
        ok = tb_true;

    } while (0);

    // failed
    if (!ok)
    {
        // kill the new edge
        if (edge_new) gb_mesh_kill_edge(impl, edge_new);
        edge_new = tb_null;
    }

    // ok?
    return edge_new;
}
tb_void_t gb_mesh_edge_remove(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_del)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && edge_del);

    // check edge
    gb_mesh_check_edge(edge_del);

    // isolated edge? kill it directly
    if (gb_mesh_kill_isolated_edge(impl, edge_del)) return ;

    // get the destinate edge
    gb_mesh_edge_ref_t edge_dst = gb_mesh_edge_lnext(edge_del);
    tb_assert_abort_and_check_return(edge_dst);

    // get the original sym edge
    gb_mesh_edge_ref_t edge_sym_org = gb_mesh_edge_oprev(edge_del);
    tb_assert_abort_and_check_return(edge_sym_org);

    // the sym edge
    gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge_del);
    tb_assert_and_check_return(edge_sym);

    /* use edge_sym_org for edge_dst if the destination vertex is isolated
     *
     * before:
     *
     *      edge_org        edge_del
     * ----------------> --------------->
     * <---------------- <---------------
     *    edge_sym_org      edge_dst'
     *      edge_dst
     */
    if (edge_dst == edge_sym) edge_dst = edge_sym_org;
    /* use edge_dst for edge_sym_org if the original vertex is isolated
     *
     * before:
     *
     *    edge_sym_org'   edge_sym_org
     *      edge_del'       edge_dst
     * ----------------> --------------->
     * <---------------- <---------------
     *      edge_del
     */
    else if (edge_sym_org == edge_del)
    {
        // reverse edge
        tb_swap(gb_mesh_edge_ref_t, edge_del, edge_sym);

        // update edge_sym_org
        edge_sym_org = edge_dst;
    }

    /* kill the destination vertex of the edge
     * and update origin for all edges leaving the destination orbit of the deleted edge
     */
    gb_mesh_kill_vertex_at_orbit(impl, gb_mesh_edge_dst(edge_del), gb_mesh_edge_org(edge_sym_org));

    /* remove edge
     *
     * before:
     *
     *        .                         
     *             .                            edge_dst.lface
     *                  .    
     *      edge_sym_org     .          edge                       edge_dst
     *  <----------------- vertex ----------------> vertex_del ------------------->
     *                                                .    .        
     *                  edge_org.rface            .             . 
     *                                        .                      . 
     *                                    .                               .
     *                                .
     *
     * gb_mesh_splice_edge(edge_sym_org, edge_sym):
     *
     *        .                         
     *             .                            edge_dst.lface
     *                  .    
     *      edge_sym_org     .        edge_sym                      edge_dst
     *  <----------------- vertex <---------------- vertex_del ------------------->
     *                                                .    .        
     *                  edge_org.rface            .             . 
     *                                        .                      . 
     *                                    .                               .
     *                                .
     *
     *        .                         
     *             .                            edge_dst.lface
     *                  .    
     *      edge_sym_org     .  edge_sym           edge_dst
     *  <----------------- vertex / vertex_del ------------------->
     *                      |       |  .    .        
     *     edge_org.rface    <-----.             . 
     *                         .                      . 
     *                     .                               .
     *                 .
     *
     * gb_mesh_splice_edge(edge_dst, edge_del):
     *
     *        .                         
     *             .                            edge_dst.lface
     *                  .    
     *      edge_sym_org     . edge_del         edge_dst
     *  <----------------- vertex / vertex_del ------------------->
     *                      |       |  .    .        
     *     edge_org.rface    ----->.             . 
     *                         .                      . 
     *                     .                               .
     *                 .
     *
     *        .                         
     *             .                  edge_dst.lface
     *                  .    
     *      edge_org         .        edge_dst
     *  -----------------> vertex ------------------->
     *                       .    .        
     * edge_org.rface     .            . 
     *                .                     . 
     *             .                             .
     *          .
     *
     */       
    gb_mesh_splice_edge(edge_sym_org,   edge_sym);
    gb_mesh_splice_edge(edge_dst,       edge_del);

    // update the reference edge, the old reference edge may have been deleted
    gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_sym_org),     edge_sym_org);
    gb_mesh_face_edge_set  (gb_mesh_edge_lface(edge_sym_org),   edge_sym_org);
    gb_mesh_face_edge_set  (gb_mesh_edge_lface(edge_dst),       edge_dst);

    // kill the edge 
    gb_mesh_kill_edge(impl, edge_del);
}
gb_mesh_edge_ref_t gb_mesh_edge_connect(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org, gb_mesh_edge_ref_t edge_dst)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return_val(impl && edge_org && edge_dst, tb_null);

    // check edges
    gb_mesh_check_edge(edge_org);
    gb_mesh_check_edge(edge_dst);

    // done
    tb_bool_t           ok = tb_false;
    gb_mesh_edge_ref_t  edge_new = tb_null;
    tb_bool_t           joining_faces = tb_false;
    do
    {
        // make the new non-loop edge
        edge_new = gb_mesh_make_edge(impl, tb_false, tb_false);
        tb_assert_and_check_break(edge_new);

        // the new sym edge
        gb_mesh_edge_ref_t edge_sym_new = gb_mesh_edge_sym(edge_new);
        tb_assert_and_check_break(edge_sym_new);

        // two faces are disjoint? 
        if (gb_mesh_edge_lface(edge_org) != gb_mesh_edge_lface(edge_dst))
        {
            // joins the two faces
            joining_faces = tb_true;

            // post the merge event, merge(edge_dst.lface, edge_org.lface) => edge_org.lface
            gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_MERGE, gb_mesh_edge_lface(edge_dst), gb_mesh_edge_lface(edge_org));

            // remove the edge_dst.lface first
            gb_mesh_kill_face_at_orbit(impl, gb_mesh_edge_lface(edge_dst), gb_mesh_edge_lface(edge_org));
        }

        /* connect edge
         *
         * before:
         *
         *           face
         *
         *         edge_org                           
         *  ---------------------->             
         * |                                    edge_new
         * |         face                  ----------------->
         * |                                        
         *  <---------------------                   
         *         edge_dst                              
         *
         * gb_mesh_splice_edge(edge_new, gb_mesh_edge_lnext(edge_org)):
         *
         *           face
         *
         *         edge_org                           
         *  ----------------------> ----------------->             
         * |                            edge_new
         * |         face                  
         * |                                        
         *  <---------------------                   
         *         edge_dst                              
         *
         * gb_mesh_splice_edge(edge_sym_new, edge_dst):
         *
         *         edge_org                           
         *  ---------------------->          
         * |                       |    
         * |         face          | edge_new      face_new
         * |                      \|/                
         *  <---------------------                   
         *         edge_dst     
         */
        gb_mesh_splice_edge(edge_new, gb_mesh_edge_lnext(edge_org));
        gb_mesh_splice_edge(edge_sym_new, edge_dst);

        // init the new edge
        gb_mesh_edge_org_set  (edge_new,        gb_mesh_edge_dst(edge_org));
        gb_mesh_edge_org_set  (edge_sym_new,    gb_mesh_edge_org(edge_dst));
        gb_mesh_edge_lface_set(edge_sym_new,    gb_mesh_edge_lface(edge_org));

        // two faces are disjoint? 
        if (!joining_faces)
        {
            // save the old face first, edge_org.lface may have been modified after making new face
            gb_mesh_face_ref_t face_old = gb_mesh_edge_lface(edge_org);

            /* make new face at edge_new.lface
             * and update lface for all edges leaving the left orbit of the edge_new
             */
            gb_mesh_face_ref_t face_new = gb_mesh_make_face_at_orbit(impl, edge_new);
            tb_assert_and_check_break(face_new);

            // post the split event, split(edge_org.lface) => (edge_org.lface, face_new)
            gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_SPLIT, face_old, face_new);
        }
        else
        {
            // init the edge_new.lface
            gb_mesh_edge_lface_set(edge_new, gb_mesh_edge_lface(edge_org));
        }

        // ok
        ok = tb_true;

    } while (0);

    // check
    tb_assert_abort(ok);

    // failed
    if (!ok)
    {
        // kill the new edge
        if (edge_new) gb_mesh_kill_edge(impl, edge_new);
        edge_new = tb_null;
    }

    // ok?
    return edge_new;
}
tb_void_t gb_mesh_edge_delete(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_del)
{
    // check
    gb_mesh_impl_t* impl = (gb_mesh_impl_t*)mesh;
    tb_assert_and_check_return(impl && edge_del);

    // check edge
    gb_mesh_check_edge(edge_del);

    // done
    tb_bool_t ok = tb_false;
    tb_bool_t joining_faces = tb_false;
    do
    {
        // two faces are disjoint? 
        if (gb_mesh_edge_lface(edge_del) != gb_mesh_edge_rface(edge_del))
        {
            // joins the two faces
            joining_faces = tb_true;

            // post the merge event, merge(edge_del.lface, edge_del.rface) => edge_del.rface
            gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_MERGE, gb_mesh_edge_lface(edge_del), gb_mesh_edge_rface(edge_del));

            // remove the edge_del.lface first
            gb_mesh_kill_face_at_orbit(impl, gb_mesh_edge_lface(edge_del), gb_mesh_edge_rface(edge_del));
        }

        /* before:
         *
         * edge_del.lface != edge_del.rface:
         *
         *         face_del
         *
         *         edge_org
         *  ---------------------->
         * |                       |
         * |                       |      
         * |         face          | edge_del      face_del
         * |                       |
         * |                      \|/
         *  <----------------------
         *         edge_dst                                  
         *
         *         face_del
         *
         *
         * edge_del.lface == edge_del.rface:
         *
         * 
         *          edge_dst
         *  <-----------------------. 
         * |            edge_del .  |
         * |     ------------->.    |
         * |    |   edge_org   |    |
         * |    |              |    |
         * |    |              |    |
         * |     <-------------     |
         * |        face_org        |
         *  ----------------------->  
         *
         * edge_del.onext == edge_del:
         *
         *  ----------------------> <---------------- org
         * |                       |     edge_del
         * |                       |
         *  <----------------------
         *                                            
         */
        if (gb_mesh_edge_onext(edge_del) == edge_del)
        {
            /* remove the edge_del.org
             *
             * after:
             *
             *  ----------------------> <---------------- null
             * |                       |     edge_del
             * |                       |
             *  <----------------------
             */
            gb_mesh_kill_vertex_at_orbit(impl, gb_mesh_edge_org(edge_del), tb_null);
        } 
        else 
        {
            // update the reference edge, the old reference edge may have been invalid
            gb_mesh_face_edge_set  (gb_mesh_edge_rface(edge_del),   gb_mesh_edge_oprev(edge_del));
            gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_del),     gb_mesh_edge_onext(edge_del));

            /* disjoining edges at the edge_del.org
             *
             * after:
             *
             * edge_del.lface != edge_del.rface:
             *
             *
             *         edge_org
             *  ---------------------->
             * |                       
             * |                           
             * |         face             
             * |                       
             * |                      
             *  <---------------------- <-----------------
             *         edge_dst              edge_del       
             *
             *
             * edge_del.lface == edge_del.rface:
             *
             * 
             *          edge_dst
             *  <-----------------------.
             * |                     .  |   
             * |                   .    |        ------------->
             * |          edge_del      |       |   edge_org   |
             * |                        |       |              |
             * |        face_new        |       |              |
             * |                        |        <-------------
             * |                        |           face_org
             *  ----------------------->  
             *
             */
            gb_mesh_splice_edge(edge_del, gb_mesh_edge_oprev(edge_del));

            // two faces are disjoint? 
            if (!joining_faces) 
            {
                // save the old face first, edge_del.lface may have been modified after making new face
                gb_mesh_face_ref_t face_old = gb_mesh_edge_lface(edge_del);

                /* make new face at edge_del.lface
                 * and update lface for all edges leaving the left orbit of the edge_del
                 */
                gb_mesh_face_ref_t face_new = gb_mesh_make_face_at_orbit(impl, edge_del);
                tb_assert_and_check_break(face_new);

                // post the split event, split(face_old) => (face_old, face_new)
                gb_mesh_post_event(impl, GB_MESH_EVENT_FACE_SPLIT, face_old, face_new);
            }
        }

        // the sym edge
        gb_mesh_edge_ref_t edge_sym = gb_mesh_edge_sym(edge_del);
        tb_assert_and_check_break(edge_sym);

        // the deleted edge is isolated now?
        if (gb_mesh_edge_onext(edge_sym) == edge_sym) 
        {
            /* remove the edge_del directly
             *
             * before:
             *
             *  ---------------------->           <---------------- null
             * |                       |               edge_del
             * |                       |
             *  <----------------------
             *             
             * after:
             *
             *  ---------------------->    
             * |                       |        
             * |                       |
             *  <----------------------
             */
            gb_mesh_kill_vertex_at_orbit(impl, gb_mesh_edge_org(edge_sym), tb_null);
            gb_mesh_kill_face_at_orbit(impl, gb_mesh_edge_lface(edge_sym), tb_null);
        }
        else 
        {
            // update the reference edge, the old reference edge may have been invalid
            gb_mesh_face_edge_set  (gb_mesh_edge_lface(edge_del),   gb_mesh_edge_oprev(edge_sym));
            gb_mesh_vertex_edge_set(gb_mesh_edge_org(edge_sym),     gb_mesh_edge_onext(edge_sym));

            /* disjoining edges at the edge_del.dst
             *
             * after:
             *
             * edge_del.lface != edge_del.rface:
             *
             *
             *         edge_org
             *  ---------------------->
             * |                       
             * |                           
             * |         face             
             * |                       
             * |                      
             *  <----------------------        <-----------------
             *         edge_dst                     edge_del       
             *
             *
             * edge_del.lface == edge_del.rface:
             *
             * 
             *          edge_dst
             *  <----------------------- 
             * |                        |   
             * |       ---------->      |        ------------->
             * |        edge_del        |       |   edge_org   |
             * |                        |       |              |
             * |        face_new        |       |              |
             * |                        |        <-------------
             * |                        |           face_org
             *  ----------------------->  
             *
             */
            gb_mesh_splice_edge(edge_sym, gb_mesh_edge_oprev(edge_sym));
        }

        // kill this edge
        gb_mesh_kill_edge(impl, edge_del);

        // ok
        ok = tb_true;

    } while (0);

    // check
    tb_assert_abort(ok);
}
#ifdef __gb_debug__
tb_void_t gb_mesh_dump(gb_mesh_ref_t mesh)
{
    // trace
    tb_trace_i("");
    tb_trace_i("edges:");

    // dump edges
    tb_for_all_if (gb_mesh_edge_ref_t, edge, gb_mesh_edge_itor(mesh), edge)
    {
        // trace
        tb_trace_i("    %{mesh_edge}", edge);
    }

    // trace
    tb_trace_i("faces:");

    // dump faces
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(mesh), face)
    {
        // trace
        tb_trace_i("    face: %{mesh_face}", face);

        // dump face.edges
        gb_mesh_edge_ref_t head = gb_mesh_face_edge(face);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // trace
            tb_trace_i("        %{mesh_edge}", edge);

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
        tb_trace_i("    vertex: %{mesh_vertex}", vertex);

        // dump vertex.edges
        gb_mesh_edge_ref_t head = gb_mesh_vertex_edge(vertex);
        gb_mesh_edge_ref_t edge = head;
        do
        {
            // trace
            tb_trace_i("        %{mesh_edge}", edge);

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
