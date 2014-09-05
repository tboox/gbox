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
__tb_extern_c__ gb_window_ref_t gb_window_init_ios(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag);
__tb_extern_c__ gb_window_ref_t gb_window_init_mac(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag);
__tb_extern_c__ gb_window_ref_t gb_window_init_windows(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag);
__tb_extern_c__ gb_window_ref_t gb_window_init_android(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag);

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag)
{
#if defined(TB_CONFIG_OS_IOS)
    return gb_window_init_ios(info, width, height, flag);
#elif defined(TB_CONFIG_OS_ANDROID)
    return gb_window_init_android(info, width, height, flag);
#elif 0//defined(TB_CONFIG_OS_MAC)
    return gb_window_init_mac(info, width, height, flag);
#elif 0//defined(TB_CONFIG_OS_WINDOWS)
    return gb_window_init_windows(info, width, height, flag);
#elif defined(GB_CONFIG_THIRD_HAVE_X11)
    return gb_window_init_x11(info, width, height, flag);
#elif defined(GB_CONFIG_THIRD_HAVE_GLUT)
    return gb_window_init_glut(info, width, height, flag);
#elif defined(GB_CONFIG_THIRD_HAVE_SDL)
    return gb_window_init_sdl(info, width, height, flag);
#elif defined(GB_CONFIG_THIRD_HAVE_FRAMEBUFFER)
    return gb_window_init_framebuffer(info, width, height, flag);
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
