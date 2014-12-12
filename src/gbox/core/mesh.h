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
 * @ingroup     core
 *
 */
#ifndef GB_CORE_MESH_H
#define GB_CORE_MESH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

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
 */
typedef struct __gb_mesh_edge_t
{
    /*! the next edge for the doubly-linked list
     *
     * next: edge->next
     * prev: edge->sym->next
     *
     * head                                   null
     *    |                                    |
     * edge1: ..............e.............. : edge1->sym
     *    |                                   /|\
     *    | next                          next |
     *   \|/                                   |
     * edge2: ..............e.............. : edge2->sym       (prev)
     *    |                                   /|\
     *    | next                          next |
     *   \|/                                   |
     * edge3: ..............e.............. : edge3->sym
     *    |                                    |
     *  null                                  head_sym
     *
     *  the doubly-linked list = the single-linked list (x2)
     */
    tb_single_list_entry_t      entry;

    /*! the next edge ccw around the origin
     *
     * onext: edge->onext
     * lprev: edge->onext->sym
     * rprev: edge->sym->onext
     * dnext: edge->rprev->sym == edge->sym->onext->sym
     */
    struct __gb_mesh_edge_t*    onext;

    /*! the next edge ccw around the left face
     *
     * lnext: edge->lnext
     * dprev: edge->lnext->sym
     * oprev: edge->sym->lnext
     * rnext: edge->oprev->sym == edge->sym->lnext->sym
     */
    struct __gb_mesh_edge_t*    lnext;

    /// the same edge, opposite direction
    struct __gb_mesh_edge_t*    sym;

    /*! the origin vertex
     *
     * org: edge->org
     * dst: edge->sym->org
     */
    gb_mesh_vertex_ref_t        org;

    /*! the left face
     *
     * lface: edge->lface
     * rface: edge->sym->lface
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
 * @param list                  the mesh
 */
tb_void_t                       gb_mesh_exit(gb_mesh_ref_t mesh);

/*! clear the mesh 
 *
 * @param list                  the mesh
 */
tb_void_t                       gb_mesh_clear(gb_mesh_ref_t mesh);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


