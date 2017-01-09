/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        rgb24.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGB24_H
#define GB_CORE_PIXMAP_RGB24_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgb24_pixel_get_l(tb_cpointer_t data)
{
    return tb_bits_get_u24_le(data);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb24_pixel_get_b(tb_cpointer_t data)
{
    return tb_bits_get_u24_be(data);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb24_pixel_set_lo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u24_le(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb24_pixel_set_bo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u24_be(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb24_pixel_cpy_o(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    ((tb_byte_t*)data)[0] = ((tb_byte_t const*)source)[0];
    ((tb_byte_t*)data)[1] = ((tb_byte_t const*)source)[1];
    ((tb_byte_t*)data)[2] = ((tb_byte_t const*)source)[2];
}
static __tb_inline__ tb_void_t gb_pixmap_rgb24_pixels_fill_lo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u24(data, tb_bits_ne_to_le_u24(pixel), count);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb24_pixels_fill_bo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u24(data, tb_bits_ne_to_be_u24(pixel), count);
}

#endif

