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
 * @file        prefix.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_BITMAP_DECODER_PREFIX_H
#define GB_CORE_BITMAP_DECODER_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"
#include "../decoder.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the bitmap decoder impl type
typedef struct __gb_bitmap_decoder_impl_t
{
    // the bitmap type
    tb_uint8_t      type;

    // the pixfmt
    tb_uint16_t     pixfmt;

    // the width
    tb_uint16_t     width;

    // the height
    tb_uint16_t     height;

    // the stream
    tb_stream_ref_t stream;

    // done
    gb_bitmap_ref_t (*done)(struct __gb_bitmap_decoder_impl_t* decoder);

    // free
    tb_void_t       (*exit)(struct __gb_bitmap_decoder_impl_t* decoder);

}gb_bitmap_decoder_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
/* probe bmp bitmap foramt
 *
 * @param stream        the stream
 *
 * @return              the score: [0, 100]
 */
tb_size_t               gb_bitmap_decoder_bmp_probe(tb_stream_ref_t stream);

/* init bmp bitmap decoder
 *
 * @param pixfmt        the pixfmt
 * @param stream        the stream
 *
 * @return              the decoder
 */
gb_bitmap_decoder_ref_t  gb_bitmap_decoder_bmp_init(tb_size_t pixfmt, tb_stream_ref_t stream);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif


