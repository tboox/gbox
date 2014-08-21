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
 * @file        view.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_GL_VIEW_H
#define GB_CORE_GL_VIEW_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the gl view type
typedef struct{}*       gb_gl_view_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

#if defined(GB_CONFIG_THIRD_HAVE_GLUT)
/*! init glut view 
 *
 * init and create a new view window
 *
 * @param name          the view name
 * @param pixfmt        the view pixfmt, supports: 
 *                      - rgba8888_be
 *                      - rgb565_le
 *                      - rgb888_be
 *                      - rgba4444_be
 *                      - rgba5551_le
 * @param width         the view width 
 * @param height        the view height
 *
 * @return              the view
 */
gb_gl_view_ref_t        gb_gl_view_init(tb_char_t const* name, tb_size_t pixfmt, tb_size_t width, tb_size_t height);

#elif defined(TB_CONFIG_OS_IOS)
/*! init ios view 
 *
 * @param name          the view name
 *
 * @return              the view
 */
gb_gl_view_ref_t        gb_gl_view_init(tb_char_t const* name);
#elif defined(TB_CONFIG_OS_ANDROID)
/*! init android view 
 *
 * @param name          the view name
 *
 * @return              the view
 */
gb_gl_view_ref_t        gb_gl_view_init(tb_char_t const* name);
#else
#   error no gl view
#endif

/*! exit view 
 *
 * @param view          the view
 */
tb_void_t               gb_gl_view_exit(gb_gl_view_ref_t view);

/*! the pixfmt
 *
 * @param view          the view
 *
 * @return              the pixfmt
 */
tb_size_t               gb_gl_view_pixfmt(gb_gl_view_ref_t view);

/*! the width
 *
 * @param view          the view
 *
 * @return              the width
 */
tb_size_t               gb_gl_view_width(gb_gl_view_ref_t view);

/*! the height
 *
 * @param view          the view
 *
 * @return              the height
 */
tb_size_t               gb_gl_view_height(gb_gl_view_ref_t view);

__tb_extern_c_leave__
#endif
