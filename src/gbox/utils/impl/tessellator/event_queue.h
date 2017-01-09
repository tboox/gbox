/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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


