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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
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
 * types
 */

/*! color => pixel
 *
 * @param color         the color 
 *
 * @return              the pixel
 */
typedef gb_pixel_t 		(*gb_pixmap_op_pixel_t)(gb_color_t color);

/*! pixel => color
 *
 * @param pixel         the pixel 
 *
 * @return              the color
 */
typedef gb_color_t 		(*gb_pixmap_op_color_t)(gb_pixel_t pixel);

/*! get pixel from data
 *
 * @param data          the data 
 *
 * @return              the pixel
 */
typedef gb_pixel_t 		(*gb_pixmap_op_pixel_get_t)(tb_cpointer_t data);

/*! set pixel to data
 *
 * @param data          the data 
 * @param pixel         the pixel 
 * @param alpha         the alpha 
 */
typedef tb_void_t 		(*gb_pixmap_op_pixel_set_t)(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha);

/*! copy pixel to data from the source data
 *
 * @param data          the data 
 * @param source        the source data 
 * @param alpha         the alpha 
 */
typedef tb_void_t 		(*gb_pixmap_op_pixel_cpy_t)(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha);

/*! get color from data
 *
 * @param data          the data 
 *
 * @return              the color
 */
typedef gb_color_t 		(*gb_pixmap_op_color_get_t)(tb_cpointer_t data);

/*! set color to data
 *
 * @param data          the data 
 * @param color         the color
 */
typedef tb_void_t 		(*gb_pixmap_op_color_set_t)(tb_pointer_t data, gb_color_t color);

/*! fill pixels to data
 *
 * @param data          the data 
 * @param pixel         the pixel
 * @param count         the count
 */
typedef tb_void_t 		(*gb_pixmap_op_pixels_fill_t)(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha);

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
	gb_pixmap_op_pixel_t        pixel;

	/// pixel => color
	gb_pixmap_op_color_t        color;

	/// get pixel from data
	gb_pixmap_op_pixel_get_t    pixel_get;

    /// set pixel to data
	gb_pixmap_op_pixel_set_t    pixel_set;

    /// copy pixel to data from the source data
	gb_pixmap_op_pixel_cpy_t    pixel_cpy;

    /// get color from data
    gb_pixmap_op_color_get_t    color_get;

    /// set color to data
    gb_pixmap_op_color_set_t    color_set;

    /// fill pixels to data
    gb_pixmap_op_pixels_fill_t  pixels_fill;

}gb_pixmap_t;

/// the pixmap ref type
typedef gb_pixmap_t const*      gb_pixmap_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! get the pixmap from the pixel format 
 *
 * @param pixfmt        the pixfmt with endian
 * @param alpha         the alpha value
 * @param quality       the quality
 *
 * @return              the pixmap
 */
gb_pixmap_ref_t 		gb_pixmap(tb_size_t pixfmt, tb_byte_t alpha, tb_size_t quality);

__tb_extern_c_leave__
#endif

