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
 * @file        convex_maker.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "convex_maker"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "convex_maker.h"
#include "vertex_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the polygon points grow
#ifdef __gb_small__
#   define GB_CONVEX_MAKER_POINTS_GROW        (16)
#else
#   define GB_CONVEX_MAKER_POINTS_GROW        (32)
#endif

// the polygon contours grow
#ifdef __gb_small__
#   define GB_CONVEX_MAKER_CONTOURS_GROW      (8)
#else
#   define GB_CONVEX_MAKER_CONTOURS_GROW      (16)
#endif

// test the polygon edge
#define GB_CONVEX_MAKER_TEST_EDGE

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the polygon contour edge type
typedef struct __gb_convex_maker_contour_edge_t
{
    // the current x-coordinate
    tb_fixed_t                          x;

    // the next x-coordinate
    tb_fixed_t                          x_next;

    // the bottom y-coordinate
    tb_fixed_t                          y_bottom;

    // the current slope: dx / dy
    tb_fixed_t                          slope;

    // the last cross for analyzing convex contour
    tb_fixed_t                          cross;

    // the points
    gb_point_ref_t                      points;

    // the points maxn
    tb_uint16_t                         points_maxn;

    // the points count
    tb_uint16_t                         points_size;

}gb_convex_maker_contour_edge_t;

// the polygon contour type
typedef struct __gb_convex_maker_contour_t
{
    // the current y-coordinate
    tb_fixed_t                          y;

    // the next y-coordinate
    tb_fixed_t                          y_next;

    // the left-hand edge
    gb_convex_maker_contour_edge_t      le;

    // the right-hand edge
    gb_convex_maker_contour_edge_t      re;

    // the active contour index
    tb_uint16_t                         index;

    // the list entry
    tb_list_entry_t                     entry;

}gb_convex_maker_contour_t, *gb_convex_maker_contour_ref_t;

