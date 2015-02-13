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
#include "../../geometry.h"

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

/* a is in b's top (contains a == b)
 *
 * sweep direction: horizontal
 *
 * v0 -------> v1-----                  
 * ---> v2 -------> v3                  
 * v4 ----------------                  
 * --------> v5 ------                  
 *
 * v0 is in v1's top
 * v1 is in v5's top
 */
#define gb_tessellator_vertex_in_top(a, b)  \
    (   gb_tessellator_vertex_point(a)->y < gb_tessellator_vertex_point(b)->y \
    ||  (   gb_tessellator_vertex_point(a)->y == gb_tessellator_vertex_point(b)->y \
        &&  gb_tessellator_vertex_point(a)->x <= gb_tessellator_vertex_point(b)->x))
   
/* a is in b's left (contains a == b)
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
 * v0 is in v1's left
 * v1 is in v5's left
 */
#define gb_tessellator_vertex_in_left(a, b)  \
    (   gb_tessellator_vertex_point(a)->x < gb_tessellator_vertex_point(b)->x \
    ||  (   gb_tessellator_vertex_point(a)->x == gb_tessellator_vertex_point(b)->x \
        &&  gb_tessellator_vertex_point(a)->y <= gb_tessellator_vertex_point(b)->y))
    
/* v is on edge(u, l)'s left
 *
 *                      u
 *                      .
 *                       .
 *                        .
 *          . ------------ .   
 *          v               .
 *                           .
 *                            .
 *                            l
 *
 */
#define gb_tessellator_vertex_on_edge_left(v, u, l)     (gb_tessellator_vertex_to_edge_position_h(v, u, l) < 0)
 
/* v is on edge(u, l) or it's left
 *
 *                      u
 *                      .
 *                       .
 *                        .
 *          . ------------ . v'
 *          v               .
 *                           .
 *                            .
 *                            l
 *
 */
#define gb_tessellator_vertex_on_edge_or_left(v, u, l)  (gb_tessellator_vertex_to_edge_position_h(v, u, l) <= 0)

/* v is on edge(u, l)'s right
 *
 *       u
 *       .              
 *      .    
 *     .         
 *    . -------- . 
 *   .           v         
 *  .                         
 * .                         
 * l
 *
 */
#define gb_tessellator_vertex_on_edge_right(v, u, l)    (gb_tessellator_vertex_to_edge_position_h(v, u, l) > 0)
 
/* v is on edge(u, l) or it's right
 *
 *       u
 *       .              
 *      .    
 *     .      
 *  v'. -------- . 
 *   .           v         
 *  .                         
 * .                         
 * l
 *
 */
#define gb_tessellator_vertex_on_edge_or_right(v, u, l) (gb_tessellator_vertex_to_edge_position_h(v, u, l) >= 0)
 
/* v is on edge(u, l)'s top
 *
 *                   . v
 *                   |
 *        l          |
 *         .         | 
 *              .    |
 *                   . 
 *                        .
 *                             . r
 *
 */
#define gb_tessellator_vertex_on_edge_top(v, l, r)      (gb_tessellator_vertex_to_edge_position_v(v, l, r) < 0)
 
/* v is on edge(u, l) or it's top
 *
 *                   . v
 *                   |
 *        l          |
 *         .         | 
 *              .    |
 *                   . 
 *                   v'   .
 *                             . r
 */
#define gb_tessellator_vertex_on_edge_or_top(v, l, r)   (gb_tessellator_vertex_to_edge_position_v(v, l, r) <= 0)

/* v is on edge(u, l)'s bottom
 *
 *                             . r
 *                        .
 *                   .
 *              .    |
 *         .         | 
 *   l               |
 *                   |
 *                   . v
 *
 */
#define gb_tessellator_vertex_on_edge_bottom(v, l, r)   (gb_tessellator_vertex_to_edge_position_v(v, l, r) > 0)
 
/* v is on edge(u, l) or it's bottom
 *
 *                             . r
 *                        .
 *                   . v'
 *              .    |
 *         .         | 
 *   l               |
 *                   |
 *                   . v
 *
 */
#define gb_tessellator_vertex_on_edge_or_bottom(v, l, r) (gb_tessellator_vertex_to_edge_position_v(v, l, r) >= 0)

/* the edge goes left?
 *  __
 * | .
 *     .
 *       .
 *         .
 *           .
 *
 *           .
 *         .
 *       .
 *     .
 * | .
 *  --
 */
#define gb_tessellator_edge_go_left(edge)   gb_tessellator_vertex_in_left(gb_mesh_edge_dst(edge), gb_mesh_edge_org(edge))
  
/* the edge goes right?
 *
 *         __
 *         . |
 *       .
 *     .  
 *   .      
 * .     
 *
 * .
 *   .
 *     .
 *       .
 *         . |
 *         --
 */
#define gb_tessellator_edge_go_right(edge)  gb_tessellator_vertex_in_left(gb_mesh_edge_org(edge), gb_mesh_edge_dst(edge))
 
/* the edge goes up?
 *  __                   __
 * | .                   . |
 *     .               .
 *       .           .
 *         .       .
 *           .   .
 *
 */
#define gb_tessellator_edge_go_up(edge)     gb_tessellator_vertex_in_top(gb_mesh_edge_dst(edge), gb_mesh_edge_org(edge))

/* the edge goes down?
 *
 *           .   .
 *         .       .
 *       .           .
 *     .               .
 * | .                   .|
 *  --                  --
 */
#define gb_tessellator_edge_go_down(edge)   gb_tessellator_vertex_in_top(gb_mesh_edge_org(edge), gb_mesh_edge_dst(edge))

// the three vertices are counter-clockwise?
#define gb_tessellator_vertex_is_ccw(v0, v1, v2) gb_points_is_ccw(gb_tessellator_vertex_point(v0), gb_tessellator_vertex_point(v1), gb_tessellator_vertex_point(v2))

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
 * @param center        the center vertex
 * @param upper         the upper vertex of the edge
 * @param lower         the lower vertex of the edge
 *
 * @return              the horizontal distance
 */
gb_float_t              gb_tessellator_vertex_to_edge_distance_h(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t upper, gb_mesh_vertex_ref_t lower);

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
 * @param center        the center vertex
 * @param left          the left vertex of the edge
 * @param right         the right vertex of the edge
 *
 * @return              the vertical distance
 */
gb_float_t              gb_tessellator_vertex_to_edge_distance_v(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t left, gb_mesh_vertex_ref_t right);

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
 * @param center        the center vertex
 * @param upper         the upper vertex of the edge
 * @param lower         the lower vertex of the edge
 *
 * @return              the horizontal position
 */
tb_long_t               gb_tessellator_vertex_to_edge_position_h(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t upper, gb_mesh_vertex_ref_t lower);

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
 * @param center        the center vertex
 * @param left          the left vertex of the edge
 * @param right         the right vertex of the edge
 *
 * @return              the vertical position
 */
tb_long_t               gb_tessellator_vertex_to_edge_position_v(gb_mesh_vertex_ref_t center, gb_mesh_vertex_ref_t left, gb_mesh_vertex_ref_t right);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


