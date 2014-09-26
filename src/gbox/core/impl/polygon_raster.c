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
            if (edge_lsh->top_x > edge_rsh->top_x)
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
                 * top_x: 1 2 3     5 6
                 *               |
                 *             4 or 5
                 */
                if (edge->top_x <= edge_active->top_x) 
                {
                    /* same vertex?
                     *
                     *
                     * top_x: 1 2 3     5 6
                     *               |   .
                     *               5    .
                     *             .       .
                     *           .          .
                     *         .          active_edge
                     *       .
                     *     edge
                     *
                     * top_x: 1 2 3   5         6
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
                     *  top_x: 1 2 3   5         6
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
                     * top_x: 1 2 3     5 6
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
                    if (edge->top_x == edge_active->top_x)
                    {
                        /* the edge is at the left-hand of the active edge?
                         * 
                         * top_x: 1 2 3     5 6    <- active_edges
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
                        tb_long_t k0 = (tb_long_t)edge->dx2 * edge_active->dy2 * edge->direction_x;
                        tb_long_t k1 = (tb_long_t)edge_active->dx2 * edge->dy2 * edge_active->direction_x;
                        if (k0 < k1) break;
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
    tb_long_t                       counter = 0; 
    tb_uint16_t                     index_lsh = active_edges; 
    tb_uint16_t                     index_rsh = 0; 
    gb_polygon_raster_edge_ref_t    edge_lsh = tb_null; 
    gb_polygon_raster_edge_ref_t    edge_rsh = tb_null; 
    while (index_lsh) 
    { 
        // the left-hand edge
        edge_lsh = edge_pool + index_lsh; 

        /* compute the counter
         *   
         *    /\
         *    |            |
         *    |-1          | +1
         *    |            |
         *    |            |
         *                \/
         */
        counter += edge_lsh->direction_y; 

        // the right-hand edge index
        index_rsh = edge_lsh->next; 
        tb_check_break(index_rsh);

        // the right-hand edge
        edge_rsh = edge_pool + index_rsh; 

        // check
        tb_assert_abort(edge_lsh->top_x <= edge_rsh->top_x);

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
                done = counter & 1;
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
                done = counter;
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
        tb_trace_d("y: %ld, direction: %d, counter: %ld, %d => %d", y, edge_lsh->direction_y, counter, edge_lsh->top_x, edge_rsh->top_x);

        // done func
        if (done) func(y, edge_lsh->top_x, edge_rsh->top_x, priv);

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
    tb_uint16_t                     prev_x = 0;
    tb_uint16_t                     index_prev = 0;
    tb_uint16_t                     index = active_edges;
    gb_polygon_raster_edge_ref_t    edge_prev = tb_null; 
    gb_polygon_raster_edge_ref_t    edge = tb_null; 
    while (index)
    {
        // the edge
        edge = edge_pool + index;

        /* remove edge from the active edges if (y >= edge->bottom_y - 1)
         *            
         *             .
         *           .  .
         *         .     .
         *       .        .  <- bottom_y - 1: end and no next y for this edge, so remove it
         *     .           . <- bottom_y and the start y of the next edge
         *       .        .
         *          .   .   
         *            .      <- bottom
         */
        if (y != bottom - 1 && edge->bottom_y < y + 2)
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

        // compute the x step size: -1 or 1
        tb_int16_t step_x = edge->direction_x;

        /* update the x coordinate using the bresenham algorithm
         *
         * |slope| < 0.5?
         */
        if (edge->dx2 > edge->dy2) 
        {
            // increase x until y++
            while (1)
            {
                if (edge->error > 0)
                {
                    edge->error -= edge->dx2;
                    break;
                }
                edge->error += edge->dy2;
                edge->top_x += step_x;
            }
        }
        else 
        {
            if (edge->error > 0)
            {
                edge->error -= edge->dy2;
                edge->top_x += step_x;
            }
            edge->error += edge->dx2;
        }

        // is order?
        if (first) first = 0;
        else if (order && edge->top_x < prev_x) order = 0;

        // update the previous x coordinate
        prev_x = edge->top_x;

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
    raster->top     = gb_float_to_long(bounds->y);
    raster->bottom  = gb_float_to_long(bounds->y + bounds->h);

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

        // not horizaontal edge?
        if (index && gb_float_to_long(pb.y) != gb_float_to_long(pe.y)) 
        {
            // update the edge index
            edge_index++;

            // check
            tb_assert_abort(edge_index < tb_arrayn(raster->edge_pool));

            // make a new edge from the edge pool
            gb_polygon_raster_edge_ref_t edge = &raster->edge_pool[edge_index];

            // init the edge direction
            edge->direction_x = 1;
            edge->direction_y = 1;

            // sort the points of the edge
            gb_point_ref_t top = &pb;
            gb_point_ref_t bottom = &pe;
            if (bottom->y < top->y)
            {
                // reverse the edge points
                top     = &pe;
                bottom  = &pb;

                // reverse the y direction
                edge->direction_y = -1;
            }

            // the top and bottom coordinates
            tb_long_t top_x     = gb_float_to_long(top->x);
            tb_long_t top_y     = gb_float_to_long(top->y);
            tb_long_t bottom_x  = gb_float_to_long(bottom->x);
            tb_long_t bottom_y  = gb_float_to_long(bottom->y);
            tb_assert_abort(top_x < TB_MAXS16 && bottom_x < TB_MAXS16 && bottom_y < TB_MAXS16);
            tb_assert_abort(bottom_y >= top_y && top_y >= raster->top && top_y - raster->top < tb_arrayn(raster->edge_table));

            // compute dx*2, dy*2 for the edge slope
            edge->dx2           = (tb_int16_t)((bottom_x - top_x) << 1);
            edge->dy2           = (tb_int16_t)((bottom_y - top_y) << 1);
            if (edge->dx2 < 0)
            {
                // |dx2|
                edge->dx2 = -edge->dx2;
                
                // reverse the x direction
                edge->direction_x = -1;
            }
            tb_assert_abort(edge->dy2);

            // init the top x coordinate for the edge
            edge->top_x = (tb_int16_t)top_x;

            // init the bottom y coordinate for the edge
            edge->bottom_y = (tb_int16_t)bottom_y;

            /* compute the slope error for the bresenham algorithm
             *
             * |slope| > 0.5
             * e / 1 = dy / dx
             * => y++ if e = dy / dx > 0.5 
             * => y++ if e = dy * 2 - dx > 0
             *
             * |slope| < 0.5
             * => y++ if e = dx * 2 - dy > 0
             */
            edge->error = (edge->dx2 > edge->dy2)? (edge->dy2 - (edge->dx2 >> 1)) : (edge->dx2 - (edge->dy2 >> 1));

            /* insert edge to the head of the edge table
             *
             * table[index]: => edge => edge => .. => 0
             *              |
             *            insert
             */
            edge->next = raster->edge_table[top_y - raster->top];
            raster->edge_table[top_y - raster->top] = edge_index;
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
    for (y = top; y <= bottom; y++)
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

