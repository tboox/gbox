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
 * trace
 */
#define TB_TRACE_MODULE_NAME            "tessellator"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "tessellator.h"
#include "mesh.h"
#include "impl/tessellator/tessellator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the output points grow
#ifdef __gb_small__
#   define GB_TESSELLATOR_OUTPUTS_GROW                          (32)
#else
#   define GB_TESSELLATOR_OUTPUTS_GROW                          (64)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

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

    // the output points
    tb_vector_ref_t         outputs;

}gb_tessellator_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_char_t const* gb_tessellator_vertex_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_vertex_ref_t vertex = (gb_tessellator_vertex_ref_t)data;
    tb_assert_and_check_return_val(vertex, tb_null);

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "%{point}", &vertex->point);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
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
        // init vertex func
        tb_item_func_t vertex_func = tb_item_func_mem(sizeof(gb_tessellator_vertex_t), tb_null, tb_null);
        vertex_func.cstr = gb_tessellator_vertex_cstr;

        // init mesh
        impl->mesh = gb_mesh_init(  tb_item_func_mem(sizeof(gb_tessellator_edge_t), tb_null, tb_null)
                                ,   tb_item_func_mem(sizeof(gb_tessellator_face_t), tb_null, tb_null)
                                ,   vertex_func);
    }

    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort_and_check_return_val(mesh, tb_false);

    // clear mesh first
    gb_mesh_clear(mesh);

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
            edge = gb_mesh_edge_make(mesh);

            // save the first point
            point_first = point;

            // save the first edge
            edge_first = edge;
        }
        // closed?
        else if (index + 1 == count && point_first->x == point->x && point_first->y == point->y)
        {
            // connect an edge to the first edge
            edge = gb_mesh_edge_connect(mesh, edge, edge_first);

            // init face.inside
            gb_tessellator_face_inside_set(gb_mesh_edge_lface(edge), 0);
            gb_tessellator_face_inside_set(gb_mesh_edge_rface(edge), 0);
        }
        else 
        {
            // append an edge
            edge = gb_mesh_edge_append(mesh, edge);
        }

        // check
        tb_assert_abort_and_check_break_state(edge, ok, tb_false);

        // init edge.winding
        gb_tessellator_edge_winding_set(edge, 1);
        gb_tessellator_edge_winding_set(gb_mesh_edge_sym(edge), -1);

        // init edge.org
        gb_tessellator_vertex_point_set(gb_mesh_edge_org(edge), *point);

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
    gb_mesh_check(mesh);
