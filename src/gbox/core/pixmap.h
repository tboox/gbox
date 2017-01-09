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
 * @author		ruki
 * @file		pixmap.h
 * @ingroup     core
 */
#ifndef GB_CORE_PIXMAP_H
#define GB_CORE_PIXMAP_H

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

/*! color => pixel
 *
 * @param color         the color 
 *
 * @return              the pixel
 */
typedef gb_pixel_t 		(*gb_pixmap_func_pixel_t)(gb_color_t color);

/*! pixel => color
 *
 * @param pixel         the pixel 
 *
 * @return              the color
 */
typedef gb_color_t 		(*gb_pixmap_func_color_t)(gb_pixel_t pixel);

/*! get pixel from data
 *
 * @param data          the data 
 *
 * @return              the pixel
 */
typedef gb_pixel_t 		(*gb_pixmap_func_pixel_get_t)(tb_cpointer_t data);

/*! set pixel to data
 *
 * @param data          the data 
 * @param pixel         the pixel 
 * @param alpha         the alpha 
 */
typedef tb_void_t 		(*gb_pixmap_func_pixel_set_t)(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha);

/*! copy pixel to data from the source data
 *
 * @param data          the data 
 * @param source        the source data 
 * @param alpha         the alpha 
 */
typedef tb_void_t 		(*gb_pixmap_func_pixel_cpy_t)(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha);

/*! get color from data
 *
 * @param data          the data 
 *
 * @return              the color
 */
typedef gb_color_t 		(*gb_pixmap_func_color_get_t)(tb_cpointer_t data);

/*! set color to data
 *
 * @param data          the data 
 * @param color         the color
 */
typedef tb_void_t 		(*gb_pixmap_func_color_set_t)(tb_pointer_t data, gb_color_t color);

/*! fill pixels to data
 *
 * @param data          the data 
 * @param pixel         the pixel
 * @param count         the count
 */
typedef tb_void_t 		(*gb_pixmap_func_pixels_fill_t)(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha);

/// the pixmap type
typedef struct __gb_pixmap_t
{
	/// the pixel name
	tb_char_t const* 	        name;

	/// the bits per-pixel
	tb_byte_t 			        bpp;

	/// the bytes per-pixel
	tb_byte_t 			        btp;

	/// the pixel format with endian
	tb_uint16_t                 pixfmt;

	/// color => pixel
	gb_pixmap_func_pixel_t        pixel;

	/// pixel => color
	gb_pixmap_func_color_t        color;

	/// get pixel from data
	gb_pixmap_func_pixel_get_t    pixel_get;

    /// set pixel to data
	gb_pixmap_func_pixel_set_t    pixel_set;

    /// copy pixel to data from the source data
	gb_pixmap_func_pixel_cpy_t    pixel_cpy;

    /// get color from data
    gb_pixmap_func_color_get_t    color_get;

    /// set color to data
    gb_pixmap_func_color_set_t    color_set;

    /// fill pixels to data
    gb_pixmap_func_pixels_fill_t  pixels_fill;

}gb_pixmap_t;

/// the pixmap ref type
typedef gb_pixmap_t const*      gb_pixmap_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! get the pixmap from the pixel format 
 *
 * @param pixfmt        the pixfmt with endian
 * @param alpha         the alpha value, do blend-alpha operation if (alpha >= GB_ALPHA_MINN && alpha <= GB_ALPHA_MAXN) 
 *
 * @return              the pixmap
 */
gb_pixmap_ref_t 		gb_pixmap(tb_size_t pixfmt, tb_byte_t alpha);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif

