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
 * @file        rgb16.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGB16_H
#define GB_CORE_PIXMAP_RGB16_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgb16_pixel_get_l(tb_cpointer_t data)
{
    return tb_bits_get_u16_le(data);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb16_pixel_get_b(tb_cpointer_t data)
{
    return tb_bits_get_u16_be(data);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb16_pixel_set_lo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb16_pixel_set_bo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb16_pixel_cpy_o(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    *((tb_uint16_t*)data) = *((tb_uint16_t*)source);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb16_pixels_fill_lo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u16(data, tb_bits_ne_to_le_u16(pixel), count);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb16_pixels_fill_bo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u16(data, tb_bits_ne_to_be_u16(pixel), count);
}


#endif
