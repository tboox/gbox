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
#define TB_TRACE_MODULE_DEBUG           (1)

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

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the polygon contour type
typedef struct __gb_polygon_cutter_contour_t
{
    // the current y-coordinate
    tb_long_t                       y;

    // the current start x-coordinate
    tb_fixed_t                      xb;

    // the current end x-coordinate
    tb_fixed_t                      xe;

    // the current start slope: dx / dy
    tb_fixed_t                      sb;

    // the current end slope: dx / dy
    tb_fixed_t                      se;

    // the start points
    gb_point_ref_t                  pb;

    // the start points maxn
    tb_uint16_t                     pb_maxn;

    // the start points count
    tb_uint16_t                     pb_count;

    // the end points
    gb_point_ref_t                  pe;

    // the end points maxn
    tb_uint16_t                     pe_maxn;

    // the end points count
    tb_uint16_t                     pe_count;

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
     * xb: |         |         |         |              ...             |
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

    // the left x-coordinate of the polygon
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

    // exit the start points
    if (contour->pb) tb_free(contour->pb);
    contour->pb = tb_null;

    // exit the end points
    if (contour->pe) tb_free(contour->pe);
    contour->pe = tb_null;
}
static tb_void_t gb_polygon_cutter_contour_exit(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && contour);

    // trace
    tb_trace_d("exit contour: xb: %{fixed}, xe: %{fixed}, sb: %{fixed}, se: %{fixed}", contour->xb, contour->xe, contour->sb, contour->se);
            
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
        contour->xb     = 0;
        contour->xe     = 0;
        contour->sb     = 0;
        contour->se     = 0;
        contour->index  = 0;

        // init the start points
        contour->pb_count = 0;
        if (!contour->pb) 
        {
            contour->pb_maxn    = GB_POLYGON_CUTTER_POINTS_GROW;
            contour->pb         = tb_nalloc_type(contour->pb_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->pb);

        // init the end points
        contour->pe_count = 0;
        if (!contour->pe) 
        {
            // ensure the enough space for making complete contour: pe = pe + reverse(pb)
            contour->pe_maxn    = GB_POLYGON_CUTTER_POINTS_GROW << 1;
            contour->pe         = tb_nalloc_type(contour->pb_maxn, gb_point_t);
        }
        tb_assert_and_check_break(contour->pe);

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
static gb_polygon_cutter_contour_ref_t gb_polygon_cutter_contour_find(gb_polygon_cutter_impl_t* impl, tb_long_t y, tb_fixed_t xb, tb_fixed_t xe)
{
    // check
    tb_assert_abort(impl && impl->contours_active);

    // compute the active contours index
    tb_long_t index = tb_fixed_round(xb) - impl->left_x;
    tb_assert_abort(index >= 0 && index < impl->contours_active_maxn);

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // empty? not found
    tb_check_return_val(tb_list_entry_size(contours), tb_null);

    // find it
    gb_polygon_cutter_contour_ref_t contour = tb_null;
    tb_for_all_if (gb_polygon_cutter_contour_ref_t, item, tb_list_entry_itor(contours), item)
    {
        // found it?
        if (item->y == y && item->xb == xb && item->xe == xe)
        {
            // save it
            contour = item;

            // ok
            break;
        }
    }

    // trace
    tb_trace_d("find contour(y: %ld, xb: %{fixed}, xe: %{fixed}): %s", y, xb, xe, contour? "ok" : "no");

    // ok?
    return contour;
}
static tb_void_t gb_polygon_cutter_contour_grow_b(gb_polygon_cutter_contour_ref_t contour, tb_size_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the start points if not enough 
    if (count > contour->pb_maxn)
    {
        contour->pb_maxn    = count + GB_POLYGON_CUTTER_POINTS_GROW;
        contour->pb         = tb_ralloc_type(contour->pb, contour->pb_maxn, gb_point_t);
        tb_assert_abort(contour->pb);
    }
}
static tb_void_t gb_polygon_cutter_contour_grow_e(gb_polygon_cutter_contour_ref_t contour, tb_size_t count)
{
    // check
    tb_assert_abort(contour && count <= TB_MAXU16);

    // grow the end points if not enough 
    if (count > contour->pe_maxn)
    {
        contour->pe_maxn    = count + (GB_POLYGON_CUTTER_POINTS_GROW << 1);
        contour->pe         = tb_ralloc_type(contour->pe, contour->pe_maxn, gb_point_t);
        tb_assert_abort(contour->pe);
    }
}
static tb_void_t gb_polygon_cutter_contour_append(gb_polygon_cutter_contour_ref_t contour, tb_long_t y, tb_fixed_t xb, tb_fixed_t xe)
{
    // check
    tb_assert_abort(contour && contour->pb_count < TB_MAXU16 && contour->pe_count < TB_MAXU16);
    tb_assert_abort(xb <= xe);

    // append points
    if (xb != xe) 
    {
        // grow the start points if not enough 
        gb_polygon_cutter_contour_grow_b(contour, contour->pb_count + 1);

        // append the start point
        gb_point_make(&contour->pb[contour->pb_count++], gb_fixed_to_float(xb), gb_long_to_float(y));
    }

    // grow the end points if not enough 
    gb_polygon_cutter_contour_grow_e(contour, contour->pe_count + 1);

    // append the end point
    gb_point_make(&contour->pe[contour->pe_count++], gb_fixed_to_float(xe), gb_long_to_float(y));
}
static tb_void_t gb_polygon_cutter_contour_done(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->func && contour && contour->pb && contour->pe);

    // the x-coordinates and slopes
    tb_fixed_t xb = contour->xb;
    tb_fixed_t xe = contour->xe;
    tb_fixed_t sb = contour->sb;
    tb_fixed_t se = contour->se;

    // add points to the contour
    if (xb + sb > xe + se) gb_polygon_cutter_contour_append(contour, contour->y, xb, xe);
    else gb_polygon_cutter_contour_append(contour, contour->y + 1, xb + sb, xe + se);

    // grow the end points if not enough 
    gb_polygon_cutter_contour_grow_e(contour, (tb_size_t)contour->pe_count + contour->pb_count + 1);

    // TODO: optimization
    // reverse to append the start points to the end points
    gb_point_ref_t  pe = contour->pe;
    gb_point_ref_t  pb = contour->pb;
    tb_uint16_t     pe_count = contour->pe_count;
    tb_uint16_t     pb_count = contour->pb_count;
    while (pb_count--)
    {
        pe[pe_count++] = pb[pb_count];
    }

    // close it
    pe[pe_count++] = pe[0];

    // check
    tb_assert_abort(pe_count == contour->pe_count + contour->pb_count + 1);

    // trace
    tb_trace_d("done contour(y: %ld, xb: %{fixed}, xe: %{fixed})", contour->y, contour->xb, contour->xe);

    // done func
    impl->func(contour->pe, pe_count, impl->priv);
}
static tb_void_t gb_polygon_cutter_contour_insert(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the active contours index
    tb_long_t index = tb_fixed_round(contour->xb) - impl->left_x;
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

    // the active contours 
    tb_list_entry_head_ref_t contours = impl->contours_active + index;

    // insert this contour
    tb_list_entry_insert_head(contours, &contour->entry);

    // save the active contours index
    contour->index = (tb_uint16_t)index;

    // trace
    tb_trace_d("insert contour(y: %ld, xb: %{fixed}, xe: %{fixed})", contour->y, contour->xb, contour->xe);
}
static tb_void_t gb_polygon_cutter_contour_update(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && impl->contours_active && contour);

    // compute the new active contours index
    tb_long_t index = tb_fixed_round(contour->xb) - impl->left_x;
    tb_assert_abort(index >= 0 && index < impl->contours_active_size);

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
    tb_trace_d("update contour(y: %ld, xb: %{fixed}, xe: %{fixed})", contour->y, contour->xb, contour->xe);
}
static tb_void_t gb_polygon_cutter_builder_init(gb_polygon_cutter_impl_t* impl, gb_rect_ref_t bounds, gb_polygon_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(impl && bounds && func);
 
    // save the user func and private data
    impl->func = func;
    impl->priv = priv;

    // init the left x-coordinate
    impl->left_x = gb_round(bounds->x);

    // the active contours size
    tb_size_t contours_active_size = gb_ceil(bounds->w);
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

    // the x-coordinates
    tb_fixed_t xb = edge_lsh->x;
    tb_fixed_t xe = edge_rsh->x;
    tb_assert_abort(xe >= xb);

    // the slopes
    tb_fixed_t sb = edge_lsh->slope;
    tb_fixed_t se = edge_rsh->slope;

    // compute the previous x-coordinates
    tb_fixed_t xb_prev = edge_lsh->is_first? xb : xb - sb;
    tb_fixed_t xe_prev = edge_rsh->is_first? xe : xe - se;

    // find the contour of this two edges
    gb_polygon_cutter_contour_ref_t contour = gb_polygon_cutter_contour_find(impl, yb - 1, xb_prev, xe_prev);
    if (contour)
    {
        // update the contour
        contour->y  = yb;
        contour->xb = xb;
        contour->xe = xe;
        contour->sb = sb;
        contour->se = se;

        // update the contour
        gb_polygon_cutter_contour_update(impl, contour);        
    }
    // not found? add a new contour
    else
    {
        // make a new contour
        contour = gb_polygon_cutter_contour_init(impl);
        tb_assert_abort(contour);

        // init the new contour
        contour->y  = yb;
        contour->xb = xb;
        contour->xe = xe;
        contour->sb = sb;
        contour->se = se;

        // append points to the contour
        gb_polygon_cutter_contour_append(contour, yb, xb, xe);

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
            tb_assert_abort(contour && contour->xe >= contour->xb);

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
