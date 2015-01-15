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

/// set the face edge
#define gb_mesh_face_edge_set(face, val)        do { tb_assert(face); (face)->edge = (val); } while (0)

/// get the vertex edge
#define gb_mesh_vertex_edge(vertex)             (tb_assert(vertex), (vertex)->edge)

/// set the vertex edge
#define gb_mesh_vertex_edge_set(vertex, val)    do { tb_assert(vertex); (vertex)->edge = (val); } while (0)

/// get the edge sym
#define gb_mesh_edge_sym(edge)                  (tb_assert(edge), (edge)->sym)

/// set the edge sym
#define gb_mesh_edge_sym_set(edge, val)         do { tb_assert(edge); (edge)->sym = (val); } while (0)

/// get the edge org
#define gb_mesh_edge_org(edge)                  (tb_assert(edge), (edge)->org)

/// set the edge org
#define gb_mesh_edge_org_set(edge, val)         do { tb_assert(edge); (edge)->org = (val); gb_mesh_vertex_edge_set(val, edge); } while (0)

/// get the edge dst
#define gb_mesh_edge_dst(edge)                  (tb_assert((edge) && (edge)->sym), (edge)->sym->org)

/// set the edge dst
#define gb_mesh_edge_dst_set(edge, val)         do { tb_assert((edge) && (edge)->sym); (edge)->sym->org = (val); gb_mesh_vertex_edge_set(val, (edge)->sym); } while (0)

/// get the edge lface
#define gb_mesh_edge_lface(edge)                (tb_assert(edge), (edge)->lface)

/// set the edge lface
#define gb_mesh_edge_lface_set(edge, val)       do { tb_assert(edge); (edge)->lface = (val); gb_mesh_face_edge_set(val, edge); } while (0)

/// get the edge rface
#define gb_mesh_edge_rface(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->lface)

/// set the edge lface
#define gb_mesh_edge_rface_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->lface = (val); gb_mesh_face_edge_set(val, (edge)->sym); } while (0)

/// get the edge onext
#define gb_mesh_edge_onext(edge)                (tb_assert(edge), (edge)->onext)

/// set the edge onext
#define gb_mesh_edge_onext_set(edge, val)       do { tb_assert(edge); (edge)->onext = (val); } while (0)

/// get the edge oprev
#define gb_mesh_edge_oprev(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->lnext)

/// set the edge oprev
#define gb_mesh_edge_oprev_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->lnext = (val); } while (0)

/// get the edge lnext
#define gb_mesh_edge_lnext(edge)                (tb_assert(edge), (edge)->lnext)

/// set the edge lnext
#define gb_mesh_edge_lnext_set(edge, val)       do { tb_assert(edge); (edge)->lnext = (val); } while (0)

/// get the edge lprev
#define gb_mesh_edge_lprev(edge)                (tb_assert((edge) && (edge)->onext), (edge)->onext->sym)

/// set the edge lprev
#define gb_mesh_edge_lprev_set(edge, val)       do { tb_assert((edge) && (edge)->onext); (edge)->onext->sym = (val); } while (0)

/// get the edge rnext
#define gb_mesh_edge_rnext(edge)                (tb_assert(gb_mesh_edge_oprev(edge)), gb_mesh_edge_oprev(edge)->sym)

// set the edge rnext
#define gb_mesh_edge_rnext_set(edge, val)       do { tb_assert(gb_mesh_edge_oprev(edge)); gb_mesh_edge_oprev(edge)->sym = (val); } while (0)

/// get the edge rprev
#define gb_mesh_edge_rprev(edge)                (tb_assert((edge) && (edge)->sym), (edge)->sym->onext)

/// set the edge rprev
#define gb_mesh_edge_rprev_set(edge, val)       do { tb_assert((edge) && (edge)->sym); (edge)->sym->onext = (val); } while (0)

/// get the edge dnext
#define gb_mesh_edge_dnext(edge)                (tb_assert(gb_mesh_edge_rprev(edge)), gb_mesh_edge_rprev(edge)->sym)

/// set the edge dnext
#define gb_mesh_edge_dnext_set(edge, val)       do { tb_assert(gb_mesh_edge_rprev(edge)); gb_mesh_edge_rprev(edge)->sym = (val); } while (0)

/// get the edge dprev
#define gb_mesh_edge_dprev(edge)                (tb_assert((edge) && (edge)->lnext), (edge)->lnext->sym)

/// set the edge rprev
#define gb_mesh_edge_dprev_set(edge, val)       do { tb_assert((edge) && (edge)->lnext); (edge)->lnext->sym = (val); } while (0)

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

/*! the face iterator
 *
 * @param mesh                  the mesh
 *
 * @return                      the face iterator
 */
tb_iterator_ref_t               gb_mesh_face_itor(gb_mesh_ref_t mesh);

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
 * @param pedge                 the new edge pointer, optional
 *
 * @return                      the new face
 */
gb_mesh_face_ref_t              gb_mesh_edge_make_at_face(gb_mesh_ref_t mesh, gb_mesh_face_ref_t face, gb_mesh_vertex_ref_t org, gb_mesh_vertex_ref_t dst, gb_mesh_edge_ref_t* pedge);

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
tb_void_t                       gb_mesh_edge_kill_at_face(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

/*! make the vertex edge
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
tb_void_t                       gb_mesh_edge_kill_at_vertex(gb_mesh_ref_t mesh, gb_mesh_edge_ref_t edge);

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


