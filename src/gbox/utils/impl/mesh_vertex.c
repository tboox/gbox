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
 * @file        mesh_vertex.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME                "mesh_vertex"
#define TB_TRACE_MODULE_DEBUG               (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh_vertex.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the mesh vertex list grow
#ifdef __tb_small__ 
#   define GB_MESH_VERTEX_LIST_GROW                 (128)
#else
#   define GB_MESH_VERTEX_LIST_GROW                 (256)
#endif

// the mesh vertex list maxn
#ifdef __tb_small__
#   define GB_MESH_VERTEX_LIST_MAXN                 (1 << 16)
#else
#   define GB_MESH_VERTEX_LIST_MAXN                 (1 << 30)
#endif

// the vertex user data pointer
#define gb_mesh_vertex_user(vertex)                 ((gb_mesh_vertex_ref_t)(vertex) + 1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh vertex list impl type
typedef struct __gb_mesh_vertex_list_impl_t
{
    // the pool
    tb_fixed_pool_ref_t         pool;

    // the head
    tb_list_entry_head_t        head;

    // the func
    tb_item_func_t              func;

#ifdef __gb_debug__
    // the id
    tb_size_t                   id;
#endif

}gb_mesh_vertex_list_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_mesh_vertex_exit(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)priv;
    tb_assert_and_check_return(impl && data);

    // exit the user data
    impl->func.free(&impl->func, (tb_pointer_t)((gb_mesh_vertex_ref_t)data + 1));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_vertex_list_ref_t gb_mesh_vertex_list_init(tb_item_func_t func)
{
    // check
    tb_assert_and_check_return_val(func.data && func.dupl && func.repl, tb_null);

    // done
    tb_bool_t                       ok = tb_false;
    gb_mesh_vertex_list_impl_t*     impl = tb_null;
    do
    {
        // make list
        impl = tb_malloc0_type(gb_mesh_vertex_list_impl_t);
        tb_assert_and_check_break(impl);

        // init func
        impl->func = func;

        // init pool, item = vertex + data
        impl->pool = tb_fixed_pool_init(tb_null, GB_MESH_VERTEX_LIST_GROW, sizeof(gb_mesh_vertex_t) + func.size, tb_null, gb_mesh_vertex_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->pool);

        // init head
        tb_list_entry_init_(&impl->head, 0, sizeof(gb_mesh_vertex_t) + func.size, tb_null);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_mesh_vertex_list_exit((gb_mesh_vertex_list_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_mesh_vertex_list_ref_t)impl;
}
tb_void_t gb_mesh_vertex_list_exit(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl);
   
    // clear it first
    gb_mesh_vertex_list_clear(list);

    // exit pool
    if (impl->pool) tb_fixed_pool_exit(impl->pool);
    impl->pool = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_mesh_vertex_list_clear(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl);
   
    // clear pool
    if (impl->pool) tb_fixed_pool_clear(impl->pool);

    // clear head
    tb_list_entry_clear(&impl->head);
}
tb_iterator_ref_t gb_mesh_vertex_list_itor(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl, tb_null);
   
    // the iterator
    return tb_list_entry_itor(&impl->head);
}
tb_size_t gb_mesh_vertex_list_size(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, 0);
    tb_assert_abort(tb_list_entry_size(&impl->head) == tb_fixed_pool_size(impl->pool));

    // the size
    return tb_list_entry_size(&impl->head);
}
tb_size_t gb_mesh_vertex_list_maxn(gb_mesh_vertex_list_ref_t list)
{
    // the vertex maxn
    return GB_MESH_VERTEX_LIST_MAXN;
}
gb_mesh_vertex_ref_t gb_mesh_vertex_list_make(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, tb_null);

    // make it
    gb_mesh_vertex_ref_t vertex = (gb_mesh_vertex_ref_t)tb_fixed_pool_malloc0(impl->pool);
    tb_assert_and_check_return_val(vertex, tb_null);

#ifdef __gb_debug__
    // init id
    vertex->id = ++impl->id;
#endif

    // insert to the vertex list
    tb_list_entry_insert_tail(&impl->head, &vertex->entry);

    // ok
    return vertex;
}
tb_char_t const* gb_mesh_vertex_list_cstr(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->func.cstr && vertex && maxn, tb_null);
  
    // make it
    tb_char_t info[256] = {0};
#ifdef __gb_debug__
    tb_long_t size = tb_snprintf(data, maxn, "(%lu: %s)", vertex->id, impl->func.cstr(&impl->func, gb_mesh_vertex_list_data(list, vertex), info, sizeof(info)));
#else
    tb_long_t size = tb_snprintf(data, maxn, "(%p: %s)", vertex, impl->func.cstr(&impl->func, gb_mesh_vertex_list_data(list, vertex), info, sizeof(info)));
#endif
    if (size >= 0) data[size] = '\0';

    // ok?
    return data;
}
tb_void_t gb_mesh_vertex_list_kill(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->pool && vertex);

#ifdef __gb_debug__
    // check
    tb_assert_abort(vertex->id);

    // clear id
    vertex->id = 0;
#endif

    // remove from the vertex list
    tb_list_entry_remove(&impl->head, &vertex->entry);

    // exit it
    tb_fixed_pool_free(impl->pool, vertex);
}
tb_cpointer_t gb_mesh_vertex_list_data(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->func.data && vertex, tb_null);

    // the user data
    return impl->func.data(&impl->func, gb_mesh_vertex_user(vertex));
}
tb_void_t gb_mesh_vertex_list_data_set(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_cpointer_t data)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->func.repl && vertex);

    // set the user data
    impl->func.repl(&impl->func, gb_mesh_vertex_user(vertex), data);
}

