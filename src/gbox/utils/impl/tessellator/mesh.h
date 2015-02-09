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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        mesh.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_MESH_H
#define GB_UTILS_IMPL_TESSELLATOR_MESH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make mesh from the given polygon
 *
 * @param impl      the tessellator impl
 * @param polygon   the polygon
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_tessellator_make_mesh(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon);

#endif


