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
 * @file        polygon_raster.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_POLYGON_RASTER_H
#define GB_CORE_IMPL_POLYGON_RASTER_H

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

// the polygon raster rule enum
typedef enum __gb_polygon_raster_rule_e
{
    GB_POLYGON_RASTER_RULE_ODD      = GB_PAINT_FILL_RULE_ODD     //< odd 
,   GB_POLYGON_RASTER_RULE_NONZERO  = GB_PAINT_FILL_RULE_NONZERO //< non-zero 

}gb_polygon_raster_rule_e;

// the polygon raster ref type
typedef struct{}*       gb_polygon_raster_ref_t;

/* the polygon raster func type
 *
 * @param lx            the left x-coordinate
 * @param rx            the right x-coordinate 
 * @param yb            the top y-coordinate
 * @param ye            the bottom y-coordinate 
 * @param priv          the private data
 */
typedef tb_void_t       (*gb_polygon_raster_func_t)(tb_long_t lx, tb_long_t rx, tb_long_t yb, tb_long_t ye, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init raster
 *
 * @return              the raster
 */
gb_polygon_raster_ref_t gb_polygon_raster_init(tb_noarg_t);

/* exit raster
 *
 * @param raster        the raster
 */
tb_void_t               gb_polygon_raster_exit(gb_polygon_raster_ref_t raster);

/* done raster
 *
 * @param raster        the raster
 * @param polygon       the polygon
 * @param bounds        the bounds
 * @param rule          the raster rule
 * @param func          the raster func
 * @param priv          the private data
 */
tb_void_t               gb_polygon_raster_done(gb_polygon_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_polygon_raster_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


