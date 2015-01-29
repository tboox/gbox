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
 * @file        tessellator.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "tessellator.h"
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef struct __gb_tessellator_impl_t
{
    // the mode
    tb_size_t               mode;

    // the winding rule
    tb_size_t               rule;

    // the func
    gb_tessellator_func_t   func;

    // the user private data
    tb_cpointer_t           priv;

    // the mesh
    gb_mesh_ref_t           mesh;

}gb_tessellator_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_tessellator_mesh_make(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_abort(impl && impl->mesh && polygon);

    // clear mesh first
    gb_mesh_clear(impl->mesh);

    // TODO
    tb_trace_noimpl();
    return tb_false;
}
static tb_void_t gb_tessellator_done_convex(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && polygon && polygon->convex && bounds);

    // make convex or monotone? done it directly
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_MONOTONE)
    {
        // check
        tb_assert_abort(impl->func);

        // done it
        impl->func(polygon, impl->priv);

        // ok
        return ;
    }

    // check
    tb_assert_abort(impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // TODO: make triangulation
    tb_trace_noimpl();
}
static tb_void_t gb_tessellator_done_concave(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{ 
    // check
    tb_assert_abort(impl && polygon && !polygon->convex && bounds);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // TODO
    tb_trace_noimpl();
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_tessellator_ref_t gb_tessellator_init()
{
    // init it
    return (gb_tessellator_ref_t)tb_malloc0_type(gb_tessellator_impl_t);
}
tb_void_t gb_tessellator_exit(gb_tessellator_ref_t tessellator)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // exit mesh
    if (impl->mesh) gb_mesh_exit(impl->mesh);
    impl->mesh = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_tessellator_mode_set(gb_tessellator_ref_t tessellator, tb_size_t mode)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set mode
    impl->mode = mode;
}
tb_void_t gb_tessellator_rule_set(gb_tessellator_ref_t tessellator, tb_size_t rule)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set rule
    impl->rule = rule;
}
tb_void_t gb_tessellator_func_set(gb_tessellator_ref_t tessellator, gb_tessellator_func_t func, tb_cpointer_t priv)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set func
    impl->func = func;
    impl->priv = priv;
}
tb_void_t gb_tessellator_done(gb_tessellator_ref_t tessellator, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_abort_and_check_return(impl && impl->func && polygon && polygon->points && polygon->counts && bounds);

    // is convex polygon for each contour?
    if (polygon->convex)
    {
        // done
        tb_size_t       index               = 0;
        gb_point_ref_t  points              = polygon->points;
        tb_uint16_t*    counts              = polygon->counts;
        tb_uint16_t     contour_counts[2]   = {0, 0};
        gb_polygon_t    contour             = {tb_null, contour_counts, tb_true};
        while ((contour_counts[0] = *counts++))
        {
            // init the polygon for this contour
            contour.points = points + index;

            // done tessellator for the convex contour, will be faster
            gb_tessellator_done_convex(impl, &contour, bounds);

            // update the contour index
            index += contour_counts[0];
        }
    }
    else
    {
        // done tessellator for the concave polygon
        gb_tessellator_done_concave(impl, polygon, bounds);
    }
}
