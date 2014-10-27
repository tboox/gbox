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

    // the edge pool size
    tb_size_t                       edge_pool_size;
    
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
static tb_bool_t gb_vertex_raster_edge_pool_init(gb_vertex_raster_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // init the edge pool
    if (!impl->edge_pool) impl->edge_pool = tb_nalloc_type(GB_VERTEX_RASTER_EDGES_GROW, gb_vertex_raster_edge_t);
    tb_assert_and_check_return_val(impl->edge_pool, tb_false);

    // init the edge pool size
    impl->edge_pool_size = 0;

    // ok
    return tb_true;
}
static tb_void_t gb_vertex_raster_edge_pool_exit(gb_vertex_raster_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // exit the edge pool
    if (impl->edge_pool) tb_free(impl->edge_pool);
    impl->edge_pool = tb_null;
}
static tb_uint16_t gb_vertex_raster_edge_pool_aloc(gb_vertex_raster_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->edge_pool);

    // the new index
    tb_size_t index = ++impl->edge_pool_size;
    tb_assert_abort(index < TB_MAXU16);

    // grow the edge pool
    if (index >= impl->edge_pool_maxn)
    {
        impl->edge_pool_maxn = index + GB_VERTEX_RASTER_EDGES_GROW;
        impl->edge_pool = tb_ralloc_type(impl->edge_pool, impl->edge_pool_maxn, gb_vertex_raster_edge_t);
        tb_assert_and_check_return_val(impl->edge_pool, 0);
    }

    // make a new edge from the edge pool
    return (tb_uint16_t)index;
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
static tb_void_t gb_vertex_raster_edge_table_exit(gb_vertex_raster_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // exit the edge table
    if (impl->edge_table) tb_free(impl->edge_table);
    impl->edge_table = tb_null;
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
    tb_long_t table_index = tb_fixed_floor(edge->y_top) - impl->edge_table_base;
    tb_assert_abort(table_index >= 0 && table_index < impl->edge_table_maxn);

    // the table head
    tb_uint16_t table_head = impl->edge_table[table_index];

    // insert edge to the active edges by the top y-coordinate in ascending
    edge->next = 0;
    if (!table_head) table_head = edge_index;
    else 
    {
        // find an inserted position
        gb_vertex_raster_edge_ref_t     edge_prev   = tb_null;
        gb_vertex_raster_edge_ref_t     edge_find   = tb_null;
        tb_uint16_t                     find_index  = table_head;
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
            edge->next  = table_head;
            table_head  = edge_index;
        }
        else
        {
            // insert to the body
            edge->next      = find_index;
            edge_prev->next = edge_index;
        }
    }

    // update the head index
    impl->edge_table[table_index] = table_head;
}
static tb_void_t gb_vertex_raster_edge_table_patch(gb_vertex_raster_impl_t* impl, tb_fixed_t y)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table);

    // the edge pool
    gb_vertex_raster_edge_ref_t edge_pool = impl->edge_pool;

    // the table index
    tb_long_t table_index = tb_fixed_floor(y) - impl->edge_table_base;
    tb_assert_abort(table_index >= 0 && table_index < impl->edge_table_maxn);

    // the table head
    tb_uint16_t table_head = impl->edge_table[table_index];

    // find an inserted position
    gb_vertex_raster_edge_ref_t     edge_prev   = tb_null;
    gb_vertex_raster_edge_ref_t     edge_find   = tb_null;
    tb_uint16_t                     find_index  = table_head;
    while (find_index)
    {
        // the edge
        edge_find = edge_pool + find_index;

        // found?
        tb_check_break(y > edge_find->y_top);
        
        // the previous edge
        edge_prev = edge_find;

        // the next edge index
        find_index = edge_prev->next;
    }

    // exists? need not patch it
    if (edge_find && edge_find->y_top == y) return ;

    // make a new edge from the edge pool
    tb_uint16_t edge_index = gb_vertex_raster_edge_pool_aloc(impl);
    tb_assert_abort(edge_index);

    // init the new edge
    gb_vertex_raster_edge_ref_t edge = impl->edge_pool + edge_index;
    edge->y_top     = y;
    edge->patching  = 1;

    // insert edge: edge_prev -> edge -> edge_find
    if (!edge_prev)
    {
        // insert to the head
        edge->next  = table_head;
        table_head  = edge_index;
    }
    else
    {
        // insert to the body
        edge->next      = find_index;
        edge_prev->next = edge_index;
    }

    // update the head index
    impl->edge_table[table_index] = table_head;
}
static tb_void_t gb_vertex_raster_edge_table_intersection(gb_vertex_raster_impl_t* impl, tb_fixed_t y, tb_fixed_t y_next, tb_fixed_t* y_next_real, gb_vertex_raster_edge_ref_t edge, gb_vertex_raster_edge_ref_t edge_next)
{
    // check
    tb_assert_abort(impl && edge && edge_next && y_next_real);

    // the x-coordinates
    tb_fixed_t x                = edge->x;
    tb_fixed_t x_next           = edge_next->x;

    // the slopes
    tb_fixed_t slope            = edge->slope;
    tb_fixed_t slope_next       = edge_next->slope;

    // the bottom y-coordinate
    tb_fixed_t y_bottom         = tb_min3(y_next, edge->y_bottom, edge_next->y_bottom);
    tb_assert_abort(y_bottom > y);

    // no intersection?
    tb_check_return(    slope != slope_next
                    &&  tb_fixed_abs(x - x_next) > TB_FIXED_HALF
                    &&  tb_fixed_abs(y - y_bottom) > TB_FIXED_HALF);

    // the bottom x-coordinate
    tb_fixed_t x_bottom         = x + tb_fixed_mul(y_bottom - y, slope);
    tb_fixed_t x_bottom_next    = x_next + tb_fixed_mul(y_bottom - y, slope_next);

    // exists intersection?
    if (x_bottom > x_bottom_next)
    {
        /* compute the y-coordinate of the intersection
         *
         * edge(x1, yb):        
         *      x = k1 * y + b1
         *      b1 = x1 - k1 * yb
         *
         * edge_next(x2, yb):  
         *      x = k2 * y + b2
         *      b2 = x2 - k2 * yb
         *
         * the y-coordinate of the intersection:
         * => k1 * y + b1 = k2 * y + b2
         * => (k1 - k2) * y = b2 - b1
         * => y = (b2 - b1) / (k1 - k2)
         * => y = (x2 - k2 * yb - x1 + k1 * yb) / (k1 - k2)
         * => y = (x2 - x1 + (k1 - k2) * yb) / (k1 - k2)
         * => y = (x2 - x1) / (k1 - k2) + yb
         *
         * => y_inter = y + (x_next - x) / (slope - slope_next)
         */
        tb_fixed_t y_inter = y + tb_fixed_div(x_next - x, slope - slope_next);

        // trace
        tb_trace_d("compute intersection: x: %{fixed} and %{fixed} => y: %{fixed}", x, x_next, y_inter);

        // this intersection is not belong to the current line?
        if (y_inter > y)
        {
            // patch the y-coordinate of the intersection to the edge table
            gb_vertex_raster_edge_table_patch(impl, y_inter);

            // need update the next y-coordinate if the y-coordinate of the intersection is too small
            if (y_inter < *y_next_real) *y_next_real = y_inter;
        }
    }
}
static tb_bool_t gb_vertex_raster_edge_table_make(gb_vertex_raster_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && polygon && polygon->points && polygon->counts && bounds);

    // empty polygon?
    tb_check_return_val(gb_nz(bounds->w) && gb_nz(bounds->h), tb_false);

    // init the edge pool
    if (!gb_vertex_raster_edge_pool_init(impl)) return tb_false;

    // init the edge table
    if (!gb_vertex_raster_edge_table_init(impl, gb_floor(bounds->y), gb_ceil(bounds->h) + 1)) return tb_false;

    // make the edge table
    gb_point_t          pb;
    gb_point_t          pe;
    tb_bool_t           first       = tb_true;
    tb_fixed_t          top         = 0;
    tb_fixed_t          bottom      = 0;
    tb_uint16_t         index       = 0;
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

                // make a new edge from the edge pool
                tb_uint16_t edge_index = gb_vertex_raster_edge_pool_aloc(impl);
                tb_assert_abort(edge_index);

                // the edge
                gb_vertex_raster_edge_ref_t edge = impl->edge_pool + edge_index;

                // init the winding
                edge->winding = 1;

                // no patching
                edge->patching = 0;

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
                edge->x         = xb;
                edge->x_next    = xb;

                // init top y-coordinate
                edge->y_top     = yb;

                // init bottom y-coordinate
                edge->y_bottom  = ye;

                // insert edge to the edge table 
                gb_vertex_raster_edge_table_insert(impl, edge_index);
 
                // patch one edge to the edge table for scanning the bottom y-coordinate
                gb_vertex_raster_edge_table_patch(impl, ye);
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
static tb_fixed_t gb_vertex_raster_edge_table_next(gb_vertex_raster_impl_t* impl, tb_fixed_t y)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table);

    // the head index
    tb_long_t head_index = tb_fixed_floor(y) - impl->edge_table_base;
    tb_assert_abort(head_index >= 0 && head_index < impl->edge_table_maxn);

    // the tail index
    tb_long_t tail_index = tb_fixed_floor(impl->bottom) - impl->edge_table_base;
    tb_assert_abort(tail_index >= 0 && tail_index < impl->edge_table_maxn);

    // find the next y-coordinate
    tb_uint16_t     edge_index;
    tb_uint16_t*    edge_table = impl->edge_table;
    for (; head_index <= tail_index; head_index++)
    {
        // exists edge?
        if ((edge_index = edge_table[head_index]))
        {
            // return the top y-coordinate of the first edge
            return impl->edge_pool[edge_index].y_top;
        }
    }

    // end
    return impl->bottom + TB_FIXED_ONE;
}
static tb_fixed_t gb_vertex_raster_active_scan_intersection(gb_vertex_raster_impl_t* impl, tb_fixed_t y, tb_fixed_t y_next)
{
    // check
    tb_assert_abort(impl && impl->edge_pool);

    // done
    tb_fixed_t                      y_next_real     = y_next; 
    tb_uint16_t                     index           = impl->active_edges; 
    tb_uint16_t                     index_next      = 0; 
    tb_uint16_t                     index_prev      = 0;
    gb_vertex_raster_edge_ref_t     edge            = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_next       = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_pool       = impl->edge_pool;
    while (index) 
    { 
        // the edge
        edge = edge_pool + index; 

        // the next edge index
        index_next = edge->next; 
        tb_check_break(index_next);

        // the next edge
        edge_next = edge_pool + index_next; 

        // check
        tb_assert_abort(edge->x <= edge_next->x);
        tb_assert_abort(!edge->patching && !edge_next->patching);

        // compute the intersection of the current edge and next edge?
        gb_vertex_raster_edge_table_intersection(impl, y, y_next, &y_next_real, edge, edge_next);

        /* is intersection now and this two edges have been not swaped?
         * we need swap it and compute the intersections
         * 
         *              | intersection |
         * edge_prev => edge => edge_next => edge_next_next
         *    |           |        |                |
         *     -----------|--------                 |
         *        compute | intersection            |
         *                |                         |
         *                 -------------------------
         *                    compute intersection
         */
        if (    y != edge->y_top && y != edge_next->y_top 
            &&  tb_fixed_abs(edge->x - edge_next->x) <= TB_FIXED_NEAR0
            &&  edge->slope != edge_next->slope
            &&  edge->x - edge->slope < edge_next->x - edge_next->slope)
        {
            // trace
            tb_trace_d("intersection now: y: %{fixed}, x: %{fixed}, %{fixed}", y, edge->x, edge_next->x);

            // compute the intersection of the previous edge and next edge
            if (index_prev) gb_vertex_raster_edge_table_intersection(impl, y, y_next, &y_next_real, edge_pool + index_prev, edge_next);

            // compute the intersection of the current edge and next next edge
            if (edge_next->next) gb_vertex_raster_edge_table_intersection(impl, y, y_next, &y_next_real, edge, edge_pool + edge_next->next);
        }

        // update the previous edge index
        index_prev = index;

        // the next edge index
        index = index_next; 
    }

    // update the next y-coordinate
    return y_next_real;
}
static tb_void_t gb_vertex_raster_active_scan_line(gb_vertex_raster_impl_t* impl, tb_fixed_t y, tb_fixed_t y_next, tb_size_t rule, gb_vertex_raster_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && func);

    // done
    tb_long_t                       done            = 0;
    tb_long_t                       winding         = 0; 
    tb_uint16_t                     active_edges    = impl->active_edges;
    tb_uint16_t                     index           = active_edges; 
    tb_uint16_t                     index_next      = 0; 
    tb_uint16_t                     index_prev      = 0;
    gb_vertex_raster_edge_ref_t     edge            = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_next       = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_pool       = impl->edge_pool;
    while (index) 
    { 
        // the edge
        edge = edge_pool + index; 

        // compute the next x-coordinate first
        if (index == active_edges) edge->x_next = edge->x + tb_fixed_mul(tb_min(y_next, edge->y_bottom) - y, edge->slope);

        /* compute the winding
         *   
         *    /\
         *    |            |
         *    |-1          | +1
         *    |            |
         *    |            |
         *                \/
         */
        winding += edge->winding; 

        // the next edge index
        index_next = edge->next; 
        tb_check_break(index_next);

        // the next edge
        edge_next = edge_pool + index_next; 

        // compute the next x-coordinate
        edge_next->x_next = edge_next->x + tb_fixed_mul(tb_min(y_next, edge_next->y_bottom) - y, edge_next->slope);

        // check
        tb_assert_abort(edge->x <= edge_next->x);
        tb_assert_abort(!edge->patching && !edge_next->patching);

        // compute the rule
        switch (rule)
        {
        case GB_VERTEX_RASTER_RULE_ODD:
            {
                /* the odd rule 
                 *
                 *    ------------------                 ------------------ 
                 *  /|\                 |               ||||||||||||||||||||
                 *   |     --------     |               ||||||||||||||||||||
                 *   |   /|\       |    |               ||||||        ||||||
                 * 0 | -1 |   0    | -1 | 0     =>      ||||||        ||||||
                 *   |    |       \|/   |               ||||||        ||||||
                 *   |     --------     |               ||||||||||||||||||||
                 *   |                 \|/              ||||||||||||||||||||
                 *    ------------------                 ------------------ 
                 */
                done = winding & 1;
            }
            break;
        case GB_VERTEX_RASTER_RULE_NONZERO:
            {
                /* the non-zero rule 
                 *
                 *    ------------------                 ------------------
                 *  /|\                 |               ||||||||||||||||||||
                 *   |     --------     |               ||||||||||||||||||||
                 *   |   /|\       |    |               ||||||||||||||||||||
                 * 0 | -1 |   -2   | -1 | 0             ||||||||||||||||||||
                 *   |    |       \|/   |               ||||||||||||||||||||
                 *   |     --------     |               ||||||||||||||||||||
                 *   |                 \|/              ||||||||||||||||||||
                 *    ------------------                 ------------------
                 */
                done = winding;
            }
            break;
        default:
            {
                // clear it
                done = 0;

                // trace
                tb_trace_e("unknown rule: %lu", rule);
            }
            break;
        }

        // trace
        tb_trace_d("y: %{fixed}, winding: %ld, %{fixed} => %{fixed}", y, winding, edge->x, edge_next->x);

        // done it for winding?
        if (done) func(y, y_next, edge, edge_next, priv);

        // update the previous edge index
        index_prev = index;

        // the next edge index
        index = index_next; 
    }
}
static tb_void_t gb_vertex_raster_active_scan_next(gb_vertex_raster_impl_t* impl, tb_fixed_t y, tb_fixed_t y_next, tb_size_t* porder)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table);
    tb_assert_abort(y < y_next && y <= impl->bottom);

    // done
    tb_size_t                       first = 1;
    tb_size_t                       order = 1;
    tb_fixed_t                      prev_x = 0;
    tb_uint16_t                     index_prev = 0;
    tb_uint16_t                     index = impl->active_edges;
    gb_vertex_raster_edge_ref_t     edge = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_prev = tb_null; 
    gb_vertex_raster_edge_ref_t     edge_pool = impl->edge_pool;
    tb_uint16_t                     active_edges = impl->active_edges;
    while (index)
    {
        // the edge
        edge = edge_pool + index;

        /* remove edge from the active edges
         *            
         *             .
         *           .  .
         *         .     .
         *       .        .  
         *     .           . <- y_bottom and the start y of the next edge
         *       .        .
         *          .   .   
         *            .      <- bottom
         */
        if (y_next >= edge->y_bottom)
        {
            // the next edge index
            index = edge->next;

            // remove this edge from head
            if (!index_prev) active_edges = index;
            else 
            {
                // the previous edge 
                edge_prev = edge_pool + index_prev;

                // remove this edge from the body
                edge_prev->next = index;
            }

            // continue 
            continue;
        }

        // check
        tb_assert_abort(y_next < edge->y_bottom);

        // update the x-coordinate
        edge->x = edge->x_next;

        // is order?
        if (porder)
        {
            if (first) first = 0;
            else if (order && edge->x < prev_x) order = 0;
        }

        // update the previous x coordinate
        prev_x = edge->x;

        // update the previous edge index
        index_prev = index;

        // update the edge index
        index = edge->next;
    }

    // save order
    if (porder) *porder = order; 

    // update the active edges 
    impl->active_edges = active_edges;
}
static tb_void_t gb_vertex_raster_active_append(gb_vertex_raster_impl_t* impl, tb_fixed_t y)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table);

    // the table index
    tb_long_t iy            = tb_fixed_floor(y);
    tb_long_t table_index   = iy - impl->edge_table_base;
    tb_assert_abort(table_index >= 0 && table_index < impl->edge_table_maxn);
    tb_assert_abort(impl->edge_table[table_index]);

    // append the some edges, which the top y-coordinate are same
    tb_uint16_t                     table_next = 0;
    tb_uint16_t                     table_head = impl->edge_table[table_index];
    tb_uint16_t                     active_edges = impl->active_edges;
    gb_vertex_raster_edge_ref_t     edge_pool = impl->edge_pool;
    gb_vertex_raster_edge_ref_t     edge = edge_pool + table_head;
    tb_fixed_t                      y_top = edge->y_top;
    do
    {
        // check
        tb_assert_abort(edge->y_top >= y);

        // save the next edge index
        table_next = edge->next;

        // insert it if be not patching 
        if (!edge->patching)
        {
            // insert the edge to the head of the active edges
            edge->next = active_edges;
            active_edges = table_head;
        }

        // the next edge index
        table_head = table_next;

        // the next edge
        edge = edge_pool + table_head;

    } while (table_head && edge->y_top == y_top);

    // update the active edges 
    impl->active_edges = active_edges;

    // update the edge table
    impl->edge_table[table_index] = table_head;
}
static tb_void_t gb_vertex_raster_active_sorted_insert(gb_vertex_raster_impl_t* impl, tb_uint16_t edge_index)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && edge_index);

    // the edge pool
    gb_vertex_raster_edge_ref_t edge_pool = impl->edge_pool;

    // the edge
    gb_vertex_raster_edge_ref_t edge = edge_pool + edge_index;

    // insert edge to the active edges by x in ascending
    edge->next = 0;
    if (!impl->active_edges) impl->active_edges = edge_index;
    else 
    {
        // find an inserted position
        gb_vertex_raster_edge_ref_t edge_prev       = tb_null;
        gb_vertex_raster_edge_ref_t edge_active     = tb_null;
        tb_uint16_t                 index_active    = impl->active_edges;
        while (index_active)
        {
            // the active edge
            edge_active = edge_pool + index_active;

            // check
            tb_assert_abort(edge_index != index_active);

            /* is this?
             *
             * x: 1 2 3     5 6
             *               |
             *             4 or 5
             */
            if (edge->x <= edge_active->x) 
            {
                /* same vertex?
                 *
                 *
                 * x: 1 2 3     5 6
                 *               |   .
                 *               5    .
                 *             .       .
                 *           .          .
                 *         .          active_edge
                 *       .
                 *     edge
                 *
                 * x: 1 2 3   5         6
                 *                 .    |
                 *                  .   5
                 *                   .    .
                 *                    .     .
                 *          active_edge       .
                 *                              . 
                 *                                .  
                 *                                  .
                 *                                   edge
                 *
                 *  x: 1 2 3   5         6
                 *                 .    |
                 *                .     5
                 *              .    .
                 *            .     .
                 *  active_edge    .
                 *                . 
                 *               .  
                 *              .
                 *             edge
                 *
                 *
                 * x: 1 2 3     5 6
                 *               |   .
                 *               5      .
                 *                 .       .
                 *                   .       active_edge 
                 *                     .           
                 *                       .
                 *                         .
                 *                           .
                 *                             .
                 *                               .
                 *                                 .
                 *                                 edge
                 */
                if (edge->x == edge_active->x)
                {
                    /* the edge is at the left-hand of the active edge?
                     * 
                     * x: 1 2 3     5 6    <- active_edges
                     *               |   .
                     *               5    .
                     *             .       .
                     *           .          .
                     *         .        active_edge
                     *       .
                     *     edge
                     *
                     * if (edge->dx / edge->dy < active->dx / active->dy)?
                     */
                    if (edge->slope < edge_active->slope) break;
                }
                else break;
            }
            
            // the previous active edge
            edge_prev = edge_active;

            // the next active edge index
            index_active = edge_prev->next;
        }

        // insert edge to the active edges: edge_prev -> edge -> edge_active
        if (!edge_prev)
        {
            // insert to the head
            edge->next          = impl->active_edges;
            impl->active_edges  = edge_index;
        }
        else
        {
            // insert to the body
            edge->next      = index_active;
            edge_prev->next = edge_index;
        }
    }
}
static tb_void_t gb_vertex_raster_active_sorted_append(gb_vertex_raster_impl_t* impl, tb_fixed_t y)
{
    // check
    tb_assert_abort(impl && impl->edge_pool && impl->edge_table);

    // the table index
    tb_long_t iy            = tb_fixed_floor(y);
    tb_long_t table_index   = iy - impl->edge_table_base;
    tb_assert_abort(table_index >= 0 && table_index < impl->edge_table_maxn);
    tb_assert_abort(impl->edge_table[table_index]);

    // append the some edges, which the top y-coordinate are same
    tb_uint16_t                     table_next = 0;
    tb_uint16_t                     table_head = impl->edge_table[table_index];
    gb_vertex_raster_edge_ref_t     edge_pool = impl->edge_pool;
    gb_vertex_raster_edge_ref_t     edge = edge_pool + table_head;
    tb_fixed_t                      y_top = edge->y_top;
    do
    {
        // check
        tb_assert_abort(edge->y_top >= y);

        // save the next edge index
        table_next = edge->next;

        // insert it if be not patching 
        if (!edge->patching)
        { 
            // insert the edge to the active edges
            gb_vertex_raster_active_sorted_insert(impl, table_head);
        }

        // the next edge index
        table_head = table_next;

        // the next edge
        edge = edge_pool + table_head;

    } while (table_head && edge->y_top == y_top);

    // update the edge table
    impl->edge_table[table_index] = table_head;
}
static tb_void_t gb_vertex_raster_active_sort(gb_vertex_raster_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->edge_pool);

    // done
    tb_uint16_t                     index       = impl->active_edges;
    tb_uint16_t                     index_next  = 0;
    gb_vertex_raster_edge_ref_t     edge        = tb_null;
    gb_vertex_raster_edge_ref_t     edge_next   = tb_null;
    gb_vertex_raster_edge_t         edge_tmp;
    gb_vertex_raster_edge_ref_t     edge_pool   = impl->edge_pool;
    while (index)
    {
        // the edge
        edge = edge_pool + index;

        // the next edge index
        index_next = edge->next;
        while (index_next)
        {
            // the next edge
            edge_next = edge_pool + index_next;

            // need sort? swap them
            if (edge->x > edge_next->x)
            {
                // save the edge
                edge_tmp = *edge;

                // swap the edge
                *edge = *edge_next;

                // restore the next index
                edge->next = edge_tmp.next;
                edge_tmp.next = edge_next->next;

                // swap the next edge
                *edge_next = edge_tmp;
            }
        
            // the next edge index
            index_next = edge_next->next;
        }

        // the next edge index
        index = edge->next;
    }
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

    // exit the edge table
    gb_vertex_raster_edge_table_exit(impl);

    // exit the edge pool
    gb_vertex_raster_edge_pool_exit(impl);

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

    // done scan
    tb_size_t       order       = 1; 
    tb_fixed_t      top         = impl->top; 
    tb_fixed_t      bottom      = impl->bottom; 
    tb_fixed_t      y           = top;
    tb_fixed_t      y_next      = top;
    while (y <= bottom)
    {
        // order? append edges to the sorted active edges by x in ascending
        if (order) gb_vertex_raster_active_sorted_append(impl, y);
        else
        {
            // append edges to the active edges from the edge table
            gb_vertex_raster_active_append(impl, y);
        
            // sort by x in ascending at the active edges
            gb_vertex_raster_active_sort(impl); 
        }

        // find the next y-coordinate from the edge table
        y_next = gb_vertex_raster_edge_table_next(impl, y);

        // scan intersection from the active edges
        y_next = gb_vertex_raster_active_scan_intersection(impl, y, y_next); 

        // scan line from the active edges
        gb_vertex_raster_active_scan_line(impl, y, y_next, rule, func, priv); 

        // end?
        tb_check_break(y < bottom);

        // scan the next line from the active edges
        if (y_next <= bottom) gb_vertex_raster_active_scan_next(impl, y, y_next, &order); 

        // update the y-coordinate 
        y = y_next;
    }
}

