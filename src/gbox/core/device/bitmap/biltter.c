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
 * @file        biltter.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_biltter"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "biltter.h"
#include "biltter/solid.h"
#include "biltter/shader.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_bitmap_biltter_init(gb_bitmap_biltter_ref_t biltter, gb_bitmap_ref_t bitmap, gb_paint_ref_t paint)
{
    // check
    tb_assert(biltter && bitmap && paint);

    // init it
    return gb_paint_shader(paint)? gb_bitmap_biltter_shader_init(biltter, bitmap, paint) : gb_bitmap_biltter_solid_init(biltter, bitmap, paint);
}
tb_void_t gb_bitmap_biltter_exit(gb_bitmap_biltter_ref_t biltter)
{
    // check
    tb_assert(biltter);

    // exit it
    if (biltter->exit) biltter->exit(biltter);
}
tb_void_t gb_bitmap_biltter_done_p(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y)
{   
    // check
    tb_assert(biltter && biltter->done_p);

    // done it
    biltter->done_p(biltter, x, y);
}
tb_void_t gb_bitmap_biltter_done_h(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w)
{   
    // check
    tb_assert(biltter && biltter->done_h);

    // done it
    biltter->done_h(biltter, x, y, w);
}
tb_void_t gb_bitmap_biltter_done_v(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t h)
{   
    // check
    tb_assert(biltter && biltter->done_v);

    // done it
    biltter->done_v(biltter, x, y, h);
}
tb_void_t gb_bitmap_biltter_done_r(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w, tb_long_t h)
{   
    // check
    tb_assert(biltter);

    // horizontal?
    if (h == 1) 
    {
        // check
        tb_assert(biltter->done_h);

        // done it
        biltter->done_h(biltter, x, y, w);
        return ;
    }
    // vertical?
    else if (w == 1)
    {
        // check
        tb_assert(biltter->done_v);

        // done it
        biltter->done_v(biltter, x, y, h);
        return ;
    }
    // rect?
    else if (biltter->done_r) biltter->done_r(biltter, x, y, w, h);
    else
    {
        // check
        tb_assert(biltter->done_h);

        // scan it
        while (h--) biltter->done_h(biltter, x, y++, w);
    }
}
