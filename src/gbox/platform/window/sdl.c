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
 * @file        sdl.c
 * @ingroup     platform
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "window_sdl"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <SDL/SDL.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the sdl window impl type
typedef struct __gb_window_sdl_impl_t
{
    // the base
    gb_window_impl_t        base;

    // the canvas
    gb_canvas_ref_t         canvas;

    // the surface
    SDL_Surface*            surface;

}gb_window_sdl_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_window_sdl_exit(gb_window_ref_t window)
{
    // check
    gb_window_sdl_impl_t* impl = (gb_window_sdl_impl_t*)window;
    tb_assert_and_check_return(impl);

    // exit bitmap
    if (impl->base.bitmap) gb_bitmap_exit(impl->base.bitmap);
    impl->base.bitmap = tb_null;

    // exit canvas
    if (impl->canvas) gb_canvas_exit(impl->canvas);
    impl->canvas = tb_null;

    // exit surface
    if (impl->surface) SDL_FreeSurface(impl->surface);
    impl->surface = tb_null;

    // exit it
    tb_free(window);

    // exit sdl
    SDL_Quit();
}
static tb_void_t gb_window_sdl_loop(gb_window_ref_t window)
{
    // check
    gb_window_sdl_impl_t* impl = (gb_window_sdl_impl_t*)window;
    tb_assert_and_check_return(impl);

    // init canvas
    if (!impl->canvas) impl->canvas = gb_canvas_init_from_window(window);
    tb_assert_abort(impl->canvas);

    // done init
    if (impl->base.info.init) impl->base.info.init((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);

    // loop
    SDL_Event event;
    tb_bool_t stop = tb_false;
    while (!stop)
    {
        // draw
        gb_window_impl_draw((gb_window_ref_t)impl, impl->canvas);

        // flip 
        if (SDL_Flip(impl->surface) < 0) stop = tb_true;

        // poll
        while (SDL_PollEvent(&event))
        {
            // done
            switch (event.type)
            {
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_DELETE:
                        break;
                    case SDLK_TAB:
                        break;
                    case SDLK_BACKSPACE:
                        break;
                    case SDLK_RETURN:
                        break;
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_SPACE:
                        break;
                    default :
                        if (event.key.keysym.sym > 0 && event.key.keysym.sym < 128)
                        {
                        }
                        break;
                    }
                }
                break;
            case SDL_QUIT:
                {
                    // stop it
                    stop = tb_true;
                }
                break;
            default:
                // trace
                tb_trace_e("unknown event: %x", event.type);
                break;
            }
        }

        // FIXME
        // wait 
        SDL_Delay(1000 / impl->base.info.framerate);
    }
 
    // done exit
    if (impl->base.info.exit) impl->base.info.exit((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);
}
static tb_void_t gb_window_sdl_fullscreen(gb_window_ref_t window, tb_bool_t fullscreen)
{
    // check
    gb_window_sdl_impl_t* impl = (gb_window_sdl_impl_t*)window;
    tb_assert_and_check_return(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_window_ref_t gb_window_init_sdl(gb_window_info_t const* info, tb_size_t width, tb_size_t height, tb_size_t flag)
{
    // done
    tb_bool_t               ok = tb_false;
    gb_window_sdl_impl_t*   impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(info && info->framerate);
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);

        // make window
        impl = tb_malloc0_type(gb_window_sdl_impl_t);
        tb_assert_and_check_break(impl);

        // init base
        impl->base.type         = GB_WINDOW_TYPE_SDL;
        impl->base.mode         = GB_WINDOW_MODE_BITMAP;
        impl->base.flag         = flag;
        impl->base.width        = (tb_uint16_t)width;
        impl->base.height       = (tb_uint16_t)height;
        impl->base.loop         = gb_window_sdl_loop;
        impl->base.exit         = gb_window_sdl_exit;
        impl->base.fullscreen   = gb_window_sdl_fullscreen;
        impl->base.info         = *info;

        /* init pixfmt
         * 
         * supports: 
         * - xrgb8888_be
         * - rgb565_le
         */
        impl->base.pixfmt       = gb_quality() < GB_QUALITY_TOP? GB_PIXFMT_RGB565 : (GB_PIXFMT_XRGB8888 | GB_PIXFMT_BENDIAN);

#ifdef TB_CONFIG_OS_MAC
        // load cocoa framwork
        tb_dynamic_ref_t cocoa = tb_dynamic_init("/System/Library/Frameworks/Cocoa.framework/Cocoa");
        if (cocoa)
        {
            // get NSApplicationLoad func
            tb_void_t (*pNSApplicationLoad)() = (tb_void_t(*)())tb_dynamic_func(cocoa, "NSApplicationLoad");
            tb_assert_and_check_break(pNSApplicationLoad);

            // load application before calling SDL_SetVideoMode
            pNSApplicationLoad();
        }
#endif

        // init sdl
        if (SDL_Init(SDL_INIT_VIDEO) < 0) break;

        // the pixmap
        gb_pixmap_ref_t pixmap = gb_pixmap(impl->base.pixfmt, 0xff);
        tb_assert_and_check_break(pixmap);

        // init mode
        tb_size_t mode = SDL_DOUBLEBUF;
        if (flag & GB_WINDOW_FLAG_HIHE_TITLEBAR) mode |= SDL_NOFRAME;
        if (flag & GB_WINDOW_FLAG_FULLSCREEN) mode |= SDL_FULLSCREEN;
        if (flag & GB_WINDOW_FLAG_NOT_REISZE) mode &= ~SDL_RESIZABLE;
        else mode |= SDL_RESIZABLE;

        // init surface
        impl->surface = SDL_SetVideoMode(width, height, pixmap->bpp, mode | SDL_HWSURFACE);
        if (!impl->surface) impl->surface = SDL_SetVideoMode(width, height, pixmap->bpp, mode | SDL_SWSURFACE);
        tb_assert_and_check_break(impl->surface && impl->surface->pixels);

        // init title
        if (impl->base.info.title) SDL_WM_SetCaption(impl->base.info.title, tb_null);

        // init bitmap
        impl->base.bitmap = gb_bitmap_init(impl->surface->pixels, impl->base.pixfmt, width, height, impl->surface->pitch, tb_false);
        tb_assert_and_check_break(impl->base.bitmap);

        // check: not implementation
        tb_assert(!(flag & GB_WINDOW_FLAG_MAXIMUM));
        tb_assert(!(flag & GB_WINDOW_FLAG_MINIMUM));
        tb_assert(!(flag & GB_WINDOW_FLAG_HIHE));

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_window_exit((gb_window_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_window_ref_t)impl;
}
