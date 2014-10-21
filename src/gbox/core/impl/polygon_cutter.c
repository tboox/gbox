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
 * @file        polygon_cutter.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "polygon_cutter"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "polygon_cutter.h"
#include "polygon_raster.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the polygon points grow
#ifdef __gb_small__
#   define GB_POLYGON_CUTTER_POINTS_GROW        (16)
#else
#   define GB_POLYGON_CUTTER_POINTS_GROW        (32)
#endif

// the polygon contours grow
#ifdef __gb_small__
#   define GB_POLYGON_CUTTER_CONTOURS_GROW      (8)
#else
#   define GB_POLYGON_CUTTER_CONTOURS_GROW      (16)
#endif

// test the polygon edge
//#define GB_POLYGON_CUTTER_TEST_EDGE

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the polygon contour type
typedef struct __gb_polygon_cutter_contour_t
{
    // the current y-coordinate
    tb_long_t                       y;

    // the current left-hand x-coordinate
    tb_fixed_t                      lx;

    // the current right-hand x-coordinate
    tb_fixed_t                      rx;

    // the current left-hand slope: dx / dy
    tb_fixed_t                      ls;

    // the current right-hand slope: dx / dy
    tb_fixed_t                      rs;

    // the bottom left-hand dy value: (y_top - round(y_top)), range: [-0.5-0.5]
    tb_fixed_t                      ldy;
    
    // the bottom right-hand dy value: (y_top - round(y_top)), range: [-0.5-0.5]
    tb_fixed_t                      rdy;

    // the bottom left-hand x-coordinate
    tb_fixed_t                      lxe;

    // the bottom right-hand x-coordinate
    tb_fixed_t                      rxe;

    // the bottom left-hand y-coordinate
    tb_int16_t                      lye;

    // the bottom right-hand y-coordinate
    tb_int16_t                      rye;

    // the left-hand points
    gb_point_ref_t                  lp;

    // the left-hand points maxn
    tb_uint16_t                     lp_maxn;

    // the left-hand points count
    tb_uint16_t                     lp_count;

    // the right-hand points
    gb_point_ref_t                  rp;

    // the right-hand points maxn
    tb_uint16_t                     rp_maxn;

    // the right-hand points count
    tb_uint16_t                     rp_count;

    // the active contour index
    tb_uint16_t                     index;

    // the list entry
    tb_list_entry_t                 entry;

}gb_polygon_cutter_contour_t, *gb_polygon_cutter_contour_ref_t;

