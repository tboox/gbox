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
#define gb_tessellator_vertex_eq(a, b)                      gb_point_eq(gb_tessellator_vertex_point(a), gb_tessellator_vertex_point(b))

// a is in b's top or a == b?
#define gb_tessellator_vertex_in_top(a, b)                  gb_point_in_top_or_eq(gb_tessellator_vertex_point(a), gb_tessellator_vertex_point(b))

// a is in b's left or a == b?
#define gb_tessellator_vertex_in_left(a, b)                 gb_point_in_left_or_eq(gb_tessellator_vertex_point(a), gb_tessellator_vertex_point(b))
    
// v is in edge(u, l)'s left?
#define gb_tessellator_vertex_in_edge_left(v, u, l)         gb_point_in_segment_left(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(u), gb_tessellator_vertex_point(l))
 
// v is on edge(u, l) or it's left?
#define gb_tessellator_vertex_on_edge_or_left(v, u, l)      gb_point_on_segment_or_left(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(u), gb_tessellator_vertex_point(l))

// v is in edge(u, l)'s right?
#define gb_tessellator_vertex_in_edge_right(v, u, l)        gb_point_in_segment_right(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(u), gb_tessellator_vertex_point(l))
 
// v is on edge(u, l) or it's right?
#define gb_tessellator_vertex_on_edge_or_right(v, u, l)     gb_point_on_segment_or_right(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(u), gb_tessellator_vertex_point(l))
 
// v is in edge(u, l)'s top?
#define gb_tessellator_vertex_in_edge_top(v, l, r)          gb_point_in_segment_top(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(l), gb_tessellator_vertex_point(r))
 
// v is on edge(u, l) or it's top?
#define gb_tessellator_vertex_on_edge_or_top(v, l, r)       gb_point_on_segment_or_top(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(l), gb_tessellator_vertex_point(r))

// v is in edge(u, l)'s bottom?
#define gb_tessellator_vertex_in_edge_bottom(v, l, r)       gb_point_in_segment_bottom(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(l), gb_tessellator_vertex_point(r))
 
// v is on edge(u, l) or it's bottom?
#define gb_tessellator_vertex_on_edge_or_bottom(v, l, r)    gb_point_on_segment_or_bottom(gb_tessellator_vertex_point(v), gb_tessellator_vertex_point(l), gb_tessellator_vertex_point(r))

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
#define gb_tessellator_vertex_is_ccw(a, b, c)   gb_points_is_ccw(gb_tessellator_vertex_point(a), gb_tessellator_vertex_point(b), gb_tessellator_vertex_point(c))

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


