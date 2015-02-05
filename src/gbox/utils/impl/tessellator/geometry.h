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
 * @author      ruki
 * @file        geometry.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_GEOMETRY_H
#define GB_UTILS_IMPL_TESSELLATOR_GEOMETRY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// vertex: a == b?
#define gb_tessellator_vertex_eq(a, b)  \
    (   gb_tessellator_vertex_point(a)->x == gb_tessellator_vertex_point(b)->x \
    &&  gb_tessellator_vertex_point(a)->y == gb_tessellator_vertex_point(b)->y)

/* vertex: a <= b?
 *
 * sweep direction: horizontal
 *
 * v0 -------> v1-----                  
 * ---> v2 -------> v3                  
 * v4 ----------------                  
 * --------> v5 ------                  
 *
 * v0 < v1 < v2 < v3 < v4 < v5
 */
#define gb_tessellator_vertex_le_h(a, b)  \
    (   gb_tessellator_vertex_point(a)->y < gb_tessellator_vertex_point(b)->y \
    ||  (   gb_tessellator_vertex_point(a)->y == gb_tessellator_vertex_point(b)->y \
        &&  gb_tessellator_vertex_point(a)->x <= gb_tessellator_vertex_point(b)->x))
   
/* vertex: a <= b? 
 *
 * sweep direction: vertical
 *
 *  v0    |     v4    |
 *  |    \ /    |     |
 *  |     v2    |     |
 * \ /    |     |    \ /
 *  v1    |     |     v5
 *  |    \ /    |     |
 *  |     v3    |     |
 *
 * v0 < v1 < v2 < v3 < v4 < v5
 */
#define gb_tessellator_vertex_le_v(a, b)  \
    (   gb_tessellator_vertex_point(a)->x < gb_tessellator_vertex_point(b)->x \
    ||  (   gb_tessellator_vertex_point(a)->x == gb_tessellator_vertex_point(b)->x \
        &&  gb_tessellator_vertex_point(a)->y <= gb_tessellator_vertex_point(b)->y))
   
/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compute the vertex-to-edge horizontal distance
 *
 *     upper            upper'
 *       .               .
 *      .    distance     .
 *     .   > 0       < 0   .
 *    . -------- . -------- .   
 *   .        center         .
 *  .                         .
 * .                           .
 * lower                       lower'
 *
 * distance = (center - edge(upper, lower)).x
 *
 * @param center        the center point
 * @param upper         the upper point of the edge
 * @param lower         the lower point of the edge
 *
 * @return              the horizontal distance
 */
gb_float_t              gb_tessellator_distance_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower);

/* compute the vertex-to-edge vertical distance
 *
 *                             . right
 *                        .
 *                   .
 *              .    |
 *         .         | distance: > 0 
 *   left            |
 *                   |
 *                   . center
 *                   |
 *   left'           |
 *         .         | distance': < 0
 *              .    |
 *                   . 
 *                        .
 *                             . right'
 *
 * distance = (center - edge(left, right)).y
 *
 * @param center        the center point
 * @param left          the left point of the edge
 * @param right         the right point of the edge
 *
 * @return              the vertical distance
 */
gb_float_t              gb_tessellator_distance_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right);

/* compute the vertex-to-edge horizontal position
 *
 * only evaluate the sign of the distance, faster than distance()
 *
 *     upper            upper'
 *       .               .
 *      .    position     .
 *     .   > 0       < 0   .
 *    . -------- . -------- .   
 *   .        center         .
 *  .                         .
 * .                           .
 * lower                       lower'
 *
 * position = sign((center - edge(upper, lower)).x)
 *
 * @param center        the center point
 * @param upper         the upper point of the edge
 * @param lower         the lower point of the edge
 *
 * @return              the horizontal position
 */
tb_long_t               gb_tessellator_position_h(gb_point_ref_t center, gb_point_ref_t upper, gb_point_ref_t lower);

/* compute the vertex-to-edge vertical position
 *
 * only evaluate the sign of the distance, faster than distance()
 *
 *                             . right
 *                        .
 *                   .
 *              .    |
 *         .         | position: > 0 
 *   left            |
 *                   |
 *                   . center
 *                   |
 *   left'           |
 *         .         | position': < 0
 *              .    |
 *                   . 
 *                        .
 *                             . right'
 *
 * position = sign((center - edge(left, right)).y)
 *
 * @param center        the center point
 * @param left          the left point of the edge
 * @param right         the right point of the edge
 *
 * @return              the vertical position
 */
tb_long_t               gb_tessellator_position_v(gb_point_ref_t center, gb_point_ref_t left, gb_point_ref_t right);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


