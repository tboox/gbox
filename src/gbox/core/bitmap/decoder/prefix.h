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


