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
 * @file        convex_maker.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_CONVEX_MAKER_H
#define GB_CORE_IMPL_CONVEX_MAKER_H

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

// the maker for the concave polygon
typedef struct{}*       gb_convex_maker_ref_t;

/* the polygon maker func type
 * 
 * @param points        the points of the convex contour
 * @param count         the point count
 * @param priv          the user private data
 */
typedef tb_void_t       (*gb_convex_maker_func_t)(gb_point_ref_t points, tb_uint16_t count, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init maker
 *
 * @return                  the maker
 */
gb_convex_maker_ref_t     gb_convex_maker_init(tb_noarg_t);

/* exit maker
 *
 * @param maker            the maker
 */
tb_void_t                   gb_convex_maker_exit(gb_convex_maker_ref_t maker);

/* done maker
 *
 * @param maker            the maker
 * @param polygon           the polygon 
 * @param bounds            the bounds of the polygon
 * @param rule              the maker rule
 * @param func              the maker func
 * @param priv              the user private data
 */
tb_void_t                   gb_convex_maker_done(gb_convex_maker_ref_t maker, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_convex_maker_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
