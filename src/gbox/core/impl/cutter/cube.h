/*!The Graphic 2D Box Library
 * 
 * GBox2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox2; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author      ruki
 * @file        cube.h
 *
 */
#ifndef GB_CORE_CUTTER_CUBE_H
#define GB_CORE_CUTTER_CUBE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* done the cube bezier cutter using binary segmentation
 *
 * @param pb            the begin point
 * @param cpb           the begin ctrl point
 * @param cpe           the end ctrl point
 * @param pe            the end point
 * @param func          the cutter func
 * @param priv          the cutter func private data for user
 *
 * <pre>
 *
 *          cpb      cp0      cpe
 *          --------------------
 *         /                    \
 *        /    pb0---------pe0   \
 *       /           p0           \
 *  cp1 /                          \ cp2
 *     /                            \
 *    /                              \
 *   /                                \
 *  /                                  \
 * /                                    \
 * pb                                   pe
 *
 * e = min(|(cpb - pb) * 2 + cpb - pe|, |(cpe - pe) * 2 + cpe - pb|) <= 1
 *
 * </pre>
 */
tb_void_t               gb_cutter_cube_done(gb_point_ref_t pb, gb_point_ref_t cpb, gb_point_ref_t cpe, gb_point_ref_t pe, gb_cutter_func_t func, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


