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
 * @file        vertex_list.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_IMPL_MESH_VERTEX_LIST_H
#define GB_UTILS_IMPL_MESH_VERTEX_LIST_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh vertex list type
typedef struct{}*           gb_mesh_vertex_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * intervertexs
 */

/* init the mesh vertex list 
 *
 * @param element           the element
 *
 * @returned                the vertex list
 */
gb_mesh_vertex_list_ref_t   gb_mesh_vertex_list_init(tb_element_t element);

/* exit the mesh vertex list
 *
 * @param list              the vertex list
 */
tb_void_t                   gb_mesh_vertex_list_exit(gb_mesh_vertex_list_ref_t list);

/* clear the mesh vertex list
 *
 * @param list              the vertex list
 */
tb_void_t                   gb_mesh_vertex_list_clear(gb_mesh_vertex_list_ref_t list);

/*! the mesh vertex iterator
 *
 * @param list              the list
 *
 * @return                  the mesh vertex iterator
 */
tb_iterator_ref_t           gb_mesh_vertex_list_itor(gb_mesh_vertex_list_ref_t list);

/*! the mesh vertex count
 *
 * @param list              the list
 *
 * @return                  the item count
 */
tb_size_t                   gb_mesh_vertex_list_size(gb_mesh_vertex_list_ref_t list);

/*! the mesh vertex maximum count
 *
 * @param list              the list
 *
 * @return                  the item max count
 */
tb_size_t                   gb_mesh_vertex_list_maxn(gb_mesh_vertex_list_ref_t list);

/*! make a bare vertex without edges
 *
 * @param list              the list
 *
 * @return                  the new vertex
 */
gb_mesh_vertex_ref_t        gb_mesh_vertex_list_make(gb_mesh_vertex_list_ref_t list);

#ifdef __gb_debug__
/*! make the vertex info string
 *
 * @param list              the list
 * @param vertex            the vertex
 * @param data              the string data
 * @param maxn              the string maxn
 *
 * @return                  the string size
 */
tb_long_t                   gb_mesh_vertex_list_cstr(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_char_t* data, tb_size_t maxn);
#endif

/*! kill the vertex, release the storage
 *
 * @param list              the list
 * @param vertex            the vertex
 */
tb_void_t                   gb_mesh_vertex_list_kill(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex);

/*! the user data 
 *
 * @param list              the list
 * @param vertex            the vertex
 *
 * @return                  the user data
 */
tb_cpointer_t               gb_mesh_vertex_list_data(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex);

/*! set the user private data 
 *
 * @param list              the list
 * @param vertex            the vertex
 * @param data              the user data
 */
tb_void_t                   gb_mesh_vertex_list_data_set(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_cpointer_t data);

/*! the list order
 *
 * @param list              the list
 *
 * @return                  the order
 */
tb_size_t                   gb_mesh_vertex_list_order(gb_mesh_vertex_list_ref_t list);

/*! set the list order
 *
 * @param list              the list
 * @param order             the order
 */
tb_void_t                   gb_mesh_vertex_list_order_set(gb_mesh_vertex_list_ref_t list, tb_size_t order);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


