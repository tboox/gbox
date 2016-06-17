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
 * @file        shader.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_biltter_shader"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "shader.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_bitmap_biltter_shader_init(gb_bitmap_biltter_ref_t biltter, gb_bitmap_ref_t bitmap, gb_paint_ref_t paint)
{
    // check
    tb_assert(biltter && bitmap && paint);
 
    // init bitmap
    biltter->bitmap = bitmap;

    // init pixmap
    biltter->pixmap = gb_pixmap(gb_bitmap_pixfmt(bitmap), gb_paint_alpha(paint));
    tb_check_return_val(biltter->pixmap, tb_false);

    // init btp and row_bytes
    biltter->btp        = biltter->pixmap->btp;
    biltter->row_bytes  = gb_bitmap_row_bytes(biltter->bitmap);

    // init shader
    // TODO

    // init operations
    // TODO

    // trace
    tb_trace_noimpl();

    // ok
    return tb_false;
}

