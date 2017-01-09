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
 * @file        event_queue.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "event_queue"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "event_queue.h"
#include "geometry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_long_t gb_tessellator_event_queue_comp(tb_element_ref_t element, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
#if 0
    // lvertex <= rvertex ? -1 : 1
    return (!gb_tessellator_vertex_leq((gb_mesh_vertex_ref_t)ldata, (gb_mesh_vertex_ref_t)rdata) << 1) - 1;
#else
    // the left and right point
    gb_point_ref_t lpoint = gb_tessellator_vertex_point(ldata);
    gb_point_ref_t rpoint = gb_tessellator_vertex_point(rdata);
    tb_assert(lpoint && rpoint);

    // lpoint < rpoint?
    return (lpoint->y < rpoint->y)? -1 : ((lpoint->y > rpoint->y)? 1 : ((lpoint->x < rpoint->x)? -1 : (lpoint->x > rpoint->x)));
#endif
}
static tb_bool_t gb_tessellator_event_queue_find(tb_iterator_ref_t iterator, tb_cpointer_t item, tb_cpointer_t value)
{
    return item == value;
}
#ifdef __gb_debug__
static tb_char_t const* gb_tessellator_event_queue_cstr(tb_element_ref_t element, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_mesh_vertex_ref_t event = (gb_mesh_vertex_ref_t)data;
    tb_assert_and_check_return_val(event, tb_null);

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "%{mesh_vertex}", event);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_event_queue_make(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert(impl);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert(mesh);

    // init event queue
    if (!impl->event_queue) 
    {
        // make event element
        tb_element_t element = tb_element_ptr(tb_null, tb_null);

        // init the comparator for the vertex event
        element.comp = gb_tessellator_event_queue_comp;

#ifdef __gb_debug__
        // init the c-string function for tb_priority_queue_dump
        element.cstr = gb_tessellator_event_queue_cstr;
#endif

        // make event queue
        impl->event_queue = tb_priority_queue_init(0, element);
    }
    tb_assert_abort_and_check_return_val(impl->event_queue, tb_false);

    // clear event queue first
    tb_priority_queue_clear(impl->event_queue);

    // done
    tb_for_all_if (gb_mesh_vertex_ref_t, vertex, gb_mesh_vertex_itor(mesh), vertex)
    {
        // put vertex event to the queue
        tb_priority_queue_put(impl->event_queue, vertex);
    }

    // ok
    return tb_priority_queue_size(impl->event_queue);
}
tb_void_t gb_tessellator_event_queue_insert(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert(impl && impl->event_queue && event);

    // insert this event
    tb_priority_queue_put(impl->event_queue, event);
}
tb_void_t gb_tessellator_event_queue_remove(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert(impl && impl->event_queue && event);

    // find it
    tb_size_t itor = tb_find_all_if(impl->event_queue, gb_tessellator_event_queue_find, event);
    if (itor != tb_iterator_tail(impl->event_queue))
    {
        // remove this event
        tb_priority_queue_remove(impl->event_queue, itor);
    }
}
