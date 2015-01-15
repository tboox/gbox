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
 * @file        mesh_edge.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_IMPL_MESH_EDGE_H
#define GB_UTILS_IMPL_MESH_EDGE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh edge list ref type
typedef struct{}*           gb_mesh_edge_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interedges
 */

/* init the mesh edge list 
 *
 * @param func              the item func
 *
 * @return                  the edge list
 */
gb_mesh_edge_list_ref_t     gb_mesh_edge_list_init(tb_item_func_t func);

/* exit the mesh edge list
 *
 * @param list              the edge list
 */
tb_void_t                   gb_mesh_edge_list_exit(gb_mesh_edge_list_ref_t list);

/* clear the mesh edge list
 *
 * @param list              the edge list
 */
tb_void_t                   gb_mesh_edge_list_clear(gb_mesh_edge_list_ref_t list);

/*! the mesh edge iterator
 *
 * @param list              the list
 *
 * @return                  the mesh edge iterator
 */
tb_iterator_ref_t           gb_mesh_edge_list_itor(gb_mesh_edge_list_ref_t list);

/*! the mesh edge count
 *
 * @param list              the list
 *
 * @return                  the item count
 */
tb_size_t                   gb_mesh_edge_list_size(gb_mesh_edge_list_ref_t list);

/*! the mesh edge maximum count
 *
 * @param list              the list
 *
 * @return                  the item max count
 */
tb_size_t                   gb_mesh_edge_list_maxn(gb_mesh_edge_list_ref_t list);

/*! make a unconnected edge
 *
 * <pre>
 *
 *  O -----> D
 *          
 * </pre>
 *
 * @param list              the list
 *
 * @return                  the new edge
 */
gb_mesh_edge_ref_t          gb_mesh_edge_list_make(gb_mesh_edge_list_ref_t list);

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
 *         |       |
 *         |       |
 *         O/D <---
 *
 * counter-clockwise:
 *
 *          -------
 *         |       |
 *         |       |
 *        \|/      |
 *         D/O ----
 *          
 * </pre>
 *
 * @param list              the list
 *
 * @return                  the new edge
 */
gb_mesh_edge_ref_t          gb_mesh_edge_list_make_loop(gb_mesh_edge_list_ref_t list, tb_bool_t is_ccw);

/*! make the edge info string
 *
 * @param list              the list
 * @param edge              the edge
 * @param data              the string data
 * @param maxn              the string maxn
 */
tb_char_t const*            gb_mesh_edge_list_cstr(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge, tb_char_t* data, tb_size_t maxn);

/*! kill the edge, release the storage
 *
 * @param list              the list
 * @param edge              the edge
 */
tb_void_t                   gb_mesh_edge_list_kill(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge);

/*! the user data 
 *
 * @param list              the list
 * @param edge              the edge
 *
 * @return                  the user data
 */
tb_cpointer_t               gb_mesh_edge_list_data(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge);

/*! set the user private data 
 *
 * @param list              the list
 * @param edge              the edge
 * @param data              the user data
 */
tb_void_t                   gb_mesh_edge_list_data_set(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge, tb_cpointer_t data);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


