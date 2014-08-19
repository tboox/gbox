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

// the pixel operation type
typedef gb_pixel_t 		(*gb_pixmap_op_pixel_t)(gb_color_t color);
typedef gb_color_t 		(*gb_pixmap_op_color_t)(gb_pixel_t pixel);

typedef gb_pixel_t 		(*gb_pixmap_op_pixel_get_t)(tb_cpointer_t data);
typedef tb_void_t 		(*gb_pixmap_op_pixel_set_t)(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha);
typedef tb_void_t 		(*gb_pixmap_op_pixel_cpy_t)(tb_pointer_t data, tb_cpointer_t pixel, tb_byte_t alpha);

typedef gb_color_t 		(*gb_pixmap_op_color_get_t)(tb_cpointer_t data);
typedef tb_void_t 		(*gb_pixmap_op_color_set_t)(tb_pointer_t data, gb_color_t pixel);

typedef tb_void_t 		(*gb_pixmap_op_pixels_set_t)(tb_pointer_t data, gb_pixel_t pixel, tb_size_t src_n, tb_byte_t alpha);

// the pixel map type
typedef struct __gb_pixmap_t
{
	// the pixel name
	tb_char_t const* 	name;

	// the bits per pixel
	tb_size_t 			bpp 	: 8;

	// the bytes per pixel
	tb_size_t 			btp 	: 8;

	// the pixel format
	tb_size_t 			pixfmt 	: 16;

	// color => pixel
	gb_pixel_t 			(*pixel)(gb_color_t color);
	// pixel => color
	gb_color_t 			(*color)(gb_pixel_t pixel);

	// pixel
	gb_pixel_t 			(*pixel_get)(tb_cpointer_t data);
	tb_void_t 			(*pixel_set)(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha);
	tb_void_t 			(*pixel_cpy)(tb_pointer_t data, tb_cpointer_t pixel, tb_byte_t alpha);

	// color
	gb_color_t 			(*color_get)(tb_cpointer_t data);
	tb_void_t 			(*color_set)(tb_pointer_t data, gb_color_t color);

	// pixels
	tb_void_t 			(*pixels_set)(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha);

}gb_pixmap_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

// the pixmap from the pixel format 
gb_pixmap_t const* 		gb_pixmap(tb_size_t pixfmt, tb_byte_t alpha);


#endif

