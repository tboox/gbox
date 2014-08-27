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
 * @file        prefix.c
 * @ingroup     platform
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_prefix"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_window_impl_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->info.draw && canvas);

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
        tb_trace_d("framerate: " GB_FMT, impl->framerate);

        // clear the frame count
		impl->fps_count = 0;

        // update the frame time
		impl->fps_time = time;
    }

    // spak timer
    if (impl->timer) tb_timer_spak(impl->timer);

    // done draw
    impl->info.draw((gb_window_ref_t)impl, canvas, impl->info.priv);
}
tb_void_t gb_window_impl_event(gb_window_ref_t window, gb_event_ref_t event)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->info.event && event);

    // done event
    impl->info.event((gb_window_ref_t)impl, event, impl->info.priv);
}


