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
 * @file        quad.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_CUTTER_QUAD_H
#define GB_CORE_IMPL_CUTTER_QUAD_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* done the quad bezier cutter using binary segmentation
 *
 * @param pb        the begin point
 * @param cp        the ctrl point 
 * @param pe        the end point
 * @param func      the cutter func
 * @param priv      the cutter func private data for user
 *
 * <pre>
 *            cp
 *            . 
 *           / \
 *          /   \
 *         /     \
 *        /       \
 *    cpb/----.----\ cpe                  
 *      /     p0    \
 *     /             \
 *    /               \
 *   /                 \
 *  /                   \
 * /                     \
 * pb                    pe
 *
 *
 * (pb, cp, pe) => (pb, cpb, p0) & (p0, cpe, pe)
 *
 * e = |cp - (pb + pe) / 2| <= 1
 *
 * </pre>
 */
tb_void_t           gb_cutter_quad_done(gb_point_ref_t pb, gb_point_ref_t cp, gb_point_ref_t pe, gb_cutter_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


