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
#ifndef GB_CORE_IMPL_RASTER_H
#define GB_CORE_IMPL_RASTER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../paint.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the raster rule enum
typedef enum __gb_raster_rule_e
{
    GB_RASTER_RULE_ODD      = GB_PAINT_FILL_RULE_ODD     //< odd 
,   GB_RASTER_RULE_NONZERO  = GB_PAINT_FILL_RULE_NONZERO //< non-zero 

}gb_raster_rule_e;

// the raster ref type
typedef struct{}*       gb_raster_ref_t;

/* the raster func type
 *
 * @param xb            the start x-coordinate
 * @param xe            the end x-coordinate 
 * @param yb            the start y-coordinate
 * @param ye            the end y-coordinate 
 * @param priv          the private data
 */
typedef tb_void_t       (*gb_raster_func_t)(tb_long_t xb, tb_long_t xe, tb_long_t yb, tb_long_t ye, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init raster
 *
 * @return              the raster
 */
gb_raster_ref_t         gb_raster_init(tb_noarg_t);

/* exit raster
 *
 * @param raster        the raster
 */
tb_void_t               gb_raster_exit(gb_raster_ref_t raster);

/* done raster
 *
 * @param raster        the raster
 * @param polygon       the polygon
 * @param bounds        the bounds
 * @param rule          the raster rule
 * @param func          the raster func
 * @param priv          the private data
 */
tb_void_t               gb_raster_done(gb_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_raster_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


