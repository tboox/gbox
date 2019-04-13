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
 * @file        face_list.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME                "face_list"
#define TB_TRACE_MODULE_DEBUG               (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "face_list.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the mesh face list grow
#ifdef __tb_small__ 
#   define GB_MESH_FACE_LIST_GROW               (32)
#else
#   define GB_MESH_FACE_LIST_GROW               (64)
#endif

// the mesh face list maxn
#ifdef __tb_small__
#   define GB_MESH_FACE_LIST_MAXN               (1 << 16)
#else
#   define GB_MESH_FACE_LIST_MAXN               (1 << 30)
#endif

// the face user data pointer
#define gb_mesh_face_user(face)                 ((gb_mesh_face_ref_t)(face) + 1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh face list impl type
typedef struct __gb_mesh_face_list_impl_t
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

}gb_mesh_face_list_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_mesh_face_exit(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)priv;
    tb_assert_and_check_return(impl && data);

    // exit the user data
    impl->element.free(&impl->element, (tb_pointer_t)((gb_mesh_face_ref_t)data + 1));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_mesh_face_list_ref_t gb_mesh_face_list_init(tb_element_t element)
{
    // check
    tb_assert_and_check_return_val(element.data && element.dupl && element.repl, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    gb_mesh_face_list_impl_t*   impl = tb_null;
    do
    {
        // make list
        impl = tb_malloc0_type(gb_mesh_face_list_impl_t);
        tb_assert_and_check_break(impl);

        // init element
        impl->element = element;

        // init pool, item = face + data
        impl->pool = tb_fixed_pool_init(tb_null, GB_MESH_FACE_LIST_GROW, sizeof(gb_mesh_face_t) + element.size, tb_null, gb_mesh_face_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->pool);

        // init head
        tb_list_entry_init_(&impl->head, 0, sizeof(gb_mesh_face_t) + element.size, tb_null);

        // init order
        impl->order = GB_MESH_ORDER_INSERT_TAIL;

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_mesh_face_list_exit((gb_mesh_face_list_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_mesh_face_list_ref_t)impl;
}
tb_void_t gb_mesh_face_list_exit(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return(impl);
   
    // clear it first
    gb_mesh_face_list_clear(list);

    // exit pool
    if (impl->pool) tb_fixed_pool_exit(impl->pool);
    impl->pool = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_mesh_face_list_clear(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
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
tb_iterator_ref_t gb_mesh_face_list_itor(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl, tb_null);
   
    // the iterator
    return tb_list_entry_itor(&impl->head);
}
tb_size_t gb_mesh_face_list_size(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, 0);
    tb_assert(tb_list_entry_size(&impl->head) == tb_fixed_pool_size(impl->pool));

    // the size
    return tb_list_entry_size(&impl->head);
}
tb_size_t gb_mesh_face_list_maxn(gb_mesh_face_list_ref_t list)
{
    // the face maxn
    return GB_MESH_FACE_LIST_MAXN;
}
gb_mesh_face_ref_t gb_mesh_face_list_make(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->pool, tb_null);

    // make it
    gb_mesh_face_ref_t face = (gb_mesh_face_ref_t)tb_fixed_pool_malloc0(impl->pool);
    tb_assert_and_check_return_val(face, tb_null);

#ifdef __gb_debug__
    // init id
    face->id = ++impl->id;

    // save list
    face->list = (tb_pointer_t)list;
#endif

    // insert to the face list
    switch (impl->order)
    {
    case GB_MESH_ORDER_INSERT_HEAD:
        tb_list_entry_insert_head(&impl->head, &face->entry);
        break;
    case GB_MESH_ORDER_INSERT_TAIL:
    default:
        tb_list_entry_insert_tail(&impl->head, &face->entry);
        break;
    }

    // ok
    return face;
}
#ifdef __gb_debug__
tb_long_t gb_mesh_face_list_cstr(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face, tb_char_t* data, tb_size_t maxn)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->element.cstr && face && maxn, -1);
  
    // make it
    tb_char_t       face_info[256] = {0};
    tb_cpointer_t   face_data = gb_mesh_face_list_data(list, face);
    return face_data? tb_snprintf(data, maxn, "(%s)", impl->element.cstr(&impl->element, face_data, face_info, sizeof(face_info))) : tb_snprintf(data, maxn, "(f%lu)", face->id);
}
#endif
tb_void_t gb_mesh_face_list_kill(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->pool && face);

#ifdef __gb_debug__
    // check
    tb_assert(face->id);

    // clear id
    face->id = 0;
#endif

    // remove from the face list
    tb_list_entry_remove(&impl->head, &face->entry);

    // exit it
    tb_fixed_pool_free(impl->pool, face);
}
tb_cpointer_t gb_mesh_face_list_data(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl && impl->element.data && face, tb_null);

    // the user data
    return impl->element.data(&impl->element, gb_mesh_face_user(face));
}
tb_void_t gb_mesh_face_list_data_set(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face, tb_cpointer_t data)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return(impl && impl->element.repl && face);

    // set the user data
    impl->element.repl(&impl->element, gb_mesh_face_user(face), data);
}
tb_size_t gb_mesh_face_list_order(gb_mesh_face_list_ref_t list)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return_val(impl, GB_MESH_ORDER_INSERT_TAIL);

    // the order
    return impl->order;
}
tb_void_t gb_mesh_face_list_order_set(gb_mesh_face_list_ref_t list, tb_size_t order)
{
    // check
    gb_mesh_face_list_impl_t* impl = (gb_mesh_face_list_impl_t*)list;
    tb_assert_and_check_return(impl);

    // set the order
    impl->order = order;
}


