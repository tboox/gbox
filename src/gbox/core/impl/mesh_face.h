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
 * @file        mesh_face.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_MESH_FACE_H
#define GB_CORE_IMPL_MESH_FACE_H

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

// the mesh face type
typedef struct __gb_mesh_face_t
{
    // the list entry
    tb_list_entry_t                 entry;

    // an arbitrary edge of the face
    struct __gb_mesh_edge_t*        edge;

}gb_mesh_face_t, *gb_mesh_face_ref_t;

// the mesh face list type
typedef struct{}*                   gb_mesh_face_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the mesh face list 
 *
 * @param func              the item func
 *
 * @return                  the face list
 */
gb_mesh_face_list_ref_t     gb_mesh_face_list_init(tb_item_func_t func);

/* exit the mesh face list
 *
 * @param list              the face list
 */
tb_void_t                   gb_mesh_face_list_exit(gb_mesh_face_list_ref_t list);

/* clear the mesh face list
 *
 * @param list              the face list
 */
tb_void_t                   gb_mesh_face_list_clear(gb_mesh_face_list_ref_t list);

/*! the mesh face count
 *
 * @param list              the list
 *
 * @return                  the item count
 */
tb_size_t                   gb_mesh_face_list_size(gb_mesh_face_list_ref_t list);

/*! the mesh face maximum count
 *
 * @param list              the list
 *
 * @return                  the item max count
 */
tb_size_t                   gb_mesh_face_list_maxn(gb_mesh_face_list_ref_t list);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


