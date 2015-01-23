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
 * @file        mesh.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_MESH_H
#define GB_UTILS_MESH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// get the face edge
#define gb_mesh_face_edge(face)                 (tb_assert(face), (face)->edge)

/// get the vertex edge
#define gb_mesh_vertex_edge(vertex)             (tb_assert(vertex), (vertex)->edge)

/// get the edge sym
#define gb_mesh_edge_sym(edge)                  (tb_assert(edge), (edge)->sym)

/// get the edge org
#define gb_mesh_edge_org(edge)                  (tb_assert(edge), (edge)->org)

/// get the edge dst
#define gb_mesh_edge_dst(edge)                  (tb_assert((edge) && (edge)->sym), (edge)->sym->org)

/// get the edge lface
#define gb_mesh_edge_lface(edge)                (tb_assert(edge), (edge)->lface)

/// get the edge rface
#define gb_mesh_edge_rface(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->lface)

/// get the edge onext
#define gb_mesh_edge_onext(edge)                (tb_assert(edge), (edge)->onext)

/// get the edge oprev
#define gb_mesh_edge_oprev(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->lnext)

/// get the edge lnext
#define gb_mesh_edge_lnext(edge)                (tb_assert(edge), (edge)->lnext)

/// get the edge lprev
#define gb_mesh_edge_lprev(edge)                (tb_assert((edge) && (edge)->onext), (edge)->onext->sym)

/// get the edge rnext
#define gb_mesh_edge_rnext(edge)                (tb_assert(gb_mesh_edge_oprev(edge)), gb_mesh_edge_oprev(edge)->sym)

/// get the edge rprev
#define gb_mesh_edge_rprev(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->onext)

/// get the edge dnext
#define gb_mesh_edge_dnext(edge)                (tb_assert(gb_mesh_edge_rprev(edge)), gb_mesh_edge_rprev(edge)->sym)

/// get the edge dprev
#define gb_mesh_edge_dprev(edge)                (tb_assert((edge) && (edge)->lnext), (edge)->lnext->sym)

/// the edge is isolated?
#define gb_mesh_edge_is_isolated(edge)          (tb_assert((edge) && (edge->sym)), (edge)->onext == (edge) && (edge)->sym->onext == (edge)->sym && (edge)->lnext == (edge->sym) && (edge)->sym->lnext == (edge))

/// the edge is isolated loop?
#define gb_mesh_edge_is_isolated_loop(edge)     (tb_assert((edge) && (edge->sym)), (edge)->onext == (edge)->sym && (edge)->sym->onext == (edge) && (edge)->lnext == (edge) && (edge)->sym->lnext == (edge)->sym)

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// declaration
struct __gb_mesh_edge_t;

/// the mesh vertex type
typedef struct __gb_mesh_vertex_t
{
    /// the list entry
    tb_list_entry_t             entry;

    /// an arbitrary edge of the vertex
    struct __gb_mesh_edge_t*    edge;

#ifdef __gb_debug__
    /// the id
    tb_size_t                   id;
#endif

}gb_mesh_vertex_t, *gb_mesh_vertex_ref_t;

/// the mesh face type
typedef struct __gb_mesh_face_t
{
    /// the list entry
    tb_list_entry_t             entry;

    /// an arbitrary edge of the face
    struct __gb_mesh_edge_t*    edge;

#ifdef __gb_debug__
    /// the id
    tb_size_t                   id;
#endif

}gb_mesh_face_t, *gb_mesh_face_ref_t;

/*! the mesh edge type
 *
 * using the half-edge data structure
 *
 * <pre>
 *              .                                            .
 *              . edge->dst                                  . edge->sym->org
 * .............D.............                 ..............O.............
 *              .                                            .
 *              .                                            .      edge->sym->lface
 *              .       edge->rface                          . 
 *              .                                            . edge->sym
 * edge->lface /.\                                          \./
 *              . edge                     edge->sym->rface  . 
 *              .                                            .
 *              .                                            .
 * .............O.............                 ..............D.............
 *              . edge->org                                  . edge->sym->dst
 *              .                                            .
 *
 *
 *
 *
 *              .                                            .
 * edge->lnext  . edge->dnext                  edge->dprev   . edge->rprev
 * ......<......D.....<.......                 ......>.......D......>......
 *              .                                            .
 *              .                                            .     
 *              .                                            . 
 *              .                                            . 
 *             /.\                                          /.\
 *              . edge                                       . edge
 *              .                                            .
 *              .                                            .
 * ......<......O.....<.......                 ......>.......O......>......
 * edge->onext  . edge->rnext                  edge->lprev   . edge->oprev
 *              .                                            .
 *        
 *          next: ccw                                    prev: cw
 *
 * </pre>
 */
