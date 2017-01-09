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
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "window.h"
#include "impl/window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the timer maxn
#ifdef __gb_small__
#   define GB_WINDOW_TIMER_MAXN         (64)
#else
#   define GB_WINDOW_TIMER_MAXN         (256)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
__tb_extern_c__ gb_window_ref_t gb_window_init_ios(gb_window_info_ref_t info);
__tb_extern_c__ gb_window_ref_t gb_window_init_mac(gb_window_info_ref_t info);
__tb_extern_c__ gb_window_ref_t gb_window_init_windows(gb_window_info_ref_t info);
__tb_extern_c__ gb_window_ref_t gb_window_init_android(gb_window_info_ref_t info);

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init(gb_window_info_ref_t info)
{
#if defined(TB_CONFIG_OS_IOS)
    return gb_window_init_ios(info);
#elif defined(TB_CONFIG_OS_ANDROID)
    return gb_window_init_android(info);
#elif 0//defined(TB_CONFIG_OS_MACOSX)
    return gb_window_init_mac(info);
#elif 0//defined(TB_CONFIG_OS_WINDOWS)
    return gb_window_init_windows(info);
#elif defined(GB_CONFIG_PACKAGE_HAVE_X11)
    return gb_window_init_x11(info);
#elif defined(GB_CONFIG_PACKAGE_HAVE_GLUT)
    return gb_window_init_glut(info);
#elif defined(GB_CONFIG_PACKAGE_HAVE_SDL)
    return gb_window_init_sdl(info);
#else
#   error no avaliable window
#endif
}
tb_void_t gb_window_exit(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl);

    // exit timer
    if (impl->timer) tb_timer_exit(impl->timer);
    impl->timer = tb_null;

    // exit it
    if (impl->exit) impl->exit(window);
}
tb_void_t gb_window_loop(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->loop);

    // loop it
    impl->loop(window);
}
tb_void_t gb_window_fullscreen(gb_window_ref_t window, tb_bool_t fullscreen)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->fullscreen);

    // fullscreen it
    impl->fullscreen(window, fullscreen);
}
tb_void_t gb_window_maximum(gb_window_ref_t window, tb_bool_t maximum)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->maximum);

    // maximum it
    impl->maximum(window, maximum);
}
tb_void_t gb_window_minimum(gb_window_ref_t window, tb_bool_t minimum)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->minimum);

    // minimum it
    impl->minimum(window, minimum);
}
tb_void_t gb_window_show(gb_window_ref_t window, tb_bool_t show)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl && impl->show);

    // show it
    impl->show(window, show);
}
tb_size_t gb_window_type(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, GB_WINDOW_TYPE_NONE);

    // the type
    return impl->type;
}
tb_size_t gb_window_mode(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, GB_WINDOW_MODE_NONE);

    // the mode
    return impl->mode;
}
tb_size_t gb_window_flag(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, GB_WINDOW_FLAG_NONE);

    // the flag
    return impl->flag;
}
tb_size_t gb_window_pixfmt(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
    return impl->pixfmt;
}
tb_size_t gb_window_width(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, 0);

    // the width
    return impl->width;
}
tb_size_t gb_window_height(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, 0);

    // the height
    return impl->height;
}
tb_char_t const* gb_window_title(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, tb_null);

    // the title
    return impl->info.title;
}
gb_bitmap_ref_t gb_window_bitmap(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, tb_null);

    // the bitmap
    return impl->bitmap;
}
gb_float_t gb_window_framerate(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, 0);

    // the framerate
    return impl->framerate;
}
tb_timer_ref_t gb_window_timer(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return_val(impl, tb_null);

    // init timer
    if (!impl->timer) impl->timer = tb_timer_init(GB_WINDOW_TIMER_MAXN, tb_true);
    tb_assert_and_check_return_val(impl->timer, tb_null);

    // the timer
    return impl->timer;
}
