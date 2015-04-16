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
 * @file        event_queue.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_EVENT_QUEUE_H
#define GB_UTILS_IMPL_TESSELLATOR_EVENT_QUEUE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make the vertex event queue and all events are sorted
 *   
 * @param impl      the tessellator impl
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_tessellator_event_queue_make(gb_tessellator_impl_t* impl);

/* insert the vertex event to queue
 *
 * @param impl      the tessellator impl
 * @param event     the vertex event
 */
tb_void_t           gb_tessellator_event_queue_insert(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event);

/* remove the vertex event from queue
 *
 * @param impl      the tessellator impl
 * @param event     the vertex event
 */
tb_void_t           gb_tessellator_event_queue_remove(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


