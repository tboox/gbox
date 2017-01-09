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
 * @file        cache_stack.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "cache_stack.h"
#include "../path.h"
#include "../paint.h"
#include "../clipper.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the cache stack impl type
typedef struct __gb_cache_stack_impl_t
{
    // the object type
    tb_size_t                   type;

    // the stack
    tb_stack_ref_t              stack;

    // the cache
    tb_stack_ref_t              cache;

    // the object
    tb_handle_t                 object;

    // the cache size
    tb_size_t                   cache_size;

}gb_cache_stack_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_handle_t gb_cache_stack_object_init(tb_size_t type)
{ 
    // done
    tb_handle_t object = tb_null;
    switch (type)
    {
    case GB_CACHE_STACK_TYPE_PATH:
        object = (tb_handle_t)gb_path_init();
        break;
    case GB_CACHE_STACK_TYPE_PAINT:
        object = (tb_handle_t)gb_paint_init();
        break;
    case GB_CACHE_STACK_TYPE_CLIPPER:
        object = (tb_handle_t)gb_clipper_init();
        break;
    default:
        break;
    }

    // ok?
    return object;
}
static tb_void_t gb_cache_stack_object_exit(tb_size_t type, tb_handle_t object)
{ 
    // check
    tb_assert_and_check_return(object);

    // done
    switch (type)
    {
    case GB_CACHE_STACK_TYPE_PATH:
        gb_path_exit((gb_path_ref_t)object);
        break;
    case GB_CACHE_STACK_TYPE_PAINT:
        gb_paint_exit((gb_paint_ref_t)object);
        break;
    case GB_CACHE_STACK_TYPE_CLIPPER:
        gb_clipper_exit((gb_clipper_ref_t)object);
        break;
    default:
        break;
    }
}
static tb_void_t gb_cache_stack_object_copy(tb_size_t type, tb_handle_t object, tb_handle_t copied)
{ 
    // check
    tb_assert_and_check_return(object);

    // done
    switch (type)
    {
    case GB_CACHE_STACK_TYPE_PATH:
        gb_path_copy((gb_path_ref_t)object, (gb_path_ref_t)copied);
        break;
    case GB_CACHE_STACK_TYPE_PAINT:
        gb_paint_copy((gb_paint_ref_t)object, (gb_paint_ref_t)copied);
        break;
    case GB_CACHE_STACK_TYPE_CLIPPER:
        gb_clipper_copy((gb_clipper_ref_t)object, (gb_clipper_ref_t)copied);
        break;
    default:
        break;
    }
}
static tb_bool_t gb_cache_stack_object_free(tb_iterator_ref_t iterator, tb_pointer_t item, tb_cpointer_t priv)
{
    // the type
    tb_size_t   type = (tb_size_t)priv;

    // the object
    tb_handle_t object = (tb_handle_t)item;

    // exit object 
    if (object) gb_cache_stack_object_exit(type, object);

    // ok
    return tb_true;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_cache_stack_ref_t gb_cache_stack_init(tb_size_t grow, tb_size_t type)
{
    // done
    tb_bool_t               ok = tb_false;
    gb_cache_stack_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(type && type < GB_CACHE_STACK_TYPE_MAXN);

        // make stack
        impl = tb_malloc0_type(gb_cache_stack_impl_t);
        tb_assert_and_check_break(impl);

        // init stack
        impl->type          = type;
        impl->cache_size    = grow? grow : 8;

        // init cache
        impl->cache = tb_stack_init(grow, tb_element_ptr(tb_null, tb_null));
        tb_assert_and_check_break(impl->cache);

        // init stack
        impl->stack = tb_stack_init(grow, tb_element_ptr(tb_null, tb_null));
        tb_assert_and_check_break(impl->stack);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_cache_stack_exit((gb_cache_stack_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_cache_stack_ref_t)impl;
}
tb_void_t gb_cache_stack_exit(gb_cache_stack_ref_t stack)
{
    // check
    gb_cache_stack_impl_t* impl = (gb_cache_stack_impl_t*)stack;
    tb_assert_and_check_return(impl);

    // exit object
    if (impl->object) gb_cache_stack_object_exit(impl->type, impl->object);
    impl->object = tb_null;

    // exit stack
    if (impl->stack) 
    {
        // exit objects
        tb_walk_all(impl->stack, gb_cache_stack_object_free, (tb_cpointer_t)impl->type);

        // exit stack
        tb_stack_exit(impl->stack);
        impl->stack = tb_null;
    }

    // exit cache
    if (impl->cache) 
    {
        // exit objects
        tb_walk_all(impl->cache, gb_cache_stack_object_free, (tb_cpointer_t)impl->type);

        // exit cache
        tb_stack_exit(impl->cache);
        impl->cache = tb_null;
    }
            
    // exit it
    tb_free(impl);
}
tb_handle_t gb_cache_stack_save(gb_cache_stack_ref_t stack)
{
    // check
    gb_cache_stack_impl_t* impl = (gb_cache_stack_impl_t*)stack;
    tb_assert_and_check_return_val(impl && impl->cache && impl->stack, tb_null);

    // init object first if be null
    if (!impl->object) impl->object = gb_cache_stack_object_init(impl->type);
    tb_assert_and_check_return_val(impl->object, tb_null);

    // get a new object from cache first
    tb_handle_t object = tb_null;
    if (tb_stack_size(impl->cache))
    {
        // get
        object = (tb_handle_t)tb_stack_top(impl->cache);
        tb_assert_and_check_return_val(impl->cache, tb_null);

        // pop
        tb_stack_pop(impl->cache);
    }
    // make a new object
    else object = gb_cache_stack_object_init(impl->type);
    tb_assert_and_check_return_val(object, tb_null);

    // init the new object using the old object
    gb_cache_stack_object_copy(impl->type, object, impl->object);

    // save the old object
    tb_stack_put(impl->stack, impl->object);

    // update the current object
    impl->object = object;

    // ok
    return impl->object;
}
tb_void_t gb_cache_stack_load(gb_cache_stack_ref_t stack)
{
    // check
    gb_cache_stack_impl_t* impl = (gb_cache_stack_impl_t*)stack;
    tb_assert_and_check_return(impl && impl->cache && impl->stack);

    // init object first if be null
    if (!impl->object) impl->object = gb_cache_stack_object_init(impl->type);
    tb_assert_and_check_return(impl->object);

    // load new object from the stack top
    tb_handle_t object = (tb_handle_t)tb_stack_top(impl->stack);
    tb_assert_and_check_return(object);

    // pop the top object 
    tb_stack_pop(impl->stack);

    // put current object to the cache if the cache be not full
    if (impl->object && tb_stack_size(impl->cache) < impl->cache_size)
        tb_stack_put(impl->cache, impl->object);
    // exit the current object
    else gb_cache_stack_object_exit(impl->type, impl->object);

    // update the current object
    impl->object = object;
}
tb_handle_t gb_cache_stack_object(gb_cache_stack_ref_t stack)
{
    // check
    gb_cache_stack_impl_t* impl = (gb_cache_stack_impl_t*)stack;
    tb_assert_and_check_return_val(impl, tb_null);

    // init object first if be null
    if (!impl->object) impl->object = gb_cache_stack_object_init(impl->type);
    tb_assert_and_check_return_val(impl->object, tb_null);

    // the current object
    return impl->object;
}
