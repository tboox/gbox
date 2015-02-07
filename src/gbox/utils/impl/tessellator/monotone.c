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
 * @file        monotone.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "monotone"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "monotone.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_done_monotone(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && impl->mesh && bounds);

    // TODO
    tb_trace_noimpl();

    // for testing single contour first
//    gb_tessellator_face_inside_set(gb_mesh_face_head(mesh), 1);

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif

    // ok
    return tb_true;
}


