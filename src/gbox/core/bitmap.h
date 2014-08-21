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
 * @file        bitmap.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_BITMAP_H
#define GB_CORE_BITMAP_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! init bitmap 
 *
 * @param data      the data, will auto make data if be null
 * @param pixfmt    the pixfmt 
 * @param width     the width 
 * @param height    the height 
 * @param row_bytes the row bytes, will auto calculate it using width if be zero
 * @param has_alpha has alpha?
 *
 * @return          the bitmap
 */
gb_bitmap_ref_t     gb_bitmap_init(tb_pointer_t data, tb_size_t pixfmt, tb_size_t width, tb_size_t height, tb_size_t row_bytes, tb_bool_t has_alpha);

/*! init bitmap from url
 *
 * @param pixfmt    the pixfmt 
 * @param url       the bitmap url
 *
 * @return          the bitmap
 */
gb_bitmap_ref_t     gb_bitmap_init_from_url(tb_size_t pixfmt, tb_char_t const* url);

/*! init bitmap from stream
 *
 * @param pixfmt    the pixfmt 
 * @param stream    the bitmap stream
 *
 * @return          the bitmap
 */
gb_bitmap_ref_t     gb_bitmap_init_from_stream(tb_size_t pixfmt, tb_stream_ref_t stream);

/*! exit bitmap 
 *
 * @param bitmap    the bitmap
 */
tb_void_t           gb_bitmap_exit(gb_bitmap_ref_t bitmap);

/*! the bitmap data size
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap data size
 */
tb_size_t           gb_bitmap_size(gb_bitmap_ref_t bitmap);

/*! the bitmap data
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap data 
 */
tb_pointer_t        gb_bitmap_data(gb_bitmap_ref_t bitmap);

/*! set the bitmap data
 *
 * @param bitmap    the bitmap
 * @param data      the bitmap data, cannot be null
 * @param pixfmt    the pixfmt 
 * @param width     the width 
 * @param height    the height 
 * @param row_bytes the row bytes, will auto calculate it using width if be zero
 * @param has_alpha has alpha?
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_bitmap_data_set(gb_bitmap_ref_t bitmap, tb_pointer_t data, tb_size_t pixfmt, tb_size_t width, tb_size_t height, tb_size_t row_bytes, tb_bool_t has_alpha);

/*! resize the bitmap
 *
 * @param bitmap    the bitmap
 * @param width     the width 
 * @param height    the height 
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_bitmap_resize(gb_bitmap_ref_t bitmap, tb_size_t width, tb_size_t height);

/*! the bitmap width
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap width 
 */
tb_size_t           gb_bitmap_width(gb_bitmap_ref_t bitmap);

/*! the bitmap height
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap height 
 */
tb_size_t           gb_bitmap_height(gb_bitmap_ref_t bitmap);

/*! the bitmap pixfmt
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap pixfmt 
 */
tb_size_t           gb_bitmap_pixfmt(gb_bitmap_ref_t bitmap);

/*! the bitmap has alpha data?
 *
 * @param bitmap    the bitmap
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_bitmap_has_alpha(gb_bitmap_ref_t bitmap);

/*! set the bitmap alpha: exists or not?
 *
 * @param bitmap    the bitmap
 * @param has_alpha has alpha value?
 */
tb_void_t           gb_bitmap_set_alpha(gb_bitmap_ref_t bitmap, tb_bool_t has_alpha);

/*! the bitmap row bytes
 *
 * @param bitmap    the bitmap
 *
 * @return          the bitmap row bytes 
 */
tb_size_t           gb_bitmap_row_bytes(gb_bitmap_ref_t bitmap);

__tb_extern_c_leave__
#endif
