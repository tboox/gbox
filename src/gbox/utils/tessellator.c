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
 * macros
 */

// the tessellator edge
#define gb_tessellator_edge(mesh, edge)                         ((gb_tessellator_edge_ref_t)gb_mesh_edge_data(mesh, edge))

// the tessellator edge winding
#define gb_tessellator_edge_winding(mesh, edge)                 (gb_tessellator_edge(mesh, edge)->winding)

// set the tessellator edge winding
#define gb_tessellator_edge_winding_set(mesh, edge, val)        do { gb_tessellator_edge(mesh, edge)->winding = (val); } while (0)

// the tessellator face
#define gb_tessellator_face(mesh, face)                         ((gb_tessellator_face_ref_t)gb_mesh_face_data(mesh, face))

// the tessellator vertex
#define gb_tessellator_vertex(mesh, vertex)                     ((gb_tessellator_vertex_ref_t)gb_mesh_vertex_data(mesh, vertex))

// the tessellator vertex point
#define gb_tessellator_vertex_point(mesh, vertex)               (&(gb_tessellator_vertex(mesh, vertex)->point))

// set the tessellator vertex point
#define gb_tessellator_vertex_point_set(mesh, vertex, val)      do { gb_tessellator_vertex(mesh, vertex)->point = (val); } while (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the tessellator edge type
typedef struct __gb_tessellator_edge_t
{
    // the winding
    tb_int32_t              winding;

}gb_tessellator_edge_t, *gb_tessellator_edge_ref_t;

// the tessellator face type
typedef struct __gb_tessellator_face_t
{
    tb_size_t               dumy;

}gb_tessellator_face_t, *gb_tessellator_face_ref_t;

// the tessellator vertex type
typedef struct __gb_tessellator_vertex_t
{
    // the point
    gb_point_t              point;

}gb_tessellator_vertex_t, *gb_tessellator_vertex_ref_t;

// the tessellator impl type
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
    tb_assert_abort(impl && polygon);

    // the points
    gb_point_ref_t      points = polygon->points;
    tb_uint16_t const*  counts = polygon->counts;
    tb_assert_abort_and_check_return_val(points && counts, tb_false);

    // not exists mesh?
    if (!impl->mesh) 
    {
        // init mesh
        impl->mesh = gb_mesh_init(  tb_item_func_mem(sizeof(gb_tessellator_edge_t),     tb_null, tb_null)
                                ,   tb_item_func_mem(sizeof(gb_tessellator_face_t),     tb_null, tb_null)
                                ,   tb_item_func_mem(sizeof(gb_tessellator_vertex_t),   tb_null, tb_null));
    }
    tb_assert_abort_and_check_return_val(impl->mesh, tb_false);

    // clear mesh first
    gb_mesh_clear(impl->mesh);

    // done
    tb_bool_t           ok          = tb_true;
    gb_point_ref_t      point       = tb_null;
    gb_point_ref_t      point_first = tb_null;
    tb_uint16_t         count       = *counts++;
    tb_size_t           index       = 0;
    gb_mesh_edge_ref_t  edge        = tb_null;
    gb_mesh_edge_ref_t  edge_first  = tb_null;
    while (index < count)
    {
        // the point
        point = points++;

        // first point?
        if (!index) 
        {
            // make a non-loop edge
            edge = gb_mesh_edge_make(impl->mesh);

            // save the first point
            point_first = point;

            // save the first edge
            edge_first = edge;
        }
        // closed?
        else if (index + 1 == count && point_first->x == point->x && point_first->y == point->y)
        {
            // connect an edge to the first edge
            edge = gb_mesh_edge_connect(impl->mesh, edge, edge_first);
        }
        else 
        {
            // append an edge
            edge = gb_mesh_edge_append(impl->mesh, edge);
        }

        // check
        tb_assert_abort_and_check_break_state(edge, ok, tb_false);

        // init edge
        gb_tessellator_edge_winding_set(impl->mesh, edge, 1);
        gb_tessellator_edge_winding_set(impl->mesh, gb_mesh_edge_sym(edge), -1);
        gb_tessellator_vertex_point_set(impl->mesh, gb_mesh_edge_org(edge), *point);

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

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(impl->mesh);
#endif

    // failed? clear mesh
    if (!ok) gb_mesh_clear(impl->mesh);

    // ok?
    return ok;
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

    // check mesh
    tb_check_return(impl->mesh && !gb_mesh_is_empty(impl->mesh));

    // TODO: make triangulation
    tb_trace_noimpl();
}
static tb_void_t gb_tessellator_done_concave(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{ 
    // check
    tb_assert_abort(impl && polygon && !polygon->convex && bounds);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // check mesh
    tb_check_return(impl->mesh && !gb_mesh_is_empty(impl->mesh));

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
