/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        edge_list.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_IMPL_MESH_EDGE_LIST_H
#define GB_UTILS_IMPL_MESH_EDGE_LIST_H

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

// the mesh edge list ref type
typedef struct{}*           gb_mesh_edge_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interedges
 */

/* init the mesh edge list 
 *
 * @param element           the element
 *
 * @return                  the edge list
 */
gb_mesh_edge_list_ref_t     gb_mesh_edge_list_init(tb_element_t element);

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

#ifdef __gb_debug__
/*! make the edge info string
 *
 * @param list              the list
 * @param edge              the edge
 * @param data              the string data
 * @param maxn              the string maxn
 *
 * @return                  the string size
 */
tb_long_t                   gb_mesh_edge_list_cstr(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge, tb_char_t* data, tb_size_t maxn);
#endif

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

/*! the list order
 *
 * @param list              the list
 *
 * @return                  the order
 */
tb_size_t                   gb_mesh_edge_list_order(gb_mesh_edge_list_ref_t list);

/*! set the list order
 *
 * @param list              the list
 * @param order             the order
 */
tb_void_t                   gb_mesh_edge_list_order_set(gb_mesh_edge_list_ref_t list, tb_size_t order);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


