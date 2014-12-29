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
#define gb_mesh_face_edge(face)                 ((face)->edge)

/// set the face edge
#define gb_mesh_face_edge_set(face, val)        ((face)->edge = (val))

/// get the face private data for user
#define gb_mesh_face_priv(face)                 ((gb_mesh_face_ref_t)(face) + 1)

/// get the vertex edge
#define gb_mesh_vertex_edge(vertex)             ((vertex)->edge)

/// set the vertex edge
#define gb_mesh_vertex_edge_set(vertex, val)    ((vertex)->edge = (val))

/// get the vertex private data for user
#define gb_mesh_vertex_priv(face)               ((gb_mesh_vertex_ref_t)(vertex) + 1)

/// get the edge sym
#define gb_mesh_edge_sym(edge)                  ((edge)->sym)

/// set the edge sym
#define gb_mesh_edge_sym_set(edge, val)         ((edge)->sym = (val))

/// get the edge org
#define gb_mesh_edge_org(edge)                  ((edge)->org)

/// set the edge org
#define gb_mesh_edge_org_set(edge, val)         ((edge)->org = (val))

/// get the edge dst
#define gb_mesh_edge_dst(edge)                  ((edge)->sym->org)

/// set the edge dst
#define gb_mesh_edge_dst_set(edge, val)         ((edge)->sym->org = (val))

/// get the edge lface
#define gb_mesh_edge_lface(edge)                ((edge)->lface)

/// set the edge lface
#define gb_mesh_edge_lface_set(edge, val)       ((edge)->lface = (val))

/// get the edge rface
#define gb_mesh_edge_rface(edge)                ((edge)->sym->lface)

/// set the edge lface
#define gb_mesh_edge_rface_set(edge, val)       ((edge)->sym->lface = (val))

/// get the edge onext
#define gb_mesh_edge_onext(edge)                ((edge)->onext)

/// set the edge onext
#define gb_mesh_edge_onext_set(edge, val)       ((edge)->onext = (val))

/// get the edge oprev
#define gb_mesh_edge_oprev(edge)                ((edge)->sym->lnext)

/// set the edge oprev
#define gb_mesh_edge_oprev_set(edge, val)       ((edge)->sym->lnext = (val))

/// get the edge lnext
#define gb_mesh_edge_lnext(edge)                ((edge)->lnext)

/// set the edge lnext
#define gb_mesh_edge_lnext_set(edge, val)       ((edge)->lnext = (val))

/// get the edge lprev
#define gb_mesh_edge_lprev(edge)                ((edge)->onext->sym)

/// set the edge lprev
#define gb_mesh_edge_lprev_set(edge, val)       ((edge)->onext->sym = (val))

/// get the edge rnext
#define gb_mesh_edge_rnext(edge)                (gb_mesh_edge_oprev(edge)->sym)

// set the edge rnext
#define gb_mesh_edge_rnext_set(edge, val)       (gb_mesh_edge_oprev(edge)->sym = (val))

/// get the edge rprev
#define gb_mesh_edge_rprev(edge)                ((edge)->sym->onext)

/// set the edge rprev
#define gb_mesh_edge_rprev_set(edge, val)       ((edge)->sym->onext = (val))

/// get the edge dnext
#define gb_mesh_edge_dnext(edge)                (gb_mesh_edge_rprev(edge)->sym)

/// set the edge dnext
#define gb_mesh_edge_dnext_set(edge, val)       (gb_mesh_edge_rprev(edge)->sym = (val))

/// get the edge dprev
#define gb_mesh_edge_dprev(edge)                ((edge)->lnext->sym)

/// set the edge rprev
#define gb_mesh_edge_dprev_set(edge, val)       ((edge)->lnext->sym = (val))

/// get the edge private data for user
#define gb_mesh_edge_priv(edge)                 ((gb_mesh_edge_ref_t)(edge) + 1)

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
    tb_list_entry_t                 entry;

    /// an arbitrary edge of the vertex
    struct __gb_mesh_edge_t*        edge;

}gb_mesh_vertex_t, *gb_mesh_vertex_ref_t;

/// the mesh face type
typedef struct __gb_mesh_face_t
{
    /// the list entry
    tb_list_entry_t                 entry;

    /// an arbitrary edge of the face
    struct __gb_mesh_edge_t*        edge;

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
     * prev: edge->sym->next
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

/*! the face iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the mesh face iterator
 */
tb_iterator_ref_t               gb_mesh_face_itor(gb_mesh_ref_t mesh);

/*! the vertex iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the mesh vertex iterator
 */
tb_iterator_ref_t               gb_mesh_vertex_itor(gb_mesh_ref_t mesh);

/*! make the root edge
 *
 * clear the mesh first and 
 * create a looping edge that connects to itself at a single vertex
 *
 * <pre>
 *          -------
 *         |       |
 *         | rface |
 *         |       |
 *         O/D <---
 *
 *           lface
 *          
 * </pre>
 *
 * @param mesh                  the mesh
 *
 * @return                      the new root edge
 */
gb_mesh_edge_ref_t              gb_mesh_make_root_edge(gb_mesh_ref_t mesh);

/*! make the face edge
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
 *
 * @return                      the new edge
 */
gb_mesh_edge_ref_t              gb_mesh_make_face_edge(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, gb_mesh_vertex_ref_t org, gb_mesh_vertex_ref_t dst);

/*! kill the face edge
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
tb_void_t                       gb_mesh_kill_face_edge(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/*! make the vertex edge
 *
 * <pre>
 * 
 *                                 .   .   .
 *   .    .    .                     . . .
 *     .  .  .                         Vnew(dst)
 *       . .                           .
 * lface  V  rface      =>  lface      .        rface
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
 * @param vertex                the origin vertex of the new edge, must be non-null
 * @param lface                 the left face of the new edge, must be non-null
 * @param rface                 the right face of the new edge, must be non-null
 *
 * @return                      the new edge
 */
gb_mesh_edge_ref_t              gb_mesh_make_vertex_edge(gb_mesh_ref_t mesh, gb_mesh_vertex_ref_t vertex, gb_mesh_face_ref_t lface, gb_mesh_face_ref_t rface);

/*! kill the vertex edge
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
tb_void_t                       gb_mesh_kill_vertex_edge(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


