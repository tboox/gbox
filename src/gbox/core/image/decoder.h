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
 * @file        decoder.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMAGE_DECODER_H
#define GB_CORE_IMAGE_DECODER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the image decoder ref type
typedef struct{}*       gb_image_decoder_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! init image decoder 
 *
 * @param pixfmt        the pixfmt, convert image to the given pixfmt
 * @param stream        the stream
 *
 * @return              the decoder
 */
gb_image_decoder_ref_t  gb_image_decoder_init(tb_size_t pixfmt, tb_stream_ref_t stream);

/*! exit image decoder
 *
 * @param decoder       decoder
 */
tb_void_t               gb_image_decoder_exit(gb_image_decoder_ref_t decoder);

/*! done image decoder 
 *
 * @param decoder       decoder 
 *
 * @return              the bitmap
 */
gb_bitmap_ref_t         gb_image_decoder_done(gb_image_decoder_ref_t decoder);

__tb_extern_c_leave__
#endif


