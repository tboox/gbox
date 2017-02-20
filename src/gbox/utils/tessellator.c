/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
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
 * private implementation
 */
static tb_void_t gb_tessellator_done_output(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert(impl && impl->mesh && impl->func);

    // init outputs first
    if (!impl->outputs) impl->outputs = tb_vector_init(GB_TESSELLATOR_OUTPUTS_GROW, tb_element_mem(sizeof(gb_point_t), tb_null, tb_null));

    // check outputs
    tb_vector_ref_t outputs = impl->outputs;
    tb_assert(outputs);

    // done
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(impl->mesh), face)
    {
        // the face is inside?
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
                tb_assert(point);

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
                tb_assert(tb_vector_data(outputs));

                // append the first point for closing the contour
                tb_vector_insert_tail(outputs, point_first);

                // done it
                impl->func((gb_point_ref_t)tb_vector_data(outputs), (tb_uint16_t)tb_vector_size(outputs), impl->priv);
            }
        }
    }
}
static tb_void_t gb_tessellator_done_convex(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert(impl && impl->func && polygon && bounds);

    // only one convex contour
    tb_assert(polygon->convex && polygon->counts && !polygon->counts[1]);

    // make convex or monotone? done it directly
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_MONOTONE)
    {
        // done it
        impl->func(polygon->points, polygon->counts[0], impl->priv);

        // ok
        return ;
    }

    // must be triangulation mode now
    tb_assert(impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // only two faces
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert(mesh && tb_iterator_size(gb_mesh_face_itor(mesh)) == 2);

    // the arbitrary face is marked "inside" and the triangulation will be not effected
    gb_tessellator_face_inside_set(gb_mesh_face_head(mesh), 1);

    // make triangulation region
    gb_tessellator_triangulation_make(impl);

    // done output
    gb_tessellator_done_output(impl);
}
static tb_void_t gb_tessellator_done_concave(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{ 
    // check
    tb_assert(impl && polygon && !polygon->convex && bounds);

    // make mesh
    if (!gb_tessellator_mesh_make(impl, polygon)) return ;

    // make horizontal monotone region
    gb_tessellator_monotone_make(impl, bounds);

    // need make convex or triangulation polygon?
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION)
    {
        // make triangulation region for each horizontal monotone region
        gb_tessellator_triangulation_make(impl);

        // make convex? 
        if (impl->mode == GB_TESSELLATOR_MODE_CONVEX)
        {
            // merge triangles to the convex polygon
            gb_tessellator_convex_make(impl);
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

    // exit event queue
    if (impl->event_queue) tb_priority_queue_exit(impl->event_queue);
    impl->event_queue = tb_null;

    // exit active regions
    if (impl->active_regions) tb_list_exit(impl->active_regions);
    impl->active_regions = tb_null;

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
    tb_assert_and_check_return(impl && impl->func && polygon && polygon->points && polygon->counts && bounds);

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
