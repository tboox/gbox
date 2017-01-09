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
 * @file        window.c
 * @ingroup     platform
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_impl"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_hong_t gb_window_impl_spak(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert(impl);

    // spak the cache time
    tb_hong_t time = tb_cache_time_spak();

    // init the frame time
    if (!impl->fps_time) impl->fps_time = time;
    
    // update the frame count
    impl->fps_count++;

    // > 1s?
    if (time > impl->fps_time + 1000)
    {
        // the framerate
        impl->framerate = gb_imuldiv(gb_long_to_float(1000), impl->fps_count, (tb_long_t)(time - impl->fps_time));

        // trace
        tb_trace_d("framerate: %{float}", &impl->framerate);

        // clear the frame count
		impl->fps_count = 0;

        // update the frame time
		impl->fps_time = time;
    }

    // spak timer
    if (impl->timer) tb_timer_spak(impl->timer);

    // the spak time
    return time;
}
tb_void_t gb_window_impl_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert(impl && impl->info.draw && canvas);

    // done draw
    impl->info.draw((gb_window_ref_t)impl, canvas, impl->info.priv);
}
tb_void_t gb_window_impl_event(gb_window_ref_t window, gb_event_ref_t event)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert(impl && impl->info.event && event);

    // done event
    impl->info.event((gb_window_ref_t)impl, event, impl->info.priv);
}


