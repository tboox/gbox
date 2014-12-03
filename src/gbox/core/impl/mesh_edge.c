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
 * @file        mesh_edge.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME                "mesh_edge"
#define TB_TRACE_MODULE_DEBUG               (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh_edge.h"
#include "mesh_face.h"
#include "mesh_edge.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// edge->dst
#define dst         sym->org

// edge->rface
#define rface       sym->lface

// edge->oprev
#define oprev       sym->lnext

// edge->lprev
#define lprev       onext->sym

// edge->dprev
#define dprev       lnext->sym

// edge->rprev
#define rprev       sym->onext

// edge->dnext
#define dnext       rprev->sym

// edge->rnext
#define rnext       oprev->sym

// the mesh edge list grow
#ifdef __tb_small__ 
#   define GB_MESH_EDGE_LIST_GROW               (128)
#else
#   define GB_MESH_EDGE_LIST_GROW               (256)
#endif

// the mesh edge list maxn
#ifdef __tb_small__
#   define GB_MESH_EDGE_LIST_MAXN               (1 << 16)
#else
#   define GB_MESH_EDGE_LIST_MAXN               (1 << 30)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh edge list impl type
typedef struct __gb_mesh_edge_list_impl_t
{
    // the itor
    tb_iterator_t               itor;

    // the pool
    tb_fixed_pool_ref_t         pool;

    // the head
    tb_list_entry_head_t        head;

    // the func
    tb_item_func_t              func;

}gb_mesh_edge_list_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_edge_list_ref_t gb_mesh_edge_list_init(tb_size_t grow, tb_item_func_t func)
{
    // check
    tb_assert_and_check_return_val(func.size && func.data && func.dupl && func.repl, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    gb_mesh_edge_list_impl_t*   impl = tb_null;
    do
    {
        // using the default grow
        if (!grow) grow = GB_MESH_EDGE_LIST_GROW;

        // make list
        impl = tb_malloc0_type(gb_mesh_edge_list_impl_t);
        tb_assert_and_check_break(impl);

        // init func
        impl->func = func;

#if 0
        // init iterator
        impl->itor.mode = TB_ITERATOR_MODE_FORWARD | TB_ITERATOR_MODE_REVERSE | TB_ITERATOR_MODE_READONLY;
        impl->itor.priv = tb_null;
        impl->itor.step = func.size;
        impl->itor.size = gb_mesh_edge_list_itor_size;
        impl->itor.head = gb_mesh_edge_list_itor_head;
        impl->itor.last = gb_mesh_edge_list_itor_last;
        impl->itor.tail = gb_mesh_edge_list_itor_tail;
        impl->itor.prev = gb_mesh_edge_list_itor_prev;
        impl->itor.next = gb_mesh_edge_list_itor_next;
        impl->itor.item = gb_mesh_edge_list_itor_item;
        impl->itor.copy = gb_mesh_edge_list_itor_copy;
        impl->itor.comp = gb_mesh_edge_list_itor_comp;

        // init pool, item = (edge + data) + (edge->sym + data)
        impl->pool = tb_fixed_pool_init(tb_null, grow, (sizeof(gb_mesh_edge_t) + func.size) << 1, tb_null, gb_mesh_edge_list_item_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->pool);
#endif

        // TODO
        // init head
        tb_list_entry_init_(&impl->head, 0, (sizeof(gb_mesh_edge_t) + func.size) << 1, tb_null);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_mesh_edge_list_exit((gb_mesh_edge_list_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_mesh_edge_list_ref_t)impl;
}
tb_void_t gb_mesh_edge_list_exit(gb_mesh_edge_list_ref_t list)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return(impl);
   
    // clear pool
    if (impl->pool) tb_fixed_pool_clear(impl->pool);

    // clear head
    tb_list_entry_clear(&impl->head);

    // exit pool
    if (impl->pool) tb_fixed_pool_exit(impl->pool);
    impl->pool = tb_null;

    // exit it
    tb_free(impl);
}
tb_size_t gb_mesh_edge_list_size(gb_mesh_edge_list_ref_t list)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, 0);
    tb_assert_abort(tb_list_entry_size(&impl->head) == tb_fixed_pool_size(impl->pool));

    // the size
    return tb_list_entry_size(&impl->head);
}
tb_size_t gb_mesh_edge_list_maxn(gb_mesh_edge_list_ref_t list)
{
    // the edge maxn
    return GB_MESH_EDGE_LIST_MAXN;
}

