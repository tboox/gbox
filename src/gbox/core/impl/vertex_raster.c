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
 * @file        vertex_raster.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "vertex_raster"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "vertex_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the polygon edges grow
#ifdef __gb_small__
#   define GB_VERTEX_RASTER_EDGES_GROW     (1024)
#else
#   define GB_VERTEX_RASTER_EDGES_GROW     (2048)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the polygon vertex raster type
 *
 * 1. make the edge table and sort edges by x-coordinate
 *
 *     (y)
 *      0 ----------------> . 
 *      1                 .   .
 *      2               .       . e2
 *      3          e1 .           .
 *      4 ------------------------> . 
 *      5         .               .
 *      6       .               .
 *      7 --> .               . e3 
 *      8       .    -> . . . . . . . . <-
 *      9      e4 .       .         .
 *      10          .   . e5 .    . e7
 *      11            .         .
 *
 * edge_table[0]: e1 e2
 * edge_table[4]: e3
 * edge_table[7]: e4
 * edge_table[8]: e5 e7
 *
 * 2. scan the all vertexes
 * - compute intersecting points
 * - insert new intersecting edges to the edge table
 *
 *     (y)
 *      0                   - 
 *      1                 .   .
 *      2               .       . e2
 *      3          e1 .           .
 *      4           . ------------- . 
 *      5         .               .
 *      6       .               .
 *      7     . ------------- . e3
 *      8       .        . . x . . . . . <-
 *      9      e4 .        x         .
 *      10          .   .  e5 .   . e7
 *      11            -         -
 *
 */
typedef struct __gb_vertex_raster_impl_t
{
    // the edge pool, tail: 0, index: > 0
    gb_vertex_raster_edge_ref_t     edge_pool;

    // the edge pool maxn
    tb_size_t                       edge_pool_maxn;
    
    // the edge table
    tb_uint16_t*                    edge_table;

    // the edge table base for the y-coordinate
    tb_long_t                       edge_table_base;

    // the edge table maxn
    tb_size_t                       edge_table_maxn;

    // the active edges
    tb_uint16_t                     active_edges;

    // the top of the polygon bounds
    tb_fixed_t                      top;

    // the bottom of the polygon bounds
    tb_fixed_t                      bottom;

}gb_vertex_raster_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static gb_vertex_raster_edge_ref_t gb_vertex_raster_edge_init(gb_vertex_raster_impl_t* impl, tb_uint16_t index)
{
    // check
    tb_assert_abort(impl && index <= TB_MAXU16);

    // init the edge pool
    if (!impl->edge_pool) impl->edge_pool = tb_nalloc_type(GB_VERTEX_RASTER_EDGES_GROW, gb_vertex_raster_edge_t);
    tb_assert_and_check_return_val(impl->edge_pool, tb_null);

    // grow the edge pool
    if (index >= impl->edge_pool_maxn)
    {
        impl->edge_pool_maxn = index + GB_VERTEX_RASTER_EDGES_GROW;
        impl->edge_pool = tb_ralloc_type(impl->edge_pool, impl->edge_pool_maxn, gb_vertex_raster_edge_t);
        tb_assert_and_check_return_val(impl->edge_pool, tb_null);
    }

    // make a new edge from the edge pool
    return &impl->edge_pool[index];
}
static tb_bool_t gb_vertex_raster_edge_table_init(gb_vertex_raster_impl_t* impl, tb_long_t table_base, tb_size_t table_size)
{
    // check
    tb_assert_abort(impl && table_size);

    // init the edge table
    if (!impl->edge_table)
    {
        impl->edge_table_maxn = table_size;
        impl->edge_table = tb_nalloc_type(impl->edge_table_maxn, tb_uint16_t);
    }
    else if (table_size > impl->edge_table_maxn)
    {
        impl->edge_table_maxn = table_size;
        impl->edge_table = tb_ralloc_type(impl->edge_table, impl->edge_table_maxn, tb_uint16_t);
    }
    tb_assert_and_check_return_val(impl->edge_table && table_size <= TB_MAXU16, tb_false);

    // clear the edge table
    tb_memset(impl->edge_table, 0, table_size * sizeof(tb_uint16_t));

    // init the edge table base
    impl->edge_table_base = table_base;

    // ok
    return tb_true;
}
static tb_void_t gb_vertex_raster_edge_table_insert(gb_vertex_raster_impl_t* impl, tb_uint16_t edge_index)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table && edge_index);

    // the edge pool
    gb_vertex_raster_edge_ref_t edge_pool = impl->edge_pool;

    // the edge
    gb_vertex_raster_edge_ref_t edge = edge_pool + edge_index;

    // the table index
    tb_long_t table_index = tb_fixed_to_long(edge->y_top) - impl->edge_table_base;
    tb_assert_abort(table_index >= 0 && table_index < impl->edge_table_maxn);

    // the head index
    tb_uint16_t head_index = impl->edge_table[table_index];

    // insert edge to the active edges by the top y-coordinate in ascending
    edge->next = 0;
    if (!head_index) head_index = edge_index;
    else 
    {
        // find an inserted position
        gb_vertex_raster_edge_ref_t     edge_prev   = tb_null;
        gb_vertex_raster_edge_ref_t     edge_find   = tb_null;
        tb_uint16_t                     find_index  = head_index;
        while (find_index)
        {
            // the edge
            edge_find = edge_pool + find_index;

            // check
            tb_assert_abort(edge_index != find_index);

            // found?
            tb_check_break(edge->y_top > edge_find->y_top);
            
            // the previous edge
            edge_prev = edge_find;

            // the next edge index
            find_index = edge_prev->next;
        }

        // insert edge: edge_prev -> edge -> edge_find
        if (!edge_prev)
        {
            // insert to the head
            edge->next  = head_index;
            head_index  = edge_index;
        }
        else
        {
            // insert to the body
            edge->next      = find_index;
            edge_prev->next = edge_index;
        }
    }

    // update the head index
    impl->edge_table[table_index] = head_index;
}
static tb_bool_t gb_vertex_raster_edge_table_make(gb_vertex_raster_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && polygon && polygon->points && polygon->counts && bounds);

    // empty polygon?
    tb_check_return_val(gb_nz(bounds->w) && gb_nz(bounds->h), tb_false);

    // init the edge table
    if (!gb_vertex_raster_edge_table_init(impl, gb_round(bounds->y), gb_round(bounds->h) + 1)) return tb_false;

    // make the edge table
    gb_point_t          pb;
    gb_point_t          pe;
    tb_bool_t           first       = tb_true;
    tb_fixed_t          top         = 0;
    tb_fixed_t          bottom      = 0;
    tb_uint16_t         index       = 0;
    tb_uint16_t         edge_index  = 0;
    gb_point_ref_t      points      = polygon->points;
    tb_uint16_t*        counts      = polygon->counts;
    tb_uint16_t         count       = *counts++;
    while (index < count)
    {
        // the point
        pe = *points++;

        // exists edge?
        if (index)
        {
            // the y-coordinates
            tb_fixed_t yb = gb_float_to_fixed(pb.y);
            tb_fixed_t ye = gb_float_to_fixed(pe.y);
            tb_fixed_t dy = ye - yb;

            // not horizaontal edge?
            if (tb_fixed_abs(dy) > TB_FIXED_NEAR0)
            {
                // the x-coordinates
                tb_fixed_t xb = gb_float_to_fixed(pb.x);
                tb_fixed_t xe = gb_float_to_fixed(pe.x);
                tb_fixed_t dx = xe - xb;

                // update the edge index
                edge_index++;

                // make a new edge from the edge pool
                gb_vertex_raster_edge_ref_t edge = gb_vertex_raster_edge_init(impl, edge_index);

                // init the winding
                edge->winding = 1;

                // sort the points of the edge by the y-coordinate
                if (yb > ye)
                {
                    // reverse the edge points
                    tb_swap(tb_fixed_t, xb, xe);
                    tb_swap(tb_fixed_t, yb, ye);

                    // reverse the winding
                    edge->winding = -1;
                }

                // compute the accurate bounds of the y-coordinate
                if (first)
                {
                    top     = yb;
                    bottom  = ye;
                    first   = tb_false;
                }
                else
                {
                    if (yb < top)    top = yb;
                    if (ye > bottom) bottom = ye;
                }

                // check
                tb_assert_abort(yb < ye);

                // compute the slope 
                edge->slope = tb_fixed_div(dx, dy);

                // init the x-coordinate
                edge->x = xb;

                // init top y-coordinate
                edge->y_top = yb;

                // init bottom y-coordinate
                edge->y_bottom = ye;

                // insert edge to the edge table 
                gb_vertex_raster_edge_table_insert(impl, edge_index);
            }
        }

        // save the previous point
        pb = pe;
        
        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }

    // update top and bottom of the polygon
    impl->top     = top;
    impl->bottom  = bottom;

    // ok
    return tb_true;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_vertex_raster_ref_t gb_vertex_raster_init()
{
    // init it
    return (gb_vertex_raster_ref_t)tb_malloc0_type(gb_vertex_raster_impl_t);
}
tb_void_t gb_vertex_raster_exit(gb_vertex_raster_ref_t raster)
{
    // check
    gb_vertex_raster_impl_t* impl = (gb_vertex_raster_impl_t*)raster;
    tb_assert_and_check_return(impl);

    // exit the edge pool
    if (impl->edge_pool) tb_free(impl->edge_pool);
    impl->edge_pool = tb_null;

    // exit the edge table
    if (impl->edge_table) tb_free(impl->edge_table);
    impl->edge_table = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_vertex_raster_done(gb_vertex_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_vertex_raster_func_t func, tb_cpointer_t priv)
{
    // check
    gb_vertex_raster_impl_t* impl = (gb_vertex_raster_impl_t*)raster;
    tb_assert_abort(impl && polygon && func);

    // only for the concave polygon
    tb_assert_abort(!polygon->convex);

    // init the active edges
    impl->active_edges = 0;

    // make the edge table
    if (!gb_vertex_raster_edge_table_make(impl, polygon, bounds)) return ;

}