typedef struct __gb_mesh_edge_t
{
    /*! the next edge for the doubly-linked list
     *
     * <pre>
     * next: edge->next
     * prev: edge->sym->next->sym
     *
     *  head: ..............e.............. : head_sym
     *    |                                     /|\
     *   \|/                                     |
     * edge1: ..............e.............. : edge1->sym
     *    |                                     /|\
     *    | next                            next |
     *   \|/                                     |
     * edge2: ..............e.............. : edge2->sym       (prev)
     *    |                                     /|\
     *    | next                            next |
     *   \|/                                     |
     * edge3: ..............e.............. : edge3->sym
     *    |                                     /|\
     *   \|/                                     |
     *  head: ..............e.............. : head_sym
     *
     * </pre>
     *
     */
    struct __gb_mesh_edge_t*    next;

    /*! the next edge ccw around the origin
     *
     * <pre>
     * onext: edge->onext
     * lprev: edge->onext->sym
     * rprev: edge->sym->onext
     * dnext: edge->rprev->sym == edge->sym->onext->sym
     * </pre>
     */
    struct __gb_mesh_edge_t*    onext;

    /*! the next edge ccw around the left face
     *
     * <pre>
     * lnext: edge->lnext
     * dprev: edge->lnext->sym
     * oprev: edge->sym->lnext
     * rnext: edge->oprev->sym == edge->sym->lnext->sym
     * </pre>
     */
    struct __gb_mesh_edge_t*    lnext;

    /// the same edge, opposite direction
    struct __gb_mesh_edge_t*    sym;

    /*! the origin vertex
     *
     * <pre>
     * org: edge->org
     * dst: edge->sym->org
     * </pre>
     */
    gb_mesh_vertex_ref_t        org;

    /*! the left face
     *
     * <pre>
     * lface: edge->lface
     * rface: edge->sym->lface
     * </pre>
     */
    gb_mesh_face_ref_t          lface;

#ifdef __gb_debug__
    /// the id
    tb_size_t                   id;
#endif

}gb_mesh_edge_t, *gb_mesh_edge_ref_t;

/*! the mesh ref type 
 *
 * using the doubly connected edge list (DCEL) 
 * like the quad-edge data structure but no edge->rot and edge->invrot
 */
typedef struct{}*               gb_mesh_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init the mesh 
 *
 * @param edge_func             the edge func
 * @param face_func             the face func
 * @param vertex_func           the vertex func
 *
 * @return                      the mesh
 */
gb_mesh_ref_t                   gb_mesh_init(tb_item_func_t edge_func, tb_item_func_t face_func, tb_item_func_t vertex_func);

/*! exit the mesh 
 *
 * @param mesh                  the mesh
 */
tb_void_t                       gb_mesh_exit(gb_mesh_ref_t mesh);

/*! clear the mesh 
 *
 * @param mesh                  the mesh
 */
tb_void_t                       gb_mesh_clear(gb_mesh_ref_t mesh);

/*! is empty? 
 *
 * @param mesh                  the mesh
 *
 * @return                      tb_true or tb_false
 */
tb_bool_t                       gb_mesh_is_empty(gb_mesh_ref_t mesh);

/*! the face iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the face iterator
 */
tb_iterator_ref_t               gb_mesh_face_itor(gb_mesh_ref_t mesh);

/*! the face string
 *
 * @param mesh                  the mesh
 * @param face                  the face
 * @param data                  the string data
 * @param maxn                  the string maxn
 */
tb_char_t const*                gb_mesh_face_cstr(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, tb_char_t* data, tb_size_t maxn);

/*! the face user data
 *
 * @param mesh                  the mesh
 * @param face                  the face
 *
 * @return                      the face user data
 */
tb_cpointer_t                   gb_mesh_face_data(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face);

/*! set the face user data
 *
 * @param mesh                  the mesh
 * @param face                  the face
 * @param data                  the face user data
 */
tb_void_t                       gb_mesh_face_data_set(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, tb_cpointer_t data);

/*! the vertex iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the mesh vertex iterator
 */
tb_iterator_ref_t               gb_mesh_vertex_itor(gb_mesh_ref_t mesh);

/*! the vertex string
 *
 * @param mesh                  the mesh
 * @param vertex                the vertex
 * @param data                  the string data
 * @param maxn                  the string maxn
 */
tb_char_t const*                gb_mesh_vertex_cstr(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, tb_char_t* data, tb_size_t maxn);

/*! the vertex user data
 *
 * @param mesh                  the mesh
 * @param vertex                the vertex
 *
 * @return                      the vertex user data
 */
tb_cpointer_t                   gb_mesh_vertex_data(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex);

/*! set the vertex user data
 *
 * @param mesh                  the mesh
 * @param vertex                the vertex
 * @param data                  the vertex user data
 */
tb_void_t                       gb_mesh_vertex_data_set(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, tb_cpointer_t data);

