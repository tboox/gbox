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
 * @file        event.c
 * @ingroup     platform
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "event"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "event.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
#ifdef __gb_debug__
tb_void_t gb_event_dump(gb_event_ref_t event)
{
    // check
    tb_assert_and_check_return(event);

    // done
    switch (event->type)
    {
    case GB_EVENT_TYPE_MOUSE:
        {
            // the code strings
            tb_char_t const* code_cstr[] =
            {
                "none"
            ,   "down"
            ,   "up"
            ,   "move"
            ,   "scroll"
            };
            tb_assert_and_check_break(event->u.mouse.code < tb_arrayn(code_cstr));

            // the button strings
            tb_char_t const* button_cstr[] =
            {
                "none"
            ,   "left"
            ,   "right"
            ,   "middle"
            };
            tb_assert_and_check_break(event->u.mouse.button < tb_arrayn(button_cstr));

            // trace
            tb_trace_i("mouse: %s %s, cursor: " GB_FMT ", " GB_FMT ", scroll: " GB_FMT ", " GB_FMT, button_cstr[event->u.mouse.button], code_cstr[event->u.mouse.code], event->u.mouse.cursor.x, event->u.mouse.cursor.y, event->u.mouse.scroll.x, event->u.mouse.scroll.y);
        }
        break;
    case GB_EVENT_TYPE_KEYBOARD:
        {
            // the cstr
            tb_char_t cstr[64] = {0};

            // the char
            if (event->u.keyboard.code < 256) 
            {
                // format
                if (tb_isgraph(event->u.keyboard.code)) 
                    tb_snprintf(cstr, sizeof(cstr) - 1, "%c", event->u.keyboard.code);
                else 
                {
                    tb_char_t const* code_cstr = tb_null;
                    switch (event->u.keyboard.code)
                    {
                    case GB_KEY_NUL:        code_cstr = "nul";          break;
                    case GB_KEY_BACKSPACE:  code_cstr = "backspace";    break;
                    case GB_KEY_TAB:        code_cstr = "tab";          break;
                    case GB_KEY_TAB_BACK:   code_cstr = "tab_back";     break;
                    case GB_KEY_RETURN:     code_cstr = "return";       break;
                    case GB_KEY_ESCAPE:     code_cstr = "escape";       break;
                    case GB_KEY_SPACE:      code_cstr = "space";        break;
                    case GB_KEY_DELETE:     code_cstr = "delete";       break;
                    }

                    if (code_cstr) tb_snprintf(cstr, sizeof(cstr) - 1, "%s", code_cstr);
                    else tb_snprintf(cstr, sizeof(cstr) - 1, "%#x", event->u.keyboard.code);
                }
            }
            else
            {
                // the code
                tb_size_t code = event->u.keyboard.code - 256;

                // the code strings
                tb_char_t const* code_cstr[] =
                {
                    "f1"
                ,   "f2"
                ,   "f3"
                ,   "f4"
                ,   "f5"
                ,   "f6"
                ,   "f7"
                ,   "f8"
                ,   "f9"
                ,   "f10"
                ,   "f11"
                ,   "f12"

                ,   "left"
                ,   "up"
                ,   "right"
                ,   "down"

                ,   "home"
                ,   "end"
                ,   "insert"
                ,   "page_up"
                ,   "page_down"

                ,   "print"
                ,   "sysreq"
                ,   "scroll_lock"
                ,   "pause"
                ,   "break"

                ,   "caps_lock"
                ,   "shift"
                ,   "ctrl"
                ,   "alt"

                ,   "menu"
                ,   "play"
                ,   "search"
                };
                tb_assert_and_check_break(code < tb_arrayn(code_cstr));

                // format
                tb_snprintf(cstr, sizeof(cstr) - 1, "%s", code_cstr[code]);
            }

            // trace
            tb_trace_i("key: %s, pressed: %u", cstr, event->u.keyboard.pressed);
        }
        break;
    case GB_EVENT_TYPE_TOUCH:
        {
            // the code strings
            tb_char_t const* code_cstr[] =
            {
                "none"
            ,   "began"
            ,   "moved"
            ,   "ended"
            ,   "canceled"
            };
            tb_assert_and_check_break(event->u.touch.code < tb_arrayn(code_cstr));

            // trace
            tb_trace_i("touch: %s, count: %u", code_cstr[event->u.touch.code], event->u.touch.count);
        }
        break;
    default:
        // trace
        tb_trace_e("invalid type: %u", event->type);
        break;
    }
}
#endif

