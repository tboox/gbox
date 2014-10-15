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
 * @file        raster.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_raster"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl raster impl type
typedef struct __gb_gl_raster_impl_t
{

}gb_gl_raster_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_gl_raster_ref_t gb_gl_raster_init()
{
    return tb_null;
}
tb_void_t gb_gl_raster_exit(gb_gl_raster_ref_t raster)
{
}
tb_void_t gb_gl_raster_done(gb_gl_raster_ref_t raster, gb_polygon_ref_t polygon, gb_gl_raster_func_t func, tb_cpointer_t priv)
{
}