// the polygon maker impl type
typedef struct __gb_convex_maker_impl_t
{
    // the raster
    gb_vertex_raster_ref_t              raster;

    // the contours
    tb_fixed_pool_ref_t                 contours;

    // the free contours
    tb_list_entry_head_t                contours_free;

    /* the active contours
     *
     *      ------------------------------------------------------------
     * lx: |         |         |         |              ...             |
     *      ------------------------------------------------------------
     *          |                   |
     *       contour               ...
     *          |
     *         ...
     */
    tb_list_entry_head_t*               contours_active;

    // the active contours maxn
    tb_size_t                           contours_active_maxn;

    // the active contours size
    tb_size_t                           contours_active_size;

    // the left x-coordinate of the polygon
    tb_long_t                           x_left;

    // the user maker func
    gb_convex_maker_func_t              func;

    // the user private data
    tb_cpointer_t                       priv;

}gb_convex_maker_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_convex_maker_contour_free(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_convex_maker_contour_ref_t contour = (gb_convex_maker_contour_ref_t)data;
    tb_assert_abort(contour);

    // exit the left-hand points
    if (contour->le.points) tb_free(contour->le.points);
    contour->le.points = tb_null;

    // exit the right-hand points
    if (contour->re.points) tb_free(contour->re.points);
    contour->re.points = tb_null;
}
static tb_void_t gb_convex_maker_contour_exit(gb_convex_maker_impl_t* impl, gb_convex_maker_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && contour);

    // trace
    tb_trace_d("exit contour: lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed}", contour->le.x, contour->re.x, contour->le.slope, contour->re.slope);
            
    // put the contour to the free contours
    tb_list_entry_insert_head(&impl->contours_free, &contour->entry);
}
static gb_convex_maker_contour_ref_t gb_convex_maker_contour_init(gb_convex_maker_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->contours);

    // done
    tb_bool_t                       ok  = tb_false;
    gb_convex_maker_contour_ref_t   contour = tb_null;
    do
    {
        // attempt to get contour from the free contours
        if (tb_list_entry_size(&impl->contours_free))
        {
            tb_list_entry_ref_t entry = tb_list_entry_head(&impl->contours_free);
            if (entry)
            {
                contour = (gb_convex_maker_contour_ref_t)tb_list_entry(&impl->contours_free, entry);
                tb_list_entry_remove_head(&impl->contours_free);
            }
        }

        // get contour from the contour pool
        if (!contour) contour = tb_fixed_pool_malloc0(impl->contours);
        tb_assert_and_check_break(contour);

        // init the contour 
        contour->y      = 0;
        contour->y_next = 0;
        contour->index  = 0;

        // init the left-hand edge
        contour->le.x               = 0;
        contour->le.x_next          = 0;
        contour->le.y_bottom        = 0;
        contour->le.slope           = 0;
        contour->le.cross           = TB_FIXED_ONE;
        contour->le.points_size    = 0;
        if (!contour->le.points) 
        {
            contour->le.points_maxn    = GB_CONVEX_MAKER_POINTS_GROW;
            contour->le.points         = tb_nalloc_type(contour->le.points_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->le.points);

        // init the right-hand points
        contour->re.x               = 0;
        contour->re.x_next          = 0;
        contour->re.y_bottom        = 0;
        contour->re.slope           = 0;
        contour->re.cross           = -TB_FIXED_ONE;
        contour->re.points_size    = 0;
        if (!contour->re.points) 
        {
            // ensure the enough space for making complete contour: re.points = re.points + reverse(le.points)
            contour->re.points_maxn    = GB_CONVEX_MAKER_POINTS_GROW << 1;
            contour->re.points         = tb_nalloc_type(contour->re.points_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->re.points);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (contour) gb_convex_maker_contour_exit(impl, contour);
        contour = tb_null;
    }

    // ok?
    return contour;
}
static __tb_inline__ tb_long_t gb_convex_maker_contour_indx(gb_convex_maker_impl_t* impl, tb_long_t xb)
{
    // check
    tb_assert_abort(impl);

    // compute the active contours index
    tb_long_t index = xb - impl->x_left;
    if (index < 0) index = 0;
    if (index >= impl->contours_active_size) index = impl->contours_active_size - 1;
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

    // ok
    return index;
}
static gb_convex_maker_contour_ref_t gb_convex_maker_contour_find_at(gb_convex_maker_impl_t* impl, tb_long_t index, tb_fixed_t y, tb_fixed_t lx, tb_fixed_t rx)
{
    // check
    tb_assert_abort(impl && impl->contours_active);
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // empty? 
    gb_convex_maker_contour_ref_t contour = tb_null;
    if (tb_list_entry_size(contours))
    {
        // find it
        tb_for_all_if (gb_convex_maker_contour_ref_t, item, tb_list_entry_itor(contours), item)
        {
            // is this?
            if (    item->y_next == y
                &&  tb_fixed_near_eq(rx, item->re.x_next)
                &&  tb_fixed_near_eq(lx, item->le.x_next))
            {
                // save it
                contour = item;

                // ok
                break;
            }
        }
    }

    // trace
    tb_trace_d("find contour(at: (%ld, %lu), y: %{fixed}: %s", index, tb_list_entry_size(contours), y, contour? "ok" : "no");

    // ok?
    return contour;
}
static gb_convex_maker_contour_ref_t gb_convex_maker_contour_find(gb_convex_maker_impl_t* impl, tb_fixed_t y, tb_fixed_t lx, tb_fixed_t rx)
{
    // check
    tb_assert_abort(impl && impl->contours_active);

    // compute the current contour index
    tb_long_t index = gb_convex_maker_contour_indx(impl, tb_fixed_round(lx));

    // find the contour in the current index
    gb_convex_maker_contour_ref_t contour = gb_convex_maker_contour_find_at(impl, index, y, lx, rx);
    if (!contour)
    {
#if 1
        /* find the contour in the range: [index - 1, index + 1]
         *
         * because the fixed_round() exists some error 
         */
        if (index > 1) contour = gb_convex_maker_contour_find_at(impl, index - 1, y, lx, rx);
        if (!contour && index < impl->contours_active_size - 1)
            contour = gb_convex_maker_contour_find_at(impl, index + 1, y, lx, rx);
#endif
    }

    // ok?
    return contour;
}
static tb_void_t gb_convex_maker_contour_grow_l(gb_convex_maker_contour_ref_t contour, tb_uint16_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the left-hand points if not enough 
    if (count > contour->le.points_maxn)
    {
        // the points
        gb_point_ref_t  points          = contour->le.points;
        tb_uint16_t     points_size     = contour->le.points_size;
        tb_uint16_t     points_maxn     = contour->le.points_maxn;
        tb_uint16_t     points_maxn_new = count + GB_CONVEX_MAKER_POINTS_GROW;

        // grow points
        points = tb_ralloc_type(points, points_maxn_new, gb_point_t);
        tb_assert_abort(points);

        // move the previous data to the new tail
        if (points_size) tb_memmov(points + points_maxn_new - points_size, points + points_maxn - points_size, points_size * sizeof(gb_point_t));

        // update the points
        contour->le.points      = points;
        contour->le.points_maxn = points_maxn_new;
    }
}
static tb_void_t gb_convex_maker_contour_grow_r(gb_convex_maker_contour_ref_t contour, tb_uint16_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the right-hand points if not enough 
    if (count > contour->re.points_maxn)
    {
        contour->re.points_maxn    = count + (GB_CONVEX_MAKER_POINTS_GROW << 1);
        contour->re.points         = tb_ralloc_type(contour->re.points, contour->re.points_maxn, gb_point_t);
        tb_assert_abort(contour->re.points);
    }
}
static tb_void_t gb_convex_maker_contour_append_l(gb_convex_maker_contour_ref_t contour, tb_fixed_t x, tb_fixed_t y)
{
    // check
    tb_assert_abort(contour && contour->le.points_size < TB_MAXU16);

    // grow the left-hand points if not enough 
    gb_convex_maker_contour_grow_l(contour, contour->le.points_size + 1);

    // reverse to append the left-hand point in the tail
    gb_point_make(&contour->le.points[contour->le.points_maxn - ++contour->le.points_size], gb_fixed_to_float(x), gb_fixed_to_float(y));
}
static tb_void_t gb_convex_maker_contour_append_r(gb_convex_maker_contour_ref_t contour, tb_fixed_t x, tb_fixed_t y)
{
    // check
    tb_assert_abort(contour && contour->re.points_size < TB_MAXU16);

    // grow the right-hand points if not enough 
    gb_convex_maker_contour_grow_r(contour, contour->re.points_size + 1);

    // append the right-hand point
    gb_point_make(&contour->re.points[contour->re.points_size++], gb_fixed_to_float(x), gb_fixed_to_float(y));
}
static tb_bool_t gb_convex_maker_contour_done(gb_convex_maker_impl_t* impl, gb_convex_maker_contour_ref_t contour, tb_fixed_t* px_inter, tb_fixed_t* py_inter)
{
    // check
    tb_assert_abort(impl && impl->func && contour && contour->le.points && contour->re.points);

    // add end-points to the contour
    tb_bool_t   is_inter = tb_false;
    tb_fixed_t  lx_next  = contour->le.x_next;
    tb_fixed_t  rx_next  = contour->re.x_next;
    tb_fixed_t  ly_next  = tb_min(contour->y_next, contour->le.y_bottom);
    tb_fixed_t  ry_next  = tb_min(contour->y_next, contour->re.y_bottom);
    if (lx_next >= rx_next)
    {
        // check
        tb_assert_abort(ly_next == ry_next);

        // exists intersection
        is_inter = tb_true;

        // same?
        tb_fixed_t x;
        tb_fixed_t y;
        if (lx_next == rx_next) 
        {
            x = rx_next;
            y = ry_next;
        }
        // compute the intersection
        else if (contour->le.slope != contour->re.slope)
        {
            y = contour->y + tb_fixed_div(contour->re.x - contour->le.x, contour->le.slope - contour->re.slope);
            x = contour->le.x + tb_fixed_mul(y - contour->y, contour->le.slope);
            gb_convex_maker_contour_append_r(contour, x, y);
        }
        // nearly same? compute the center point
        else
        {
            x = tb_fixed_avg(lx_next, rx_next);
            y = tb_fixed_avg(contour->y, ly_next);
        }
            
        // append the intersection
        gb_convex_maker_contour_append_r(contour, x, y);

        // save the intersection
        if (px_inter) *px_inter = x;
        if (py_inter) *py_inter = y;
    }
    else
    {
        gb_convex_maker_contour_append_l(contour, lx_next, ly_next);
        gb_convex_maker_contour_append_r(contour, rx_next, ry_next);
    }

    // the points
    gb_point_ref_t  re_points       = contour->re.points;
    gb_point_ref_t  le_points       = contour->le.points;
    tb_uint16_t     re_points_size  = contour->re.points_size;
    tb_uint16_t     le_points_size  = contour->le.points_size;
    tb_uint16_t     le_points_maxn  = contour->le.points_maxn;

    // grow the right-hand points if not enough 
    gb_convex_maker_contour_grow_r(contour, re_points_size + le_points_size + 1);

    // append the left-hand points to the right-hand points
    tb_memcpy(re_points + re_points_size, le_points + le_points_maxn - le_points_size, le_points_size * sizeof(gb_point_t));
    re_points_size += le_points_size;

    // close it
    if (re_points[re_points_size - 1].x != re_points[0].x || re_points[re_points_size - 1].y != re_points[0].y)
        re_points[re_points_size++] = re_points[0];

    // trace
    tb_trace_d("done contour(at: (%ld), y: %{fixed}, x: %{fixed}, %{fixed}, slope: %{fixed}, %{fixed})", contour->index, contour->y, contour->le.x, contour->re.x, contour->le.slope, contour->re.slope);

    // done func
    impl->func(re_points, re_points_size, impl->priv);

    // reset the contour
    contour->le.points_size = 0;
    contour->re.points_size = 0;
    contour->le.cross       = TB_FIXED_ONE;
    contour->re.cross       = -TB_FIXED_ONE;

    // ok?
    return is_inter;
}
static tb_void_t gb_convex_maker_contour_insert(gb_convex_maker_impl_t* impl, gb_convex_maker_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the contour index
    tb_long_t index = gb_convex_maker_contour_indx(impl, tb_fixed_round(contour->le.x_next));

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // insert this contour
    tb_list_entry_insert_head(contours, &contour->entry);

    // save the active contours index
    contour->index = (tb_uint16_t)index;

    // trace
    tb_trace_d("insert contour(at: (%ld, %lu), y: %{fixed}, x: %{fixed}, %{fixed}, slope: %{fixed}, %{fixed})", index, tb_list_entry_size(contours), contour->y, contour->le.x, contour->re.x, contour->le.slope, contour->re.slope);
}
static tb_void_t gb_convex_maker_contour_update(gb_convex_maker_impl_t* impl, gb_convex_maker_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the contour index
    tb_long_t index = gb_convex_maker_contour_indx(impl, tb_fixed_round(contour->le.x_next));

    // the old active contours 
    tb_list_entry_head_ref_t contours_old = impl->contours_active + contour->index;

    // the new active contours 
    tb_list_entry_head_ref_t contours_new = impl->contours_active + index;

    // remove the contour from the old active contours
    tb_list_entry_remove(contours_old, &contour->entry);

    // insert to the new active contours
    tb_list_entry_insert_head(contours_new, &contour->entry);

    // update the active contours index
    contour->index = (tb_uint16_t)index;

    // trace
    tb_trace_d("update contour(at: (%ld, %lu), y: %{fixed}, x: %{fixed}, %{fixed}, slope: %{fixed}, %{fixed})", index, tb_list_entry_size(contours_new), contour->y, contour->le.x, contour->re.x, contour->le.slope, contour->re.slope);
}
static tb_long_t gb_convex_maker_contour_finish(tb_iterator_ref_t iterator, tb_cpointer_t item, tb_cpointer_t priv)
{
    // check
    gb_convex_maker_impl_t* impl = (gb_convex_maker_impl_t*)priv;
    tb_assert_abort(impl);

    // the contour
    gb_convex_maker_contour_ref_t contour = (gb_convex_maker_contour_ref_t)item;
    tb_assert_abort(contour);

    // done contour
    gb_convex_maker_contour_done(impl, contour, tb_null, tb_null);

    // exit contour
    gb_convex_maker_contour_exit(impl, contour);

    // remove it
    return 0;
}
static tb_void_t gb_convex_maker_builder_init(gb_convex_maker_impl_t* impl, gb_rect_ref_t bounds, gb_convex_maker_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(impl && bounds && func);
 
    // save the user func and private data
    impl->func = func;
    impl->priv = priv;

    // init the left x-coordinate
    impl->x_left = gb_round(bounds->x);

    // the active contours size
    tb_size_t contours_active_size = gb_ceil(bounds->w) + 1;
    tb_assert_abort(contours_active_size && contours_active_size <= TB_MAXU16);

    // init the active contours
    tb_bool_t need_init = tb_false;
    if (!impl->contours_active)
    {
        // make it
        impl->contours_active_maxn  = contours_active_size;
        impl->contours_active       = tb_nalloc_type(impl->contours_active_maxn, tb_list_entry_head_t);
        tb_assert_abort(impl->contours_active);

        // init it
        need_init = tb_true;
    }
    // grow the active contours
    else if (contours_active_size > impl->contours_active_maxn)
    {
        // make it
        impl->contours_active_maxn  = contours_active_size;
        impl->contours_active       = tb_ralloc_type(impl->contours_active, impl->contours_active_maxn, tb_list_entry_head_t);
        tb_assert_abort(impl->contours_active);

        /* init it
         *
         * the address of the list head may be changed after reallocing, 
         * so need re-init all contours(index = 0, ...) and rebuild list head
         */
        need_init = tb_true;
    }

    // init the active contours
    tb_size_t index = 0;
    if (need_init)
    {
        for (index = 0; index < contours_active_size; index++)
        {
            // init it
            tb_list_entry_init(impl->contours_active + index, gb_convex_maker_contour_t, entry, tb_null);
        }
    }

#ifdef __gb_debug__
    // check the active contours
    for (index = 0; index < contours_active_size; index++)
    {
        // the contours
        tb_list_entry_head_ref_t contours = impl->contours_active + index;

        // must be valid
        tb_assert_abort(tb_list_entry_is_valid(contours));

        // must be empty
        tb_assert_abort(tb_list_entry_is_null(contours));
    }
#endif

    // save the active contours size
    impl->contours_active_size = contours_active_size;
}
static tb_void_t gb_convex_maker_builder_done(tb_fixed_t y, tb_fixed_t y_next, gb_vertex_raster_edge_ref_t le, gb_vertex_raster_edge_ref_t re, tb_cpointer_t priv)
{
    // check
    gb_convex_maker_impl_t* impl = (gb_convex_maker_impl_t*)priv;
    tb_assert_abort(impl && le && re);

    // the edge factors
    tb_fixed_t  lx      = le->x;
    tb_fixed_t  rx      = re->x;
    tb_fixed_t  lx_next = le->x_next;
    tb_fixed_t  rx_next = re->x_next;
    tb_fixed_t  ls      = le->slope;
    tb_fixed_t  rs      = re->slope;

    // check
    tb_assert_abort(lx <= rx);
       
    // trace
    tb_trace_d("line: y: %{fixed}, %{fixed}, x: %{fixed}, %{fixed}, slope: %{fixed}, %{fixed}, x_next: %{fixed}, %{fixed}, y_bottom: %{fixed}, %{fixed}", y, y_next, lx, rx, ls, rs, lx_next, rx_next, le->y_bottom, re->y_bottom);

    // find the contour of this two edges
    gb_convex_maker_contour_ref_t contour = gb_convex_maker_contour_find(impl, y, lx, rx);
    if (contour)
    {
        // cannot be the first line
        tb_assert_abort(contour->le.points_size || contour->re.points_size);

        // the contour is finished?
        tb_bool_t is_finished = tb_false;

        /* is intersecting join?
         * 
         * .       .
         *   .   .
         *     . <- this join
         *   .   .
         * .       .
         *
         */
        tb_bool_t is_inter = lx == rx;
        if (is_inter)
        { 
            // trace
            tb_trace_d("join: intersecting");

            // finish it 
            is_finished = tb_true;
        }

        // the left-hand point is join?
        tb_bool_t is_join_left = (contour->le.slope != ls) && !is_inter;
        if (is_join_left)
        {
            // trace
            tb_trace_d("join: left");

            /* compute the vector cross in the counter-clockwise direction
             *
             * cross = tb_fixed_mul(-contour->le.slope, TB_FIXED_ONE) - tb_fixed_mul(-TB_FIXED_ONE, ls);
             */
            tb_fixed_t cross = ls - contour->le.slope;

            // is concave? finish it
            if (((tb_hong_t)cross * contour->le.cross) < 0) is_finished = tb_true;

            // save this cross
            contour->le.cross = cross;
        }
        
        // the right-hand point is join?
        tb_bool_t is_join_right = (contour->re.slope != rs) && !is_inter;
        if (is_join_right)
        {
            // trace
            tb_trace_d("join: right");

            /* compute the vector cross in the clockwise direction
             *
             * cross = tb_fixed_mul(-contour->re.slope, TB_FIXED_ONE) - tb_fixed_mul(-TB_FIXED_ONE, rs);
             */
            tb_fixed_t cross = rs - contour->re.slope;

            // is concave? finish it
            if (((tb_hong_t)cross * contour->re.cross) < 0) is_finished = tb_true;

            // save this cross
            contour->re.cross = cross;
        }

#ifdef GB_CONVEX_MAKER_TEST_EDGE
        tb_fixed_t x_inter = 0;
        tb_fixed_t y_inter = 0;
        {
            // only append all edge points
            is_inter        = tb_false;
            is_join_left    = tb_false;
            is_join_right   = tb_false;       
            gb_convex_maker_contour_append_l(contour, lx, y);
            gb_convex_maker_contour_append_r(contour, rx, y);
        }
#else
        // the contour is finished? 
        tb_fixed_t x_inter;
        tb_fixed_t y_inter;
        if (is_finished)
        {
            // done it and clear points for inserting a new contour
            is_inter = gb_convex_maker_contour_done(impl, contour, &x_inter, &y_inter);

            // patch the joins if be not intersection after doing maker
            if (!is_inter)
            {
                is_join_left = tb_true;
                is_join_right = tb_true;
            }
        }
#endif

        // is intersection?
        if (is_inter)
        {
            // append the intersection
            gb_convex_maker_contour_append_r(contour, x_inter, y_inter);
        }
        else
        {
            // the left-hand point is join?
            if (is_join_left)
            {
                // append the left-hand point to the new contour
                gb_convex_maker_contour_append_l(contour, lx, y);
            }

            // the right-hand point is join?
            if (is_join_right)
            {
                // append the right-hand point to the new contour
                gb_convex_maker_contour_append_r(contour, rx, y);
            }
        }

        // update the new contour
        contour->y              = y;
        contour->y_next         = y_next;
        contour->le.x           = lx;
        contour->re.x           = rx;
        contour->le.slope       = ls;
        contour->re.slope       = rs;
        contour->le.x_next      = lx_next;
        contour->re.x_next      = rx_next;
        contour->le.y_bottom    = le->y_bottom;
        contour->re.y_bottom    = re->y_bottom;
        gb_convex_maker_contour_update(impl, contour);        
    }
    // not found? add a new contour
    else
    {
        // make a new contour
        contour = gb_convex_maker_contour_init(impl);
        tb_assert_abort(contour);

        // init the new contour
        contour->y              = y;
        contour->y_next         = y_next;
        contour->le.x           = lx;
        contour->re.x           = rx;
        contour->le.slope       = ls;
        contour->re.slope       = rs;
        contour->le.x_next      = lx_next;
        contour->re.x_next      = rx_next;
        contour->le.y_bottom    = le->y_bottom;
        contour->re.y_bottom    = re->y_bottom;
  
        // append points to the contour
        gb_convex_maker_contour_append_l(contour, lx, y);
        gb_convex_maker_contour_append_r(contour, rx, y);


        // insert the new contour
        gb_convex_maker_contour_insert(impl, contour);
    }
}
static tb_void_t gb_convex_maker_builder_exit(gb_convex_maker_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // check the active contours
    tb_size_t index = 0;
    tb_size_t count = impl->contours_active_size;
    for (index = 0; index < count; index++)
    {
        // the active contours
        tb_list_entry_head_ref_t contours = impl->contours_active + index;

        // empty?
        tb_check_continue(tb_list_entry_size(contours));

        // trace
        tb_trace_d("finished contours: %lu", tb_list_entry_size(contours));

        // finish them
        tb_remove_if(tb_list_entry_itor(contours), gb_convex_maker_contour_finish, (tb_pointer_t)impl);

        // check
        tb_assert_abort(tb_list_entry_is_valid(contours));
        tb_assert_abort(tb_list_entry_is_null(contours));
    }

    // clear the active contours size
    impl->contours_active_size = 0;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_convex_maker_ref_t gb_convex_maker_init()
{
    // done
    tb_bool_t                   ok = tb_false;
    gb_convex_maker_impl_t*   impl = tb_null;
    do
    {
        // make maker
        impl = tb_malloc0_type(gb_convex_maker_impl_t);
        tb_assert_and_check_break(impl);

        // init raster
        impl->raster = gb_vertex_raster_init();
        tb_assert_and_check_break(impl->raster);

        // init the contours
        impl->contours = tb_fixed_pool_init(tb_null, GB_CONVEX_MAKER_CONTOURS_GROW, sizeof(gb_convex_maker_contour_t), tb_null, gb_convex_maker_contour_free, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->contours);

        // init the free contours
        tb_list_entry_init(&impl->contours_free, gb_convex_maker_contour_t, entry, tb_null);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_convex_maker_exit((gb_convex_maker_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_convex_maker_ref_t)impl;
}
tb_void_t gb_convex_maker_exit(gb_convex_maker_ref_t maker)
{
    // check
    gb_convex_maker_impl_t* impl = (gb_convex_maker_impl_t*)maker;
    tb_assert_and_check_return(impl);

    // exit active contours
    if (impl->contours_active) tb_free(impl->contours_active);
    impl->contours_active = tb_null;

    // exit free contours
    tb_list_entry_exit(&impl->contours_free);

    // exit the contours
    if (impl->contours) tb_fixed_pool_exit(impl->contours);
    impl->contours = tb_null;

    // exit the raster
    if (impl->raster) gb_vertex_raster_exit(impl->raster);
    impl->raster = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_convex_maker_done(gb_convex_maker_ref_t maker, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_convex_maker_func_t func, tb_cpointer_t priv)
{
    // check
    gb_convex_maker_impl_t* impl = (gb_convex_maker_impl_t*)maker;
    tb_assert_and_check_return(impl && impl->raster && polygon && func);

    // is convex polygon for each contour?
    if (polygon->convex)
    {
        // done
        tb_size_t       index   = 0;
        gb_point_ref_t  points  = polygon->points;
        tb_uint16_t*    counts  = polygon->counts;
        tb_uint16_t     count   = 0;
        while ((count = *counts++))
        {
            // done it
            func(points + index, count, priv);

            // update the contour index
            index += count;
        }
    }
    else
    {
        // init builder
        gb_convex_maker_builder_init(impl, bounds, func, priv);

        // done raster and reduce the complex polygon to the some convex polygons
        gb_vertex_raster_done(impl->raster, polygon, bounds, rule, gb_convex_maker_builder_done, (tb_cpointer_t)impl);

        // exit builder
        gb_convex_maker_builder_exit(impl);
    }
}
