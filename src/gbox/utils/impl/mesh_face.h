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
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_IMPL_MESH_FACE_H
#define GB_UTILS_IMPL_MESH_FACE_H

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

// the mesh face list type
typedef struct{}*           gb_mesh_face_list_ref_t;

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

/*! the mesh face iterator
 *
 * @param list              the list
 *
 * @return                  the mesh face iterator
 */
tb_iterator_ref_t           gb_mesh_face_list_itor(gb_mesh_face_list_ref_t list);

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

/*! make a bare face without edges
 *
 * @param list              the list
 *
 * @return                  the new face
 */
gb_mesh_face_ref_t          gb_mesh_face_list_make(gb_mesh_face_list_ref_t list);

/*! kill the face, release the storage
 *
 * @param list              the list
 * @param face              the face
 */
tb_void_t                   gb_mesh_face_list_kill(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


