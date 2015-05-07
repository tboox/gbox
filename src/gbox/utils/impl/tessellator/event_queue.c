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
    // lvertex <= rvertex ? -1 : 1
    return (!gb_tessellator_vertex_leq((gb_mesh_vertex_ref_t)ldata, (gb_mesh_vertex_ref_t)rdata) << 1) - 1;
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
    tb_assert_abort(impl);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);

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
    tb_assert_abort(impl && impl->event_queue && event);

    // insert this event
    tb_priority_queue_put(impl->event_queue, event);
}
tb_void_t gb_tessellator_event_queue_remove(gb_tessellator_impl_t* impl, gb_mesh_vertex_ref_t event)
{
    // check
    tb_assert_abort(impl && impl->event_queue && event);

    // find it
    tb_size_t itor = tb_find_all_if(impl->event_queue, gb_tessellator_event_queue_find, event);
    if (itor != tb_iterator_tail(impl->event_queue))
    {
        // remove this event
        tb_priority_queue_remove(impl->event_queue, itor);
    }
}
