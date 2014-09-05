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
#define TB_TRACE_MODULE_DEBUG           (1)

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

    // the button
    tb_size_t               button;

    // the normal width before fullscreen
    tb_uint16_t             normal_width;

    // the normal height before fullscreen
    tb_uint16_t             normal_height;

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
    SDL_Event evet;
    tb_bool_t stop = tb_false;
    while (!stop)
    {
        // draw
        gb_window_impl_draw((gb_window_ref_t)impl, impl->canvas);

        // flip 
        if (SDL_Flip(impl->surface) < 0) stop = tb_true;

        // poll
        while (SDL_PollEvent(&evet))
        {
            // done
            switch (evet.type)
            {
            case SDL_MOUSEMOTION:
                {
                    // init event
                    gb_event_t              event = {0};
                    event.type              = GB_EVENT_TYPE_MOUSE;
                    event.u.mouse.code      = GB_MOUSE_MOVE;
                    event.u.mouse.cursor    = gb_point_imake(evet.motion.x, evet.motion.y);
                    event.u.mouse.button    = impl->button;

                    // done event
                    gb_window_impl_event((gb_window_ref_t)impl, &event);
                }
                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                {
                    // init event
                    gb_event_t              event = {0};
                    event.type              = GB_EVENT_TYPE_MOUSE;
                    event.u.mouse.code      = evet.type == SDL_MOUSEBUTTONDOWN? GB_MOUSE_DOWN : GB_MOUSE_UP;
                    event.u.mouse.cursor    = gb_point_imake(evet.button.x, evet.button.y);

                    // init button
                    switch (evet.button.button)
                    {
                    case SDL_BUTTON_LEFT:   event.u.mouse.button = GB_MOUSE_BUTTON_LEFT;    break;
                    case SDL_BUTTON_RIGHT:  event.u.mouse.button = GB_MOUSE_BUTTON_RIGHT;   break;
                    case SDL_BUTTON_MIDDLE: event.u.mouse.button = GB_MOUSE_BUTTON_MIDDLE;  break;
                    default:                event.u.mouse.button = GB_MOUSE_BUTTON_NONE;    break;
                    }

                    // save button
                    impl->button = evet.type == SDL_MOUSEBUTTONDOWN? event.u.mouse.button : GB_MOUSE_BUTTON_NONE;

                    // done event
                    gb_window_impl_event((gb_window_ref_t)impl, &event);
                }
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                {
                    // init event
                    gb_event_t                  event = {0};
                    event.type                  = GB_EVENT_TYPE_KEYBOARD;
                    event.u.keyboard.pressed    = evet.type == SDL_KEYDOWN? tb_true : tb_false;

                    // init code
                    switch ((tb_size_t)evet.key.keysym.sym)
                    {
                    case SDLK_F1:           event.u.keyboard.code = GB_KEY_F1;          break;
                    case SDLK_F2:           event.u.keyboard.code = GB_KEY_F2;          break;
                    case SDLK_F3:           event.u.keyboard.code = GB_KEY_F3;          break;
                    case SDLK_F4:           event.u.keyboard.code = GB_KEY_F4;          break;
                    case SDLK_F5:           event.u.keyboard.code = GB_KEY_F5;          break;
                    case SDLK_F6:           event.u.keyboard.code = GB_KEY_F6;          break;
                    case SDLK_F7:           event.u.keyboard.code = GB_KEY_F7;          break;
                    case SDLK_F8:           event.u.keyboard.code = GB_KEY_F8;          break;
                    case SDLK_F9:           event.u.keyboard.code = GB_KEY_F9;          break;
                    case SDLK_F10:          event.u.keyboard.code = GB_KEY_F10;         break;
                    case SDLK_F11:          event.u.keyboard.code = GB_KEY_F11;         break;
                    case SDLK_F12:          event.u.keyboard.code = GB_KEY_F12;         break;

                    case SDLK_LEFT:         event.u.keyboard.code = GB_KEY_LEFT;        break;
                    case SDLK_UP:           event.u.keyboard.code = GB_KEY_UP;          break;
                    case SDLK_RIGHT:        event.u.keyboard.code = GB_KEY_RIGHT;       break;
                    case SDLK_DOWN:         event.u.keyboard.code = GB_KEY_DOWN;        break;

                    case SDLK_HOME:         event.u.keyboard.code = GB_KEY_HOME;        break;
                    case SDLK_END:          event.u.keyboard.code = GB_KEY_END;         break;
                    case SDLK_INSERT:       event.u.keyboard.code = GB_KEY_INSERT;      break;
                    case SDLK_PAGEUP:       event.u.keyboard.code = GB_KEY_PAGEUP;      break;
                    case SDLK_PAGEDOWN:     event.u.keyboard.code = GB_KEY_PAGEDOWN;    break;

                    case SDLK_HELP:         event.u.keyboard.code = GB_KEY_HELP;        break;
                    case SDLK_PRINT:        event.u.keyboard.code = GB_KEY_PRINT;       break;
                    case SDLK_SYSREQ:       event.u.keyboard.code = GB_KEY_SYSREQ;      break;
                    case SDLK_BREAK:        event.u.keyboard.code = GB_KEY_BREAK;       break;
                    case SDLK_MENU:         event.u.keyboard.code = GB_KEY_MENU;        break;
                    case SDLK_POWER:        event.u.keyboard.code = GB_KEY_POWER;       break;
                    case SDLK_EURO:         event.u.keyboard.code = GB_KEY_EURO;        break;
                    case SDLK_UNDO:         event.u.keyboard.code = GB_KEY_UNDO;        break;

                    case SDLK_NUMLOCK:      event.u.keyboard.code = GB_KEY_NUMLOCK;     break;
                    case SDLK_CAPSLOCK:     event.u.keyboard.code = GB_KEY_CAPSLOCK;    break;
                    case SDLK_SCROLLOCK:    event.u.keyboard.code = GB_KEY_SCROLLLOCK;  break;
                    case SDLK_RSHIFT:       event.u.keyboard.code = GB_KEY_RSHIFT;      break;
                    case SDLK_LSHIFT:       event.u.keyboard.code = GB_KEY_LSHIFT;      break;
                    case SDLK_RCTRL:        event.u.keyboard.code = GB_KEY_RCTRL;       break;
                    case SDLK_LCTRL:        event.u.keyboard.code = GB_KEY_LCTRL;       break;
                    case SDLK_RALT:         event.u.keyboard.code = GB_KEY_RALT;        break;
                    case SDLK_LALT:         event.u.keyboard.code = GB_KEY_LALT;        break;
                    case 0x136:             event.u.keyboard.code = GB_KEY_RCMD;        break;
                    case 0x135:             event.u.keyboard.code = GB_KEY_LCMD;        break;

                    case SDLK_PAUSE:        event.u.keyboard.code = GB_KEY_PAUSE;       break;

                    default :
                        if (evet.key.keysym.sym < 256)
                        {
                            // the char code
                            event.u.keyboard.code = evet.key.keysym.sym;
                        }
                        break;
                    }

                    // done event
                    if (event.u.keyboard.code) gb_window_impl_event((gb_window_ref_t)impl, &event);
                }
                break;
            case SDL_VIDEORESIZE:
                {
                    // trace
                    tb_trace_d("resize: type: %d, %dx%d", evet.resize.type, evet.resize.w, evet.resize.h);

                    // TODO
                    // ...
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
                tb_trace_e("unknown event: %x", evet.type);
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

    // the pixmap
    gb_pixmap_ref_t pixmap = gb_pixmap(impl->base.pixfmt, 0xff);
    tb_assert_and_check_return(pixmap);

    // fullscreen?
    tb_size_t changed = tb_false;
    if (fullscreen && !(impl->base.flag & GB_WINDOW_FLAG_FULLSCREEN)) 
    {
        // exit surface
        if (impl->surface) SDL_FreeSurface(impl->surface);

        // init mode
        tb_size_t mode = SDL_DOUBLEBUF | SDL_FULLSCREEN;

        // the screen width and height
        tb_uint16_t screen_width  = tb_screen_width();
        tb_uint16_t screen_height = tb_screen_height();
        tb_assert_abort(screen_width && screen_height && screen_width <= GB_WIDTH_MAXN && screen_height <= GB_HEIGHT_MAXN);

        // init surface
        impl->surface = SDL_SetVideoMode(screen_width, screen_height, pixmap->bpp, mode | SDL_HWSURFACE);
        if (!impl->surface) impl->surface = SDL_SetVideoMode(screen_width, screen_height, pixmap->bpp, mode | SDL_SWSURFACE);
        tb_assert_abort(impl->surface && impl->surface->pixels);

        // update flag
        impl->base.flag |= GB_WINDOW_FLAG_FULLSCREEN;

        // save the normal width and height
        impl->normal_width      = (tb_uint16_t)gb_window_width(window);
        impl->normal_height     = (tb_uint16_t)gb_window_height(window);

        // update the window width and height
        impl->base.width        = screen_width;
        impl->base.height       = screen_height;

        // changed
        changed = tb_true;
    }
    else if (impl->base.flag & GB_WINDOW_FLAG_FULLSCREEN)
    {
        // exit surface
        if (impl->surface) SDL_FreeSurface(impl->surface);

        // init mode
        tb_size_t mode = SDL_DOUBLEBUF;
        if (impl->base.flag & GB_WINDOW_FLAG_HIHE_TITLEBAR) mode |= SDL_NOFRAME;
        if (impl->base.flag & GB_WINDOW_FLAG_NOT_REISZE) mode &= ~SDL_RESIZABLE;
        else mode |= SDL_RESIZABLE;

        // init surface
        impl->surface = SDL_SetVideoMode(impl->normal_width, impl->normal_height, pixmap->bpp, mode | SDL_HWSURFACE);
        if (!impl->surface) impl->surface = SDL_SetVideoMode(impl->normal_width, impl->normal_height, pixmap->bpp, mode | SDL_SWSURFACE);
        tb_assert_abort(impl->surface && impl->surface->pixels);

        // update flag
        impl->base.flag &= ~GB_WINDOW_FLAG_FULLSCREEN;

        // update the window width and height
        impl->base.width        = impl->normal_width;
        impl->base.height       = impl->normal_height;

        // changed
        changed = tb_true;
    }

    // ok?
    if (changed)
    {
        // exit canvas first
        if (impl->canvas) gb_canvas_exit(impl->canvas);

        // exit bitmap first
        if (impl->base.bitmap) tb_free(impl->base.bitmap);

        // init bitmap
        impl->base.bitmap = gb_bitmap_init(impl->surface->pixels, impl->base.pixfmt, impl->base.width, impl->base.height, impl->surface->pitch, tb_false);
        tb_assert_abort(impl->base.bitmap);

        // init canvas
        impl->canvas = gb_canvas_init_from_window(window);
        tb_assert_abort(impl->canvas);

        // done resize
        if (impl->base.info.resize) impl->base.info.resize((gb_window_ref_t)impl, impl->canvas, impl->base.info.priv);
    }
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

        // init normal width and height
        impl->normal_width      = (tb_uint16_t)width;
        impl->normal_height     = (tb_uint16_t)height;

        /* init pixfmt
         * 
         * supports: 
         * - xrgb8888_be
         * - rgb565_le
         */
        impl->base.pixfmt       = gb_quality() < GB_QUALITY_TOP? GB_PIXFMT_RGB565 : (GB_PIXFMT_XRGB8888 | GB_PIXFMT_BENDIAN);

        // init sdl
        if (SDL_Init(SDL_INIT_VIDEO) < 0) break;

        // ignore some events
        SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
        SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
        SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

        // the pixmap
        gb_pixmap_ref_t pixmap = gb_pixmap(impl->base.pixfmt, 0xff);
        tb_assert_and_check_break(pixmap);

        // init mode
        tb_size_t mode = SDL_DOUBLEBUF;
        if (flag & GB_WINDOW_FLAG_HIHE_TITLEBAR) mode |= SDL_NOFRAME;
        if (flag & GB_WINDOW_FLAG_FULLSCREEN) mode |= SDL_FULLSCREEN;
        if (flag & GB_WINDOW_FLAG_NOT_REISZE) mode &= ~SDL_RESIZABLE;
        else mode |= SDL_RESIZABLE;

        // fullscreen? update the window size
        if (flag & GB_WINDOW_FLAG_FULLSCREEN)
        {
            impl->base.width    = (tb_uint16_t)tb_screen_width();
            impl->base.height   = (tb_uint16_t)tb_screen_height();
            tb_assert_and_check_break(impl->base.width && impl->base.height && impl->base.width <= GB_WIDTH_MAXN && impl->base.height <= GB_HEIGHT_MAXN);
        }

        // init surface
        impl->surface = SDL_SetVideoMode(impl->base.width, impl->base.height, pixmap->bpp, mode | SDL_HWSURFACE);
        if (!impl->surface) impl->surface = SDL_SetVideoMode(impl->base.width, impl->base.height, pixmap->bpp, mode | SDL_SWSURFACE);
        tb_assert_and_check_break(impl->surface && impl->surface->pixels);

        // init title
        if (impl->base.info.title) SDL_WM_SetCaption(impl->base.info.title, tb_null);

        // init bitmap
        impl->base.bitmap = gb_bitmap_init(impl->surface->pixels, impl->base.pixfmt, impl->base.width, impl->base.height, impl->surface->pitch, tb_false);
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
