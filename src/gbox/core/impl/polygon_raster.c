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
 * @file        polygon_raster.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "polygon_raster"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "polygon_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_uint16_t gb_polygon_raster_edges_append(gb_polygon_raster_edge_ref_t edge_pool, tb_uint16_t active_edges, tb_uint16_t edge_index)
{
    // check
    tb_assert_abort(edge_pool);

    // done
    tb_uint16_t                     next = 0;
    gb_polygon_raster_edge_ref_t    edge = tb_null;
    while (edge_index)
    {
        // the edge
        edge = edge_pool + edge_index;

        // save the next edge index
        next = edge->next;

        // insert the edge to the head of the active edges
        edge->next = active_edges;
        active_edges = edge_index;

        // the next edge index
        edge_index = next;
    }

    // update the active edges 
    return active_edges;
}
static tb_void_t gb_polygon_raster_edges_sort(gb_polygon_raster_edge_ref_t edge_pool, tb_uint16_t active_edges)
{
    // check
    tb_assert_abort(edge_pool);

    // done
    tb_uint16_t                     index_lsh   = active_edges;
    tb_uint16_t                     index_rsh   = 0;
    gb_polygon_raster_edge_ref_t    edge_lsh    = tb_null;
    gb_polygon_raster_edge_ref_t    edge_rsh    = tb_null;
    gb_polygon_raster_edge_t        edge_tmp;
    while (index_lsh)
    {
        // the left-hand edge
        edge_lsh = edge_pool + index_lsh;

        // the right-hand edge index
        index_rsh = edge_lsh->next;
        while (index_rsh)
        {
            // the right-hand edge
            edge_rsh = edge_pool + index_rsh;

            // need sort? swap them
            if (edge_lsh->x > edge_rsh->x)
            {
                // save the left-hand edge
                edge_tmp = *edge_lsh;

                // swap the left-hand edge
                *edge_lsh = *edge_rsh;

                // restore the next index
                edge_lsh->next = edge_tmp.next;
                edge_tmp.next = edge_rsh->next;

                // swap the right-hand edge
                *edge_rsh = edge_tmp;
            }
        
            // the next right-hand edge index
            index_rsh = edge_rsh->next;
        }

        // the next left-hand edge index
        index_lsh = edge_lsh->next;
    }
}
static tb_uint16_t gb_polygon_raster_edges_sorted_append(gb_polygon_raster_edge_ref_t edge_pool, tb_uint16_t active_edges, tb_uint16_t edge_index)
{
    // check
    tb_assert_abort(edge_pool);

    // done
    tb_uint16_t                     index_next = 0;
    tb_uint16_t                     index_active = 0;
    gb_polygon_raster_edge_ref_t    edge = tb_null;
    gb_polygon_raster_edge_ref_t    edge_prev = tb_null;
    gb_polygon_raster_edge_ref_t    edge_active = tb_null;
    while (edge_index)
    {
        // the edge
        edge = edge_pool + edge_index;

        // save the next edge index
        index_next = edge->next;

        // insert edge to the active edges by x in ascending
        edge->next = 0;
        if (!active_edges) active_edges = edge_index;
        else 
        {
            // find an inserted position
            edge_prev       = tb_null;
            index_active    = active_edges;
            while (index_active)
            {
                // the active edge
                edge_active = edge_pool + index_active;

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
                edge->next      = active_edges;
                active_edges    = edge_index;
            }
            else
            {
                // insert to the body
                edge->next      = index_active;
                edge_prev->next = edge_index;
            }
        }

        // the next edge index
        edge_index = index_next;
    }

    // update the active edges 
    return active_edges;
}
static tb_void_t gb_polygon_raster_scanning_line(gb_polygon_raster_edge_ref_t edge_pool, tb_uint16_t active_edges, tb_uint16_t y, tb_size_t rule, gb_polygon_raster_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(edge_pool && func);

    // done
    tb_long_t                       done = 0;
    tb_long_t                       winding = 0; 
    tb_uint16_t                     index_lsh = active_edges; 
    tb_uint16_t                     index_rsh = 0; 
    gb_polygon_raster_edge_ref_t    edge_lsh = tb_null; 
    gb_polygon_raster_edge_ref_t    edge_rsh = tb_null; 
    while (index_lsh) 
    { 
        // the left-hand edge
        edge_lsh = edge_pool + index_lsh; 

        /* compute the winding
         *   
         *    /\
         *    |            |
         *    |-1          | +1
         *    |            |
         *    |            |
         *                \/
         */
        winding += edge_lsh->winding; 

        // the right-hand edge index
        index_rsh = edge_lsh->next; 
        tb_check_break(index_rsh);

        // the right-hand edge
        edge_rsh = edge_pool + index_rsh; 

        // check
        tb_assert_abort(edge_lsh->x <= edge_rsh->x);

        // compute the rule
        switch (rule)
        {
        case GB_POLYGON_RASTER_RULE_ODD:
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
        case GB_POLYGON_RASTER_RULE_NONZERO:
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
        tb_trace_d("y: %ld, winding: %ld, %{fixed} => %{fixed}", y, winding, edge_lsh->x, edge_rsh->x);

        // done func
        if (done) func(y, tb_fixed_round(edge_lsh->x), tb_fixed_round(edge_rsh->x), priv);

        // the left-hand edge index
        index_lsh = index_rsh; 
    }
}
static tb_int16_t gb_polygon_raster_scanning_next(gb_polygon_raster_edge_ref_t edge_pool, tb_uint16_t active_edges, tb_long_t y, tb_long_t bottom, tb_size_t* porder)
{
    // check
    tb_assert_abort(edge_pool && porder && y <= bottom);

    // done
    tb_size_t                       first = 1;
    tb_size_t                       order = 1;
    tb_fixed_t                      prev_x = 0;
    tb_uint16_t                     index_prev = 0;
    tb_uint16_t                     index = active_edges;
    gb_polygon_raster_edge_ref_t    edge_prev = tb_null; 
    gb_polygon_raster_edge_ref_t    edge = tb_null; 
    while (index)
    {
        // the edge
        edge = edge_pool + index;

        /* remove edge from the active edges if (y >= edge->bottom_y)
         *            
         *             .
         *           .  .
         *         .     .
         *       .        .  <- bottom_y: end and no next y for this edge, so remove it
         *     .           . <- the start y of the next edge
         *       .        .
         *          .   .   
         *            .      <- bottom
         */
        if (y != bottom - 1 && edge->bottom_y < y + 1)
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
            continue;
        }

        // update the x-coordinate
        edge->x += edge->slope;

        // is order?
        if (first) first = 0;
        else if (order && edge->x < prev_x) order = 0;

        // update the previous x coordinate
        prev_x = edge->x;

        // update the previous edge index
        index_prev = index;

        // the next edge index
        index = edge->next;
    }

    // save order
    *porder = order; 

    // update the active edges 
    return active_edges;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_polygon_raster_init(gb_polygon_raster_ref_t raster, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(raster && polygon && polygon->points && polygon->counts && bounds);

    // empty polygon?
    tb_check_return_val(gb_nz(bounds->w) && gb_nz(bounds->h), tb_false);

    // clear the edge table
    tb_memset(raster->edge_table, 0, sizeof(raster->edge_table));

    // init top and bottom of the polygon
    raster->top     = gb_round(bounds->y);
    raster->bottom  = gb_round(bounds->y + bounds->h);

    // init the edge table
    gb_point_t      pb;
    gb_point_t      pe;
    tb_uint16_t     index = 0;
    tb_uint16_t     edge_index = 0;
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    while (index < count)
    {
        // the point
        pe = *points++;

        // exists edge?
        if (index)
        {
            // get the integer y-coordinates
            tb_long_t iyb = gb_round(pb.y);
            tb_long_t iye = gb_round(pe.y);

            // not horizaontal edge?
            if (iyb != iye) 
            {
                // get the fixed-point coordinates
                tb_fixed6_t xb = gb_float_to_fixed6(pb.x);
                tb_fixed6_t yb = gb_float_to_fixed6(pb.y);
                tb_fixed6_t xe = gb_float_to_fixed6(pe.x);
                tb_fixed6_t ye = gb_float_to_fixed6(pe.y);

                // compute the delta coordinates
                tb_fixed6_t dx = xe - xb;
                tb_fixed6_t dy = ye - yb;

                // update the edge index
                edge_index++;

                // check
                tb_assert_abort(edge_index < tb_arrayn(raster->edge_pool));

                // make a new edge from the edge pool
                gb_polygon_raster_edge_ref_t edge = &raster->edge_pool[edge_index];

                // init the winding
                edge->winding = 1;

                // sort the points of the edge by the y-coordinate
                if (yb > ye)
                {
                    // reverse the edge points
                    tb_swap(tb_fixed6_t, xb, xe);
                    tb_swap(tb_fixed6_t, yb, ye);
                    tb_swap(tb_long_t, iyb, iye);

                    // reverse the winding
                    edge->winding = -1;
                }

                // check
                tb_assert_abort(iyb < iye);

                // compute the slope 
                edge->slope = tb_fixed6_div(dx, dy);

                /* compute the more accurate start x-coordinate
                 *
                 * xb + (iyb - yb + 0.5) * dx / dy
                 * => xb + ((0.5 - yb) % 1) * dx / dy
                 */
                edge->x = tb_fixed6_to_fixed(xb) + ((edge->slope * ((TB_FIXED6_HALF - yb) & 63)) >> 6);

                // init the bottom y coordinate for the edge
                edge->bottom_y = iye - 1;

                // the top y-coordinate
                tb_long_t top_y = iyb;
                tb_assert_abort(top_y >= raster->top && top_y - raster->top < tb_arrayn(raster->edge_table));

                /* insert edge to the head of the edge table
                 *
                 * table[index]: => edge => edge => .. => 0
                 *              |
                 *            insert
                 */
                edge->next = raster->edge_table[top_y - raster->top];
                raster->edge_table[top_y - raster->top] = edge_index;
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

    // ok
    return tb_true;
}
tb_void_t gb_polygon_raster_exit(gb_polygon_raster_ref_t raster)
{
}
tb_void_t gb_polygon_raster_done(gb_polygon_raster_ref_t raster, tb_size_t rule, gb_polygon_raster_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(raster && func);

    // done scanning
    tb_long_t                       y;
    tb_size_t                       order           = 1; 
    tb_uint16_t                     active_edges    = 0; 
    tb_long_t                       top             = raster->top; 
    tb_long_t                       bottom          = raster->bottom; 
    gb_polygon_raster_edge_ref_t    edge_pool       = raster->edge_pool;
    tb_uint16_t*                    edge_table      = raster->edge_table;
    for (y = top; y < bottom; y++)
    {
        // order? append edges to the sorted active edges by x in ascending
        if (order) active_edges = gb_polygon_raster_edges_sorted_append(edge_pool, active_edges, edge_table[y - top]); 
        else
        {
            // append edges to the active edges from the edge table
            active_edges = gb_polygon_raster_edges_append(edge_pool, active_edges, edge_table[y - top]); 

            // sort by x in ascending at the active edges
            gb_polygon_raster_edges_sort(edge_pool, active_edges); 
        }

        // scanning line from the active edges
        gb_polygon_raster_scanning_line(edge_pool, active_edges, y, rule, func, priv); 

        // scanning the next line from the active edges
        active_edges = gb_polygon_raster_scanning_next(edge_pool, active_edges, y, bottom, &order); 
    }
}

