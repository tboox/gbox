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
#include "window/prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_window_exit(gb_window_ref_t window)
{
    // check
    gb_window_impl_t* impl = (gb_window_impl_t*)window;
    tb_assert_and_check_return(impl);

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

