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
#   define GB_POLYGON_CUTTER_POINTS_GROW        (32)
#else
#   define GB_POLYGON_CUTTER_POINTS_GROW        (64)
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
    // the current start x-coordinate
    tb_fixed_t                      xb;

    // the current end x-coordinate
    tb_fixed_t                      xe;

    // the current start slope: dx / dy
    tb_fixed_t                      sb;

    // the current end slope: dx / dy
    tb_fixed_t                      se;

    // the points
    tb_vector_ref_t                 points;

    // the single list entry
    tb_single_list_entry_t          entry;

}gb_polygon_cutter_contour_t, *gb_polygon_cutter_contour_ref_t;

// the polygon cutter impl type
typedef struct __gb_polygon_cutter_impl_t
{
    // the raster
    gb_polygon_raster_ref_t         raster;

    // the contours
    tb_fixed_pool_ref_t             contours;

    // the free contours
    tb_single_list_entry_head_t     contours_free;

    // the active contours
    tb_single_list_entry_head_t     contours_active;

    // the previous active contour index
    tb_size_t                       active_prev;

    // the current active contour index
    tb_size_t                       active_itor;

    // the active y-coordinate
    tb_long_t                       active_y;

    // is first scanning line?
    tb_bool_t                       is_first;

    // the user cutter func
    gb_polygon_cutter_func_t        func;

    // the user private data
    tb_cpointer_t                   priv;

}gb_polygon_cutter_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_polygon_cutter_contour_exit(tb_pointer_t data, tb_cpointer_t priv)
{
    // check
    gb_polygon_cutter_contour_ref_t contour = (gb_polygon_cutter_contour_ref_t)data;
    tb_assert_abort(contour);

    // exit points
    if (contour->points) tb_vector_exit(contour->points);
    contour->points = tb_null;
}
static tb_void_t gb_polygon_cutter_contour_active_exit(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_contour_ref_t contour)
{
    // check
    tb_assert_abort(impl && contour);

    // put the contour to the free contours
    tb_single_list_entry_insert_head(&impl->contours_free, &contour->entry);
}
static gb_polygon_cutter_contour_ref_t gb_polygon_cutter_contour_active_init(gb_polygon_cutter_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->contours);

    // done
    tb_bool_t                       ok  = tb_false;
    gb_polygon_cutter_contour_ref_t contour = tb_null;
    do
    {
        // attempt to get contour from the free contours
        if (tb_single_list_entry_size(&impl->contours_free))
        {
            tb_single_list_entry_ref_t entry = tb_single_list_entry_head(&impl->contours_free);
            if (entry)
            {
                contour = (gb_polygon_cutter_contour_ref_t)tb_single_list_entry(&impl->contours_free, entry);
                tb_single_list_entry_remove_head(&impl->contours_free);
            }
        }

        // get contour from the contour pool
        if (!contour) contour = tb_fixed_pool_malloc0(impl->contours);
        tb_assert_and_check_break(contour);

        // init contour
        contour->xb = 0;
        contour->xe = 0;
        contour->sb = 0;
        contour->se = 0;

        // init points
        if (!contour->points) contour->points = tb_vector_init(GB_POLYGON_CUTTER_POINTS_GROW, tb_item_func_mem(sizeof(gb_point_t), tb_null, tb_null));
        tb_assert_and_check_break(contour->points);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (contour) gb_polygon_cutter_contour_active_exit(impl, contour);
        contour = tb_null;
    }

    // ok?
    return contour;
}
static tb_bool_t gb_polygon_cutter_is_newline(gb_polygon_cutter_impl_t* impl, tb_long_t y)
{
    // check
    tb_assert_abort(impl);

    // done
    tb_bool_t is_newline = tb_false;
    if (impl->is_first) 
    {
        // init the active y-coordinate
        impl->active_y = y;
        impl->is_first = tb_false;

        // new line
        is_newline = tb_true;
    }
    else if (impl->active_y != y)
    {
        // update the active y-coordinate
        impl->active_y = y;

        // new line
        is_newline = tb_true;
    }

    // is new line?
    return is_newline;
}
static tb_void_t gb_polygon_cutter_builder_init(gb_polygon_cutter_impl_t* impl, gb_polygon_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(impl && func);
 
    // save the user func and private data
    impl->func = func;
    impl->priv = priv;

    // check the active contours
    tb_assert_abort(tb_single_list_entry_is_null(&impl->contours_active));

    // init the first scanning line
    impl->is_first = tb_true;
}
static tb_void_t gb_polygon_cutter_builder_done(tb_long_t yb, tb_long_t ye, gb_polygon_raster_edge_ref_t edge_lsh, gb_polygon_raster_edge_ref_t edge_rsh, tb_cpointer_t priv)
{
    // check
    gb_polygon_cutter_impl_t* impl = (gb_polygon_cutter_impl_t*)priv;
    tb_assert_abort(impl && edge_lsh && edge_rsh);

    // done 
    tb_bool_t                       ok = tb_false;
    gb_polygon_cutter_contour_ref_t contour = tb_null;
    tb_iterator_ref_t               actives = tb_single_list_entry_itor(&impl->contours_active);
    do
    {
        // only one line
        tb_assert_abort(yb + 1 == ye);

        // the x-coordinates
        tb_fixed_t xb = edge_lsh->x;
        tb_fixed_t xe = edge_rsh->x;
        tb_assert_abort(xe >= xb);

        // the slopes
        tb_fixed_t sb = edge_lsh->slope;
        tb_fixed_t se = edge_rsh->slope;

        // is new line?
        tb_bool_t is_newline = gb_polygon_cutter_is_newline(impl, yb);
        if (is_newline)
        {
            // init the active contour index
            impl->active_prev = tb_iterator_tail(actives);
            impl->active_itor = tb_iterator_head(actives);
        }

        // trace
        tb_trace_d("newline: %d, actives: %lu, y: %ld, xb: %{fixed}, xe: %{fixed}, sb: %{fixed}, se: %{fixed}", is_newline, tb_single_list_entry_size(&impl->contours_active), yb, xb, xe, sb, se);

        // no active contour now?
        if (impl->active_itor == tb_iterator_tail(actives))
        {
            // trace
            tb_trace_d("append a new contour");

            // make a new contour
            contour = gb_polygon_cutter_contour_active_init(impl);
            tb_assert_and_check_break(contour);

            // init the new contour 
            contour->xb = xb;
            contour->xe = xe;
            contour->sb = sb;
            contour->se = se;

            // append the new contour to the active contours
            tb_single_list_entry_insert_tail(&impl->contours_active, &contour->entry);
 
            // update the active contour index
            impl->active_itor = tb_iterator_head(actives);
        }
        else
        {
            // the contour 
            contour = (gb_polygon_cutter_contour_ref_t)tb_iterator_item(actives, impl->active_itor);
            tb_assert_and_check_break(contour);

            // the two edges are belong to this active contour?
            if ((xb == contour->xb + sb) && (xe == contour->xe + se))
            {
                // trace
                tb_trace_d("update contour");

                // update the contour
                contour->xb = xb;
                contour->xe = xe;
                contour->sb = sb;
                contour->se = se;
            }
            // insert a new contour before this active contour
            else
            {
                // trace
                tb_trace_d("insert a new contour");

                // make a new contour
                contour = gb_polygon_cutter_contour_active_init(impl);
                tb_assert_and_check_break(contour);

                // init the new contour 
                contour->xb = xb;
                contour->xe = xe;
                contour->sb = sb;
                contour->se = se;
 
                // insert the new contour before this active contour
                if (impl->active_prev != tb_iterator_tail(actives))
                {
                    // the prev contour 
                    gb_polygon_cutter_contour_ref_t contour_prev = (gb_polygon_cutter_contour_ref_t)tb_iterator_item(actives, impl->active_prev);
                    tb_assert_and_check_break(contour_prev);

                    // insert it
                    tb_single_list_entry_insert_next(&impl->contours_active, &contour_prev->entry, &contour->entry);

                    // update the active contour index
                    impl->active_itor = tb_iterator_next(actives, impl->active_prev);
                }
                else
                {
                    // insert it
                    tb_single_list_entry_insert_head(&impl->contours_active, &contour->entry);

                    // update the active contour index
                    impl->active_itor = tb_iterator_head(actives);
                }
            }
        }

        // update the active contour index
        impl->active_prev = impl->active_itor;
        impl->active_itor = tb_iterator_next(actives, impl->active_itor);
  
        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // trace
        tb_trace_e("make convex failed!");

        // exit contour
        if (contour) gb_polygon_cutter_contour_active_exit(impl, contour);
        contour = tb_null;
    }
}
static tb_void_t gb_polygon_cutter_builder_exit(gb_polygon_cutter_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // done the left active contours
    // TODO
    if (tb_single_list_entry_size(&impl->contours_active))
    {
        // trace
        tb_trace_d("make the left %lu active contours", tb_single_list_entry_size(&impl->contours_active));
        
        // done
        tb_iterator_ref_t   actives = tb_single_list_entry_itor(&impl->contours_active);
        tb_size_t           itor = tb_iterator_head(actives);
        while (itor != tb_iterator_tail(actives))
        {
            // the contour
            gb_polygon_cutter_contour_ref_t contour = (gb_polygon_cutter_contour_ref_t)tb_iterator_item(actives, itor);
            tb_assert_abort(contour);

            // save next
            tb_size_t next = tb_iterator_next(actives, itor);

            // done contour
            // TODO

            // exit contour
            gb_polygon_cutter_contour_active_exit(impl, contour);

            // next
            itor = next;
        }

        // clear the active contours
        tb_single_list_entry_clear(&impl->contours_active);
    }
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
        impl->contours = tb_fixed_pool_init(tb_null, GB_POLYGON_CUTTER_CONTOURS_GROW, sizeof(gb_polygon_cutter_contour_t), tb_null, gb_polygon_cutter_contour_exit, (tb_cpointer_t)impl);
        tb_assert_and_check_break(impl->contours);

        // init the free contours
        tb_single_list_entry_init(&impl->contours_free, gb_polygon_cutter_contour_t, entry, tb_null);

        // init the active contours
        tb_single_list_entry_init(&impl->contours_active, gb_polygon_cutter_contour_t, entry, tb_null);

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
    tb_single_list_entry_exit(&impl->contours_active);

    // exit free contours
    tb_single_list_entry_exit(&impl->contours_free);

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
        gb_polygon_cutter_builder_init(impl, func, priv);

        // done raster and reduce the complex polygon to the some convex polygons
        gb_polygon_raster_done(impl->raster, polygon, bounds, rule, gb_polygon_cutter_builder_done, (tb_cpointer_t)impl);

        // exit builder
        gb_polygon_cutter_builder_exit(impl);
    }
}
