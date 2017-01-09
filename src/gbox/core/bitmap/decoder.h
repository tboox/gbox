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
 * @file        decoder.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_BITMAP_DECODER_H
#define GB_CORE_BITMAP_DECODER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the bitmap decoder ref type
typedef struct{}*       gb_bitmap_decoder_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init bitmap decoder 
 *
 * @param pixfmt        the pixfmt, convert bitmap to the given pixfmt
 * @param stream        the stream
 *
 * @return              the decoder
 */
gb_bitmap_decoder_ref_t  gb_bitmap_decoder_init(tb_size_t pixfmt, tb_stream_ref_t stream);

/*! exit bitmap decoder
 *
 * @param decoder       decoder
 */
tb_void_t               gb_bitmap_decoder_exit(gb_bitmap_decoder_ref_t decoder);

/*! done bitmap decoder 
 *
 * @param decoder       decoder 
 *
 * @return              the bitmap
 */
gb_bitmap_ref_t         gb_bitmap_decoder_done(gb_bitmap_decoder_ref_t decoder);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif


