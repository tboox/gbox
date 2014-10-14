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
 * @file        raster.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_DEVICE_GL_RASTER_H
#define GB_CORE_DEVICE_GL_RASTER_H

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

// the gl raster for the concave polygon
typedef struct{}*   gb_gl_raster_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init raster
 *
 * @return          the raster
 */
gb_gl_raster_ref_t  gb_gl_raster_init(tb_noarg_t);

/* exit raster
 *
 * @param raster    the raster
 */
tb_void_t           gb_gl_raster_exit(gb_gl_raster_ref_t raster);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