/*! the edge iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the edge iterator
 */
tb_iterator_ref_t               gb_mesh_edge_itor(gb_mesh_ref_t mesh);

/*! the edge string
 *
 * @param mesh                  the mesh
 * @param edge                  the edge
 * @param data                  the string data
 * @param maxn                  the string maxn
 */
tb_char_t const*                gb_mesh_edge_cstr(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge, tb_char_t* data, tb_size_t maxn);

/*! the edge user data
 *
 * @param mesh                  the mesh
 * @param edge                  the edge
 *
 * @return                      the edge user data
 */
tb_cpointer_t                   gb_mesh_edge_data(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/*! set the edge user data
 *
 * @param mesh                  the mesh
 * @param edge                  the edge
 * @param data                  the edge user data
 */
tb_void_t                       gb_mesh_edge_data_set(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge, tb_cpointer_t data);

/*! make a unconnected edge
 *
 * <pre>
 *
 *           lface
 *
 *         O -----> D
 *
 *           lface
 *          
 * </pre>
 *
 * @param mesh                  the mesh
 *
 * @return                      the new edge
 */
gb_mesh_edge_ref_t              gb_mesh_edge_make(gb_mesh_ref_t mesh);

/*! make a self-loop edge
 *
 * create a looping edge that connects to itself at a single vertex
 *
 * <pre>
 *
 * clockwise:
 *
 *          -------
 *         |       |
 *         | rface |
 *         |       |
 *         O/D <---
 *
 *           lface
 *
 *
 * counter-clockwise:
 *
 *          -------
 *         |       |
 *         | lface |
 *        \|/      |
 *         D/O ----
 *
 *           rface
 *          
 * </pre>
 *
 * @param mesh                  the mesh
 * @param is_ccw                is counter-clockwise?
 *
 * @return                      the new edge
 */
gb_mesh_edge_ref_t              gb_mesh_edge_make_loop(gb_mesh_ref_t mesh, tb_bool_t is_ccw);

/*! make the edge at face
 *
 * <pre>
 *            . dst                        . dst
 *      .       .                     .    .  .
 * .              .              .         .    .
 * .                .            .        /.\     .
 * .       face       .          . face    .        .
 * .                    .    =>  .         . fnew     .
 *    .             .               . edge .      .
 *       .      .                      .   .   .
 *          . org                          . org
 * </pre>
 *
 * @param mesh                  the mesh
 * @param face                  the face to divide to make the new edge, must be non-null
 * @param org                   the origin vertex of the new edge, must be non-null
 * @param dst                   the destinate vertex of the new edge, must be non-null
 * @param pedge                 the new edge pointer, optional
 *
 * @return                      the new face
 */
gb_mesh_face_ref_t              gb_mesh_edge_make_at_face(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, gb_mesh_vertex_ref_t org, gb_mesh_vertex_ref_t dst, gb_mesh_edge_ref_t* pedge);

/*! kill the edge at face
 *
 * <pre>
 *            . dst                        . dst
 *      .       .                     .    .  .
 * .              .              .         .    .
 * .                .            .        /.\     .
 * .       face       .          . face    .        .
 * .                    .    <=  .         . fnew     .
 *    .             .               . edge .      .
 *       .      .                      .   .   .
 *          . org                          . org
 * </pre>
 *
 * @param mesh                  the mesh
 * @param edge                  the face edge
 */
tb_void_t                       gb_mesh_edge_kill_at_face(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/*! make the edge at vertex
 *
 * <pre>
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
 * </pre>
 *
 * @param mesh                  the mesh
 * @param vertex                the origin vertex of the new edge, must be non-null
 * @param lface                 the left face of the new edge, must be non-null
 * @param rface                 the right face of the new edge, must be non-null
 * @param pedge                 the new edge pointer, optional
 *
 * @return                      the new vertex
 */
gb_mesh_vertex_ref_t            gb_mesh_edge_make_at_vertex(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, gb_mesh_face_ref_t lface, gb_mesh_face_ref_t rface, gb_mesh_edge_ref_t* pedge);

/*! kill the edge at vertex
 *
 * <pre>
 * 
 *                                 .   .   .
 *   .    .    .                     . . .
 *     .  .  .                         Vnew(dst)
 *       . .                           .
 * lface  V  rface      <=  lface      .        rface
 *       . .                           .
 *     .     .                        /.\ edge
 *                                     . 
 *                                     .
 *                                     V(org)
 *                                    . .
 *                                  .     . 
 * </pre>
 *
 * @param mesh                  the mesh
 * @param edge                  the vertex edge
 */
tb_void_t                       gb_mesh_edge_kill_at_vertex(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/*! splice edge
 *
 * refer to the paper of Guibas and Stolfi, simplified since we don't use flips (p98) 
 *
 * <pre>
 *
 *          .                                          .
 * .        .        .                        .        .        .
 *   .      .      .                            .      .      .
 *     .    .    .                                .    .    .
 *    o  .  .  .                                  o .  .  .  
 *         ...                                        ...
 *          . o.org             |          o.lface     . o.org      d.lface
 *                              |                    .   .
 *        o.lface               |                  .       .
 *                              |                .           .
 *          . d.org            \ /             .               . d
 *        .   .                                                  .
 *      .       .                                                  .
 *    .           . d                                                .
 *                  .
 *                    .
 *                        
 *                         <= splice =>
 *
 * the d.org and d.lface will be inserted or removed
 *          
 * </pre>
 *
 * @param mesh                  the mesh
 * @param edge_org              the original edge
 * @param edge_dst              the destinate edge
 *
 * @return                      tb_true or tb_false
 */
tb_bool_t                       gb_mesh_edge_splice(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org, gb_mesh_edge_ref_t edge_dst);

/*! connect edge
 *
 * add a new edge e connecting the destination of edge_org to the origin of edge_dst, 
 * in such a way that edge_org.lface = edge_new.lface = edge_dst.lface after the connection is complete. 
 * for added convenience it will also set the org and dst fields of the new edge to edge_org.dst and edge_dst.org.
 *
 * <pre>
 *
 * edge_org.lface == edge_dst.lface? disjoining face
 *
 *           face                                    face_new
 *
 *         edge_org                                  edge_org
 *  ---------------------->                   ---------------------->
 * |                                         |                       |
 * |                                         |                       |      
 * |         face             => connect =>  |         face          | edge_new      face_new
 * |                                         |                       |
 * |                                         |                      \|/
 *  <---------------------                    <----------------------
 *         edge_dst                                  edge_dst
 *
 *           face                                    face_new
 *
 *
 * edge_org.lface != edge_dst.lface? joining faces
 *
 * 
 *         edge_dst                face_org                             edge_dst
 *  <----------------------                                     <-----------------------. 
 * |                       |       edge_org                    |            edge_new .  |
 * |                       |    ------------->                 |     ------------->.    |
 * |                       |   |              |                |    |   edge_org   |    |
 * |       face_dst        |   |              | => connect =>  |    |              |    |
 * |                       |   |              |                |    |              |    |
 * |                       |    <-------------                 |     <-------------     |
 * |                       |                                   |        face_org        |
 *  ---------------------->                                     ----------------------->
 *
 * after: edge_new.lface == edge_org.lface == edge_dst.lface
 *
 * </pre>
 *
 * @param mesh                  the mesh
 * @param edge_org              the original edge
 * @param edge_dst              the destinate edge
 *
 * @return                      the new edge
 */
gb_mesh_edge_ref_t              gb_mesh_edge_connect(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge_org, gb_mesh_edge_ref_t edge_dst);

/*! delete edge
 *
 * The operation will disconnect the edge e from the rest of the structure 
 * (this may cause the rest of the structure to fall apart in two separate components). 
 *
 * In a sense, DeleteEdge is the inverse of ConnectEdge. 
 *
 * <pre>
 *
 * before: edge_del.lface == edge_org.lface == edge_dst.lface
 *
 * edge_del.lface != edge_del.rface? joining faces
 *
 *           face                                    face_del
 *
 *         edge_org                                  edge_org
 *  ---------------------->                   ---------------------->
 * |                                         |                       |
 * |                                         |                       |      
 * |         face             <= delete <=   |         face          | edge_del      face_del
 * |                                         |                       |
 * |                                         |                      \|/
 *  <---------------------                    <----------------------
 *         edge_dst                                  edge_dst
 *
 *           face                                    face_del
 *
 *
 * edge_del.lface == edge_del.rface? disjoining face
 *
 * 
 *         edge_dst                                                     edge_dst
 *  <----------------------                                     <-----------------------. 
 * |                       |       edge_org                    |            edge_del .  |
 * |                       |    ------------->                 |     ------------->.    |
 * |                       |   |              |                |    |   edge_org   |    |
 * |       face_new        |   |              | <= delete <=   |    |              |    |
 * |                       |   |              |                |    |              |    |
 * |                       |    <-------------                 |     <-------------     |
 * |                       |       face_org                    |        face_org        |
 *  ---------------------->                                     ----------------------->
 *
 * </pre>
 *
 * @param mesh                  the mesh
 * @param edge                  the edge
 *
 * @return                      tb_true or tb_false
 */
tb_bool_t                       gb_mesh_edge_delete(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

#ifdef __gb_debug__
/*! dump mesh

 * @param mesh                  the mesh
 */
tb_void_t                       gb_mesh_dump(gb_mesh_ref_t mesh);

/*! check mesh

 * @param mesh                  the mesh
 */
tb_void_t                       gb_mesh_check(gb_mesh_ref_t mesh);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


