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

