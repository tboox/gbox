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
 * @ingroup     utils
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

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

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
    // the pool
    tb_fixed_pool_ref_t             pool;

    // the head edge
    gb_mesh_edge_t                  head[2];

    // the edge size
    tb_size_t                       edge_size;

    // the func
    tb_item_func_t                  func;

}gb_mesh_edge_list_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_mesh_edge_exit(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)priv;
    tb_assert_and_check_return(impl && data);

    // exit the user data
    impl->func.free(&impl->func, (tb_pointer_t)((gb_mesh_edge_ref_t)data + 1));
    impl->func.free(&impl->func, (tb_pointer_t)((gb_mesh_edge_ref_t)((tb_byte_t*)data + impl->edge_size) + 1));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tb_inline__ tb_void_t gb_mesh_edge_init(gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_abort(edge);

    // the edge sym
    gb_mesh_edge_ref_t edge_sym = edge->sym;
    tb_assert_abort(edge_sym && edge < edge_sym);

    /* init edge and make self-loop
     *
     *  edge: ..............e.............. : edge_sym
     *    |                                     /|\
     *   \|/                                     |
     *  edge: ..............e.............. : edge_sym
     *
     */
    edge->next     = edge;
    edge_sym->next = edge_sym;
}
static __tb_inline__ tb_void_t gb_mesh_edge_insert_prev(gb_mesh_edge_ref_t edge, gb_mesh_edge_ref_t edge_next)
{
    // check
    tb_assert_abort(edge && edge_next);

    // the edge sym
    gb_mesh_edge_ref_t edge_sym = edge->sym;
    tb_assert_abort(edge_sym && edge < edge_sym);

    // the edge next sym
    gb_mesh_edge_ref_t edge_next_sym = edge_next->sym;
    tb_assert_abort(edge_next_sym && edge_next < edge_next_sym);

    // the edge prev sym
	gb_mesh_edge_ref_t edge_prev_sym = edge_next_sym->next;
    tb_assert_abort(edge_prev_sym && edge_prev_sym->sym);

    /* insert edge before the next edge
     *
     *  edge_prev : ..............e.............. : edge_prev_sym
     *       |    |                             /|\    /|\
     *       |   \|/                             |      | 
     *          edge : ...........e..........: edge_sym 
     *       |    |                             /|\     |
     *      \|/  \|/                             |      |
     *  edge_next : ..............e.............. : edge_next_sym
     *
     */
	edge_sym->next              = edge_prev_sym;
	edge_prev_sym->sym->next    = edge;
	edge->next                  = edge_next;
	edge_next_sym->next         = edge_sym;
}
static __tb_inline__ tb_void_t gb_mesh_edge_remove(gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_abort(edge);

    // the edge sym
    gb_mesh_edge_ref_t edge_sym = edge->sym;
    tb_assert_abort(edge_sym && edge < edge_sym);

    // the edge next
	gb_mesh_edge_ref_t edge_next = edge->next;
    tb_assert_abort(edge_next && edge_next->sym);

    // the edge prev sym
	gb_mesh_edge_ref_t edge_prev_sym = edge_sym->next;
    tb_assert_abort(edge_prev_sym && edge_prev_sym->sym);

    /* remove edge 
     *
     *  edge_prev : ..............e.............. : edge_prev_sym
     *       |    |                             /|\    /|\
     *       |   \|/                             |      | 
     *       |  edge : ...........e..........: edge_sym |
     *       |    |                             /|\     |
     *      \|/  \|/                             |      |
     *  edge_next : ..............e.............. : edge_next_sym
     *
     */
	edge_next->sym->next = edge_prev_sym;
	edge_prev_sym->sym->next = edge_next;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_edge_list_ref_t gb_mesh_edge_list_init(tb_item_func_t func)
{
    // check
    tb_assert_and_check_return_val(func.data && func.dupl && func.repl, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    gb_mesh_edge_list_impl_t*   impl = tb_null;
    do
    {
        // make list
        impl = tb_malloc0_type(gb_mesh_edge_list_impl_t);
        tb_assert_and_check_break(impl);

        // init func
        impl->func = func;

        // init edge size
        impl->edge_size = tb_align_cpu(sizeof(gb_mesh_edge_t) + func.size);

        // init pool, item = (edge + data) + (edge->sym + data)
        impl->pool = tb_fixed_pool_init(tb_null, GB_MESH_EDGE_LIST_GROW, impl->edge_size << 1, tb_null, gb_mesh_edge_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->pool);

        // init head edge
        impl->head[0].sym = &impl->head[1];
        impl->head[1].sym = &impl->head[0];

        // init edge list
        gb_mesh_edge_init(impl->head);

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
   
    // clear it first
    gb_mesh_edge_list_clear(list);

    // exit pool
    if (impl->pool) tb_fixed_pool_exit(impl->pool);
    impl->pool = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_mesh_edge_list_clear(gb_mesh_edge_list_ref_t list)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return(impl);
   
    // clear pool
    if (impl->pool) tb_fixed_pool_clear(impl->pool);

    // clear list
    gb_mesh_edge_init(impl->head);
}
tb_size_t gb_mesh_edge_list_size(gb_mesh_edge_list_ref_t list)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, 0);

    // the size
    return tb_fixed_pool_size(impl->pool);
}
tb_size_t gb_mesh_edge_list_maxn(gb_mesh_edge_list_ref_t list)
{
    // the edge maxn
    return GB_MESH_EDGE_LIST_MAXN;
}
gb_mesh_edge_ref_t gb_mesh_edge_list_make(gb_mesh_edge_list_ref_t list)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, tb_null);

    // make it
    gb_mesh_edge_ref_t edge = (gb_mesh_edge_ref_t)tb_fixed_pool_malloc0(impl->pool);
    tb_assert_and_check_return_val(edge, tb_null);

    // the edge sym
    gb_mesh_edge_ref_t edge_sym = (gb_mesh_edge_ref_t)((tb_byte_t*)edge + impl->edge_size);

    // init edge
    edge->sym       = edge_sym;
    edge->onext     = edge;
    edge->lnext     = edge;

    // init edge sym
    edge_sym->sym   = edge;
    edge_sym->onext = edge_sym;
    edge_sym->lnext = edge_sym;

    // insert edge to the tail of list
    gb_mesh_edge_insert_prev(edge, impl->head);

    // ok
    return edge;
}
tb_void_t gb_mesh_edge_list_kill(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge)
{
    // check
    gb_mesh_edge_list_impl_t* impl = (gb_mesh_edge_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->pool && edge);

    // make sure the edge points to the first half-edge
    if (edge->sym < edge) edge = edge->sym;

    // remove it from the list
    gb_mesh_edge_remove(edge);

    // exit it
    tb_fixed_pool_free(impl->pool, edge);
}