// the polygon cutter impl type
typedef struct __gb_polygon_cutter_impl_t
{
    // the raster
    gb_polygon_raster_ref_t         raster;

    // the contours
    tb_fixed_pool_ref_t             contours;

    // the free contours
    tb_list_entry_head_t            contours_free;

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
    tb_list_entry_head_t*           contours_active;

    // the active contours maxn
    tb_size_t                       contours_active_maxn;

    // the active contours size
    tb_size_t                       contours_active_size;

    // the left-hand x-coordinate of the polygon
    tb_long_t                       left_x;

    // the user cutter func
    gb_polygon_cutter_func_t        func;

    // the user private data
    tb_cpointer_t                   priv;

}gb_polygon_cutter_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_polygon_cutter_contour_free(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_polygon_cutter_contour_ref_t contour = (gb_polygon_cutter_contour_ref_t)data;
    tb_assert_abort(contour);

    // exit the left-hand points
    if (contour->lp) tb_free(contour->lp);
    contour->lp = tb_null;

    // exit the right-hand points
    if (contour->rp) tb_free(contour->rp);
    contour->rp = tb_null;
}
static tb_void_t gb_polygon_cutter_contour_exit(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && contour);

    // trace
    tb_trace_d("exit contour: lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed}", contour->lx, contour->rx, contour->ls, contour->rs);
            
    // put the contour to the free contours
    tb_list_entry_insert_head(&impl->contours_free, &contour->entry);
}
static gb_polygon_cutter_contour_ref_t gb_polygon_cutter_contour_init(gb_polygon_cutter_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->contours);

    // done
    tb_bool_t                       ok  = tb_false;
    gb_polygon_cutter_contour_ref_t contour = tb_null;
    do
    {
        // attempt to get contour from the free contours
        if (tb_list_entry_size(&impl->contours_free))
        {
            tb_list_entry_ref_t entry = tb_list_entry_head(&impl->contours_free);
            if (entry)
            {
                contour = (gb_polygon_cutter_contour_ref_t)tb_list_entry(&impl->contours_free, entry);
                tb_list_entry_remove_head(&impl->contours_free);
            }
        }

        // get contour from the contour pool
        if (!contour) contour = tb_fixed_pool_malloc0(impl->contours);
        tb_assert_and_check_break(contour);

        // init contour
        contour->y      = 0;
        contour->lx     = 0;
        contour->rx     = 0;
        contour->ls     = 0;
        contour->rs     = 0;
        contour->ldy    = 0;
        contour->rdy    = 0;
        contour->lxe    = 0;
        contour->rxe    = 0;
        contour->lye    = 0;
        contour->rye    = 0;
        contour->index  = 0;

        // init the left-hand points
        contour->lp_count = 0;
        if (!contour->lp) 
        {
            contour->lp_maxn    = GB_POLYGON_CUTTER_POINTS_GROW;
            contour->lp         = tb_nalloc_type(contour->lp_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->lp);

        // init the right-hand points
        contour->rp_count = 0;
        if (!contour->rp) 
        {
            // ensure the enough space for making complete contour: rp = rp + reverse(lp)
            contour->rp_maxn    = GB_POLYGON_CUTTER_POINTS_GROW << 1;
            contour->rp         = tb_nalloc_type(contour->rp_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->rp);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (contour) gb_polygon_cutter_contour_exit(impl, contour);
        contour = tb_null;
    }

    // ok?
    return contour;
}
static tb_void_t gb_polygon_cutter_contour_curt(gb_polygon_cutter_impl_t* impl, gb_polygon_raster_edge_ref_t edge_lsh, gb_polygon_raster_edge_ref_t edge_rsh, tb_fixed_t* plx, tb_fixed_t* prx)
{
    // check
    tb_assert_abort(impl && edge_lsh && edge_rsh && plx && prx);

    // is top line for edges?
    tb_bool_t lt = edge_lsh->is_top;
    tb_bool_t rt = edge_rsh->is_top;

    // the next x-coordinates
    tb_fixed_t lx = edge_lsh->x;
    tb_fixed_t rx = edge_rsh->x;

    // patch to the real x-coordinates for the top line
    if (lt && rt) 
    {
        lx += tb_fixed_mul(edge_lsh->dy_top, edge_lsh->slope);
        rx += tb_fixed_mul(edge_rsh->dy_top, edge_rsh->slope);
    }
    else if (lt) 
    {
        lx += tb_fixed_mul(edge_lsh->dy_top, edge_lsh->slope);
        rx += tb_fixed_mul(edge_lsh->dy_top, edge_rsh->slope);
    }
    else if (rt) 
    { 
        lx += tb_fixed_mul(edge_rsh->dy_top, edge_lsh->slope);
        rx += tb_fixed_mul(edge_rsh->dy_top, edge_rsh->slope);
    }

    // save the x-coordinates
    *plx = lx;
    *prx = rx;
}
static tb_void_t gb_polygon_cutter_contour_next(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour, tb_fixed_t* plx, tb_fixed_t* prx)
{
    // check
    tb_assert_abort(impl && contour && plx && prx);

    // the slopes
    tb_fixed_t ls = contour->ls;
    tb_fixed_t rs = contour->rs;

    // the y-coordinates
    tb_int16_t y    = contour->y;
    tb_int16_t lye  = contour->lye;
    tb_int16_t rye  = contour->rye;

    // the next x-coordinates
    tb_fixed_t lx = contour->lx + ls;
    tb_fixed_t rx = contour->rx + rs;

    // patch to the real x-coordinates for joins
    if (y == lye && y == rye) 
    {
        lx += tb_fixed_mul(contour->ldy, ls);
        rx += tb_fixed_mul(contour->rdy, rs);
    }
    else if (y == lye) 
    {
        lx += tb_fixed_mul(contour->ldy, ls);
        rx += tb_fixed_mul(contour->ldy, rs);
    }
    else if (y == rye) 
    { 
        lx += tb_fixed_mul(contour->rdy, ls);
        rx += tb_fixed_mul(contour->rdy, rs);
    }

    // save the x-coordinates
    *plx = lx;
    *prx = rx;
}
static __tb_inline__ tb_long_t gb_polygon_cutter_contour_indx(gb_polygon_cutter_impl_t* impl, tb_long_t xb)
{
    // check
    tb_assert_abort(impl);

    // compute the active contours index
    tb_long_t index = xb - impl->left_x;
    if (index < 0) index = 0;
    if (index >= impl->contours_active_size) index = impl->contours_active_size - 1;
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

    // ok
    return index;
}
static gb_polygon_cutter_contour_ref_t gb_polygon_cutter_contour_find_at(gb_polygon_cutter_impl_t* impl, tb_long_t index, tb_long_t y)
{
    // check
    tb_assert_abort(impl && impl->contours_active);
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // empty? 
    gb_polygon_cutter_contour_ref_t contour = tb_null;
    if (tb_list_entry_size(contours))
    {
        // find it
        tb_for_all_if (gb_polygon_cutter_contour_ref_t, item, tb_list_entry_itor(contours), item)
        {
            // only one contour at this index for the same line
            if (item->y + 1 == y)
            {
                // save it
                contour = item;

                // ok
                break;
            }
        }
    }

    // trace
    tb_trace_d("find contour(at: (%ld, %lu), y: %ld: %s", index, tb_list_entry_size(contours), y, contour? "ok" : "no");

    // ok?
    return contour;
}
static gb_polygon_cutter_contour_ref_t gb_polygon_cutter_contour_find(gb_polygon_cutter_impl_t* impl, tb_long_t y, gb_polygon_raster_edge_ref_t edge_lsh, gb_polygon_raster_edge_ref_t edge_rsh)
{
    // check
    tb_assert_abort(impl && impl->contours_active && edge_lsh && edge_rsh);

    // compute the current x-coordinates
    tb_fixed_t lx;
    tb_fixed_t rx;
    gb_polygon_cutter_contour_curt(impl, edge_lsh, edge_rsh, &lx, &rx);

    // compute the current contour index
    tb_long_t index = gb_polygon_cutter_contour_indx(impl, tb_fixed_round(tb_fixed_avg(lx, rx)));

    // find the contour in the current index
    gb_polygon_cutter_contour_ref_t contour = gb_polygon_cutter_contour_find_at(impl, index, y);
    if (!contour)
    {
        /* find the contour in the range: [index - 1, index + 1]
         *
         * because the fixed_round() exists some error 
         */
        if (index > 1) contour = gb_polygon_cutter_contour_find_at(impl, index - 1, y);
        if (!contour && index < impl->contours_active_size - 1)
            contour = gb_polygon_cutter_contour_find_at(impl, index + 1, y);
    }

    // ok?
    return contour;
}
static tb_void_t gb_polygon_cutter_contour_grow_l(gb_polygon_cutter_contour_ref_t contour, tb_size_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the left-hand points if not enough 
    if (count > contour->lp_maxn)
    {
        contour->lp_maxn    = count + GB_POLYGON_CUTTER_POINTS_GROW;
        contour->lp         = tb_ralloc_type(contour->lp, contour->lp_maxn, gb_point_t);
        tb_assert_abort(contour->lp);
    }
}
static tb_void_t gb_polygon_cutter_contour_grow_r(gb_polygon_cutter_contour_ref_t contour, tb_size_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the right-hand points if not enough 
    if (count > contour->rp_maxn)
    {
        contour->rp_maxn    = count + (GB_POLYGON_CUTTER_POINTS_GROW << 1);
        contour->rp         = tb_ralloc_type(contour->rp, contour->rp_maxn, gb_point_t);
        tb_assert_abort(contour->rp);
    }
}
static tb_void_t gb_polygon_cutter_contour_append_l(gb_polygon_cutter_contour_ref_t contour, tb_long_t y, tb_fixed_t lx)
{
    // check
    tb_assert_abort(contour && contour->lp_count < TB_MAXU16);

    // grow the left-hand points if not enough 
    gb_polygon_cutter_contour_grow_l(contour, contour->lp_count + 1);

    // append the left-hand point
    gb_point_make(&contour->lp[contour->lp_count++], gb_fixed_to_float(lx), gb_long_to_float(y));
}
static tb_void_t gb_polygon_cutter_contour_append_r(gb_polygon_cutter_contour_ref_t contour, tb_long_t y, tb_fixed_t rx)
{
    // check
    tb_assert_abort(contour && contour->rp_count < TB_MAXU16);

    // grow the right-hand points if not enough 
    gb_polygon_cutter_contour_grow_r(contour, contour->rp_count + 1);

    // append the right-hand point
    gb_point_make(&contour->rp[contour->rp_count++], gb_fixed_to_float(rx), gb_long_to_float(y));
}
static tb_void_t gb_polygon_cutter_contour_append(gb_polygon_cutter_contour_ref_t contour, tb_long_t y, tb_fixed_t lx, tb_fixed_t rx)
{
    // check
    tb_assert_abort(contour && lx <= rx);

    // append points
    if (lx != rx) 
    {
        // append the left-hand point
        gb_polygon_cutter_contour_append_l(contour, y, lx);
    }

    // append the right-hand point
    gb_polygon_cutter_contour_append_r(contour, y, rx);
}
static tb_void_t gb_polygon_cutter_contour_done(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->func && contour && contour->lp && contour->rp);

    // compute the next x-coordinates
    tb_fixed_t lx = contour->lxe;
    tb_fixed_t rx = contour->rxe;

    // add points to the contour
    if (lx > rx) gb_polygon_cutter_contour_append(contour, contour->y, contour->lx, contour->rx);
    else gb_polygon_cutter_contour_append(contour, contour->y + 1, lx, rx);

    // grow the right-hand points if not enough 
    gb_polygon_cutter_contour_grow_r(contour, (tb_size_t)contour->rp_count + contour->lp_count + 1);

    // TODO: optimization
    // reverse to append the left-hand-hand points to the right-hand points
    gb_point_ref_t  rp = contour->rp;
    gb_point_ref_t  lp = contour->lp;
    tb_uint16_t     rp_count = contour->rp_count;
    tb_uint16_t     lp_count = contour->lp_count;
    while (lp_count--)
    {
        rp[rp_count++] = lp[lp_count];
    }

    // close it
    if (rp[rp_count - 1].x != rp[0].x || rp[rp_count - 1].y != rp[0].y)
        rp[rp_count++] = rp[0];

    // check
    tb_assert_abort(rp_count <= contour->rp_count + contour->lp_count + 1);

    // trace
    tb_trace_d("done contour(at: (%ld), y: %ld, lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed})", contour->index, contour->y, contour->lx, contour->rx, contour->ls, contour->rs);

    // done func
    impl->func(contour->rp, rp_count, impl->priv);

    // clear points
    contour->lp_count = 0;
    contour->rp_count = 0;
}
static tb_void_t gb_polygon_cutter_contour_insert(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the next x-coordinates
    tb_fixed_t lx;
    tb_fixed_t rx;
    gb_polygon_cutter_contour_next(impl, contour, &lx, &rx);

    // compute the contour next index
    tb_long_t index = gb_polygon_cutter_contour_indx(impl, tb_fixed_round(tb_fixed_avg(lx, rx)));

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // insert this contour
    tb_list_entry_insert_head(contours, &contour->entry);

    // save the active contours index
    contour->index = (tb_uint16_t)index;

    // save the bottom x-coordinates
    contour->lxe = lx;
    contour->rxe = rx;

    // trace
    tb_trace_d("insert contour(at: (%ld, %lu), y: %ld, lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed})", index, tb_list_entry_size(contours), contour->y, contour->lx, contour->rx, contour->ls, contour->rs);
}
static tb_void_t gb_polygon_cutter_contour_update(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the next x-coordinates
    tb_fixed_t lx;
    tb_fixed_t rx;
    gb_polygon_cutter_contour_next(impl, contour, &lx, &rx);

    // compute the contour next index
    tb_long_t index = gb_polygon_cutter_contour_indx(impl, tb_fixed_round(tb_fixed_avg(lx, rx)));

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

    // save the bottom x-coordinates
    contour->lxe = lx;
    contour->rxe = rx;

    // trace
    tb_trace_d("update contour(at: (%ld, %lu), y: %ld, lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed})", index, tb_list_entry_size(contours_new), contour->y, contour->lx, contour->rx, contour->ls, contour->rs);
}
static tb_void_t gb_polygon_cutter_builder_init(gb_polygon_cutter_impl_t* impl, gb_rect_ref_t bounds, gb_polygon_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(impl && bounds && func);
 
    // save the user func and private data
    impl->func = func;
    impl->priv = priv;

    // init the left-hand-hand x-coordinate
    impl->left_x = gb_round(bounds->x);

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
            tb_list_entry_init(impl->contours_active + index, gb_polygon_cutter_contour_t, entry, tb_null);
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
static tb_void_t gb_polygon_cutter_builder_done(tb_long_t yb, tb_long_t ye, gb_polygon_raster_edge_ref_t edge_lsh, gb_polygon_raster_edge_ref_t edge_rsh, tb_cpointer_t priv)
{
    // check
    gb_polygon_cutter_impl_t* impl = (gb_polygon_cutter_impl_t*)priv;
    tb_assert_abort(impl && edge_lsh && edge_rsh);

    // only one line
    tb_assert_abort(yb < TB_MAXS16 && yb + 1 == ye);

    // the edge factors
    tb_fixed_t  lx = edge_lsh->x;
    tb_fixed_t  rx = edge_rsh->x;
    tb_fixed_t  ls = edge_lsh->slope;
    tb_fixed_t  rs = edge_rsh->slope;
    tb_assert_abort(rx >= lx);

    // trace
    tb_trace_d("line: yb: %ld, lx: %{fixed}, rx: %{fixed}, ls: %{fixed}, rs: %{fixed}, y_bottom: %d %d", yb, lx, rx, ls, rs, edge_lsh->y_bottom, edge_rsh->y_bottom);

    // find the contour of this two edges
    gb_polygon_cutter_contour_ref_t contour = gb_polygon_cutter_contour_find(impl, yb, edge_lsh, edge_rsh);
    if (contour)
    {
        // the x-coordinates is same?
        tb_bool_t is_same_x = lx == rx;

        // is the first line?
        tb_bool_t is_first = !contour->lp_count && !contour->rp_count;

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
        tb_bool_t is_inter = is_same_x || (contour->lx + contour->ls) > (contour->rx + contour->rs);
        if (is_inter)
        { 
            // trace
            tb_trace_d("join: intersecting");

            // is finished 
            if (!is_first) is_finished = tb_true;
        }

        // the left-hand point is join?
        tb_bool_t is_join_left = (contour->ls != ls) && !is_inter;
        if (is_join_left)
        {
            // trace
            tb_trace_d("join: left");

            // analyze the contour
            if (!is_first)
            {
                // is finished 
                if (ls < 0) is_finished = tb_true;
            }
        }
        
        // the right-hand point is join?
        tb_bool_t is_join_right = (contour->rs != rs) && !is_inter;
        if (is_join_right)
        {
            // trace
            tb_trace_d("join: right");

            // analyze the contour
            if (!is_first)
            {
                // is finished 
                if (rs > 0) is_finished = tb_true;
            }
        }

#ifdef GB_POLYGON_CUTTER_TEST_EDGE
        // only append all edge points
        is_inter        = tb_false;
        is_join_left    = tb_false;
        is_join_right   = tb_false;       
        gb_polygon_cutter_contour_append(contour, yb, lx, rx);
#else
        // the contour is finished? 
        if (is_finished)
        {
            // done it and clear points for inserting a new contour
            gb_polygon_cutter_contour_done(impl, contour);

            // patch the joins if be not inserting point after doing cutter
            if (!is_inter)
            {
                is_join_left = tb_true;
                is_join_right = tb_true;
            }
        }
#endif
 
        // is intersecting point?
        if (is_inter)
        {
            // append points to the new contour
            if (is_same_x) gb_polygon_cutter_contour_append_r(contour, yb, rx);
            else gb_polygon_cutter_contour_append(contour, yb, lx, rx);
        }

        // the left-hand point is join?
        if (is_join_left)
        {
            // append the left-hand point to the new contour
            gb_polygon_cutter_contour_append_l(contour, yb, lx);
        }

        // the right-hand point is join?
        if (is_join_right)
        {
            // append the right-hand point to the new contour
            gb_polygon_cutter_contour_append_r(contour, yb, rx);
        }

        // update the new contour
        contour->y      = yb;
        contour->lx     = lx;
        contour->rx     = rx;
        contour->ls     = ls;
        contour->rs     = rs;
        contour->ldy    = edge_lsh->dy_bottom;
        contour->rdy    = edge_rsh->dy_bottom;
        contour->lye    = edge_lsh->y_bottom;
        contour->rye    = edge_rsh->y_bottom;
        gb_polygon_cutter_contour_update(impl, contour);        
    }
    // not found? add a new contour
    else
    {
        // make a new contour
        contour = gb_polygon_cutter_contour_init(impl);
        tb_assert_abort(contour);

        // init the new contour
        contour->y      = yb;
        contour->lx     = lx;
        contour->rx     = rx;
        contour->ls     = ls;
        contour->rs     = rs; 
        contour->ldy    = edge_lsh->dy_bottom;
        contour->rdy    = edge_rsh->dy_bottom;
        contour->lye    = edge_lsh->y_bottom;
        contour->rye    = edge_rsh->y_bottom;

        // append points to the contour
        gb_polygon_cutter_contour_append(contour, yb, lx, rx);

        // insert the new contour
        gb_polygon_cutter_contour_insert(impl, contour);
    }
}
static tb_void_t gb_polygon_cutter_builder_exit(gb_polygon_cutter_impl_t* impl)
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

        // TODO: using tb_remove_if
        // the active iterator
        tb_iterator_ref_t iterator = tb_list_entry_itor(contours);
        tb_assert_abort(iterator);

        // done
        tb_size_t itor = tb_iterator_head(iterator);
        while (itor != tb_iterator_tail(iterator))
        {
            // the contour
            gb_polygon_cutter_contour_ref_t contour = (gb_polygon_cutter_contour_ref_t)tb_iterator_item(iterator, itor);
            tb_assert_abort(contour && contour->rx >= contour->lx);

            // save next
            tb_size_t next = tb_iterator_next(iterator, itor);

            // done contour
            gb_polygon_cutter_contour_done(impl, contour);

            // remove contour
            tb_list_entry_remove(contours, &contour->entry);

            // exit contour
            gb_polygon_cutter_contour_exit(impl, contour);

            // next
            itor = next;
        }

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
gb_polygon_cutter_ref_t gb_polygon_cutter_init()
{
    // done
    tb_bool_t                   ok = tb_false;
    gb_polygon_cutter_impl_t*   impl = tb_null;
    do
    {
        // make cutter
        impl = tb_malloc0_type(gb_polygon_cutter_impl_t);
        tb_assert_and_check_break(impl);

        // init raster
        impl->raster = gb_polygon_raster_init();
        tb_assert_and_check_break(impl->raster);

        // init the contours
        impl->contours = tb_fixed_pool_init(tb_null, GB_POLYGON_CUTTER_CONTOURS_GROW, sizeof(gb_polygon_cutter_contour_t), tb_null, gb_polygon_cutter_contour_free, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->contours);

        // init the free contours
        tb_list_entry_init(&impl->contours_free, gb_polygon_cutter_contour_t, entry, tb_null);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_polygon_cutter_exit((gb_polygon_cutter_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_polygon_cutter_ref_t)impl;
}
tb_void_t gb_polygon_cutter_exit(gb_polygon_cutter_ref_t cutter)
{
    // check
    gb_polygon_cutter_impl_t* impl = (gb_polygon_cutter_impl_t*)cutter;
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
    if (impl->raster) gb_polygon_raster_exit(impl->raster);
    impl->raster = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_polygon_cutter_done(gb_polygon_cutter_ref_t cutter, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule, gb_polygon_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    gb_polygon_cutter_impl_t* impl = (gb_polygon_cutter_impl_t*)cutter;
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
        gb_polygon_cutter_builder_init(impl, bounds, func, priv);

        // done raster and reduce the complex polygon to the some convex polygons
        gb_polygon_raster_done(impl->raster, polygon, bounds, rule, gb_polygon_cutter_builder_done, (tb_cpointer_t)impl);

        // exit builder
        gb_polygon_cutter_builder_exit(impl);
    }
}
