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
    tb_assert_abort(biltter && bitmap && paint);

    // init it
    return gb_paint_shader(paint)? gb_bitmap_biltter_shader_init(biltter, bitmap, paint) : gb_bitmap_biltter_solid_init(biltter, bitmap, paint);
}
tb_void_t gb_bitmap_biltter_exit(gb_bitmap_biltter_ref_t biltter)
{
    // check
    tb_assert_abort(biltter);

    // exit it
    if (biltter->exit) biltter->exit(biltter);
}
tb_void_t gb_bitmap_biltter_done_h(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w)
{   
    // check
    tb_assert_abort(biltter && biltter->done_h);

    // done it
    biltter->done_h(biltter, x, y, w);
}
tb_void_t gb_bitmap_biltter_done_v(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t h)
{   
    // check
    tb_assert_abort(biltter && biltter->done_v);

    // done it
    biltter->done_v(biltter, x, y, h);
}
tb_void_t gb_bitmap_biltter_done_r(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w, tb_long_t h)
{   
    // check
    tb_assert_abort(biltter);

    // done it
    if (biltter->done_r) biltter->done_r(biltter, x, y, w, h);
    else
    {
        // check
        tb_assert_abort(biltter->done_h);

        // scan it
        while (h--) biltter->done_h(biltter, x, y++, w);
    }
}