#endif

    // failed? clear mesh
    if (!ok) gb_mesh_clear(mesh);

    // ok?
    return ok && !gb_mesh_is_empty(mesh);
}
static tb_bool_t gb_tessellator_done_monotone(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && bounds);

    // TODO
    tb_trace_noimpl();

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(mesh);
#endif

    // ok
    return tb_true;
}
static tb_bool_t gb_tessellator_done_triangulation(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(impl && mesh && bounds);

    // TODO
    tb_trace_noimpl();

#ifdef __gb_debug__
    // check mesh
    gb_mesh_check(mesh);
#endif

    // ok
    return tb_true;
}
static tb_void_t gb_tessellator_done_output(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->func);

    // init outputs first
    if (!impl->outputs) impl->outputs = tb_vector_init(GB_TESSELLATOR_OUTPUTS_GROW, tb_item_func_mem(sizeof(gb_point_t), tb_null, tb_null));

    // check outputs
    tb_vector_ref_t outputs = impl->outputs;
    tb_assert_abort(outputs);

    // done
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(impl->mesh), face)
    {
        // is the inside face?
        if (gb_tessellator_face_inside(face)) 
        {
            // clear outputs
            tb_vector_clear(outputs);

            // make contour
            gb_mesh_edge_ref_t  head        = gb_mesh_face_edge(face);
            gb_mesh_edge_ref_t  edge        = head;
            gb_point_ref_t      point       = tb_null;
            gb_point_ref_t      point_first = tb_null;
            do
            {
                // the point
                point = gb_tessellator_vertex_point(gb_mesh_edge_org(edge));
                tb_assert_abort(point);

                // append point
                tb_vector_insert_tail(outputs, point);

                // save the first point
                if (!point_first) point_first = point;

                // the next edge
                edge = gb_mesh_edge_lnext(edge);

            } while (edge != head);

            // exists valid contour?
            if (tb_vector_size(outputs) > 2)
            {
                // check
                tb_assert_abort(tb_vector_data(outputs));

                // append the first point for closing the contour
                tb_vector_insert_tail(outputs, point_first);

                // done it
                impl->func((gb_point_ref_t)tb_vector_data(outputs), tb_vector_size(outputs), impl->priv);
            }
        }
    }
}
static tb_void_t gb_tessellator_done_convex(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && impl->func && polygon && bounds);

    // only one convex contour
    tb_assert_abort(polygon->convex && polygon->counts && !polygon->counts[1]);

    // make convex or monotone? done it directly
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_MONOTONE)
    {
        // done it
        impl->func(polygon->points, polygon->counts[0], impl->priv);

        // ok
        return ;
    }

    // must be triangulation mode now
    tb_assert_abort(impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // only two faces
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh && tb_iterator_size(gb_mesh_face_itor(mesh)) == 2);

    // get the two faces
    gb_mesh_face_ref_t face1 = gb_mesh_face_head(mesh);
    gb_mesh_face_ref_t face2 = gb_mesh_face_last(mesh);
    tb_assert_abort(face1 && face2 && face1 != face2);

#if 1
    // TODO
    gb_tessellator_face_inside_set(face2, 1);
#else
    // get the two edges of the face1
    gb_mesh_edge_ref_t edge1 = gb_mesh_face_edge(face1);
    gb_mesh_edge_ref_t edge2 = gb_mesh_edge_lnext(edge1);
    tb_assert_abort(edge1 != edge2);

    /* get points
     *
     *         face2
     *
     *        vector2
     *         edge2
     *  <------------------- point2
     * | point3             |
     * |                    |           |
     * |       face1        | edge1     | vector1
     * |                    |          \ /
     * |                    |
     *  -------------------> point1
     */
    gb_point_ref_t point1 = gb_tessellator_vertex_point(gb_mesh_edge_org(edge1));
    gb_point_ref_t point2 = gb_tessellator_vertex_point(gb_mesh_edge_org(edge2));
    gb_point_ref_t point3 = gb_tessellator_vertex_point(gb_mesh_edge_dst(edge2));
    tb_assert_abort(point1 && point2 && point3);

    // make vectors
    gb_vector_t vector1;
    gb_vector_t vector2;
    gb_vector_make_from_two_points(&vector1, point2, point1);
    gb_vector_make_from_two_points(&vector2, point2, point3);

//    gb_vector_cross(&vector1, &vector2)
#endif

    // done triangulation
    if (!gb_tessellator_done_triangulation(impl, bounds)) return ;

    // done output
    gb_tessellator_done_output(impl);
}
static tb_void_t gb_tessellator_done_concave(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{ 
    // check
    tb_assert_abort(impl && polygon && !polygon->convex && bounds);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // done monotone
    if (!gb_tessellator_done_monotone(impl, bounds)) return ;

    // make convex or triangulation?
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION)
    {
        // done triangulation
        if (!gb_tessellator_done_triangulation(impl, bounds)) return ;

        // make convex? 
        if (impl->mode == GB_TESSELLATOR_MODE_CONVEX)
        {
            // TODO merge triangles to the convex polygon
            tb_trace_noimpl();
        }
    }

    // done output
    gb_tessellator_done_output(impl);
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

    // exit outputs
    if (impl->outputs) tb_vector_exit(impl->outputs);
    impl->outputs = tb_null;

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
