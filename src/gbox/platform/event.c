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
            tb_trace_i("mouse: %s %s, cursor: %{point}, scroll: %{point}", button_cstr[event->u.mouse.button], code_cstr[event->u.mouse.code], &event->u.mouse.cursor, &event->u.mouse.scroll);
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
                if (tb_isgraph(event->u.keyboard.code) && !tb_isspace(event->u.keyboard.code)) 
                    tb_snprintf(cstr, sizeof(cstr) - 1, "%c", event->u.keyboard.code);
                else 
                {
                    tb_char_t const* code_cstr = tb_null;
                    switch (event->u.keyboard.code)
                    {
                    case GB_KEY_NUL:        code_cstr = "nul";          break;
                    case GB_KEY_BACKSPACE:  code_cstr = "backspace";    break;
                    case GB_KEY_TAB:        code_cstr = "tab";          break;
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
                ,   "pageup"
                ,   "pagedown"

                ,   "help"
                ,   "print"
                ,   "sysreq"
                ,   "break"
                ,   "menu"
                ,   "power"
                ,   "euro"
                ,   "undo"

                ,   "numlock"
                ,   "capslock"
                ,   "scrolllock"
                ,   "rshift"
                ,   "lshift"
                ,   "rctrl"
                ,   "lctrl"
                ,   "ralt"
                ,   "lalt"
                ,   "rcmd"
                ,   "lcmd"

                ,   "pause"
                ,   "search"
                ,   "tabback"
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
    case GB_EVENT_TYPE_ACTIVE:
        {
            // the code strings
            tb_char_t const* code_cstr[] =
            {
                "none"
            ,   "background"
            ,   "foreground"
            };
            tb_assert_and_check_break(event->u.active.code < tb_arrayn(code_cstr));

            // trace
            tb_trace_i("active: %s", code_cstr[event->u.active.code]);
        }
        break;
    default:
        // trace
        tb_trace_e("invalid type: %u", event->type);
        break;
    }
}
#endif

