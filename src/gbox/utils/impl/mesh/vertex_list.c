/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
 * @file        vertex_list.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME                "vertex_list"
#define TB_TRACE_MODULE_DEBUG               (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "vertex_list.h"

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

    // the element
    tb_element_t                element;

    // the order
    tb_size_t                   order;

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
    impl->element.free(&impl->element, (tb_pointer_t)((gb_mesh_vertex_ref_t)data + 1));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_vertex_list_ref_t gb_mesh_vertex_list_init(tb_element_t element)
{
    // check
    tb_assert_and_check_return_val(element.data && element.dupl && element.repl, tb_null);

    // done
    tb_bool_t                       ok = tb_false;
    gb_mesh_vertex_list_impl_t*     impl = tb_null;
    do
    {
        // make list
        impl = tb_malloc0_type(gb_mesh_vertex_list_impl_t);
        tb_assert_and_check_break(impl);

        // init element
        impl->element = element;

        // init pool, item = vertex + data
        impl->pool = tb_fixed_pool_init(tb_null, GB_MESH_VERTEX_LIST_GROW, sizeof(gb_mesh_vertex_t) + element.size, tb_null, gb_mesh_vertex_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->pool);

        // init head
        tb_list_entry_init_(&impl->head, 0, sizeof(gb_mesh_vertex_t) + element.size, tb_null);

        // init order
        impl->order = GB_MESH_ORDER_INSERT_TAIL;

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

#ifdef __gb_debug__
    // clear id
    impl->id = 0;
#endif
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
    tb_assert(tb_list_entry_size(&impl->head) == tb_fixed_pool_size(impl->pool));

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

    // save list
    vertex->list = (tb_pointer_t)list;
#endif

    // insert to the vertex list
    switch (impl->order)
    {
    case GB_MESH_ORDER_INSERT_HEAD:
        tb_list_entry_insert_head(&impl->head, &vertex->entry);
        break;
    case GB_MESH_ORDER_INSERT_TAIL:
    default:
        tb_list_entry_insert_tail(&impl->head, &vertex->entry);
        break;
    }

    // ok
    return vertex;
}
#ifdef __gb_debug__
tb_long_t gb_mesh_vertex_list_cstr(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->element.cstr && vertex && maxn, -1);
  
    // make it
    tb_char_t       vertex_info[256] = {0};
    tb_cpointer_t   vertex_data = gb_mesh_vertex_list_data(list, vertex);
    return vertex_data? tb_snprintf(data, maxn, "(%s)", impl->element.cstr(&impl->element, vertex_data, vertex_info, sizeof(vertex_info))) : tb_snprintf(data, maxn, "(v%lu)", vertex->id);
}
#endif
tb_void_t gb_mesh_vertex_list_kill(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->pool && vertex);

#ifdef __gb_debug__
    // check
    tb_assert(vertex->id);

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
    tb_assert_and_check_return_val(impl && impl->element.data && vertex, tb_null);

    // the user data
    return impl->element.data(&impl->element, gb_mesh_vertex_user(vertex));
}
tb_void_t gb_mesh_vertex_list_data_set(gb_mesh_vertex_list_ref_t list, gb_mesh_vertex_ref_t vertex, tb_cpointer_t data)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->element.repl && vertex);

    // set the user data
    impl->element.repl(&impl->element, gb_mesh_vertex_user(vertex), data);
}
tb_size_t gb_mesh_vertex_list_order(gb_mesh_vertex_list_ref_t list)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return_val(impl, GB_MESH_ORDER_INSERT_TAIL);

    // the order
    return impl->order;
}
tb_void_t gb_mesh_vertex_list_order_set(gb_mesh_vertex_list_ref_t list, tb_size_t order)
{
    // check
    gb_mesh_vertex_list_impl_t* impl = (gb_mesh_vertex_list_impl_t*)list;
    tb_assert_and_check_return(impl);

    // set the order
    impl->order = order;
}


