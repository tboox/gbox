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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        active_region.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "active_region"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "active_region.h"
#include "geometry.h"
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// enable test?
#define GB_ACTIVE_REGION_TEST_ENABLE    (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_tessellator_active_region_leq(gb_mesh_vertex_ref_t event, gb_tessellator_active_region_ref_t lregion, gb_tessellator_active_region_ref_t rregion)
{
    // check
    tb_assert_abort(event && lregion && rregion);

    // the edges
    gb_mesh_edge_ref_t ledge = lregion->edge;
    gb_mesh_edge_ref_t redge = rregion->edge;
    tb_assert_abort(ledge && redge);

    /* two edges must go up
     *
     *  / \     / \
     *   |       |
     */
    tb_assert_abort(gb_tessellator_vertex_in_top(gb_mesh_edge_dst(ledge), gb_mesh_edge_org(ledge)));
    tb_assert_abort(gb_tessellator_vertex_in_top(gb_mesh_edge_dst(redge), gb_mesh_edge_org(redge)));

    /* 
     *        event      
     *         .      
     * ledge .           
     *     .                                       
     *
     */
    if (gb_mesh_edge_dst(ledge) == event)
    {
        /* 
         *           event
         *             .
         *     ledge .   .
         *         .       . redge
         *       .           .
         *
         */
        if (gb_mesh_edge_dst(redge) == event)
        {
            /* 
             *           event
             *             .
             *     ledge .   .
             *         .       . redge
             *       . --------  .
             *                     .
             *                       .
             *
             */
            if (gb_tessellator_vertex_in_top(gb_mesh_edge_org(ledge), gb_mesh_edge_org(redge))) 
                return gb_tessellator_vertex_on_edge_or_left(gb_mesh_edge_org(ledge), gb_mesh_edge_dst(redge), gb_mesh_edge_org(redge));

            /* 
             *         event        
             *           .                      
             *         .   . redge      
             * ledge .       .           
             *     . --------- .       
             *   .                                 
             * .                                         
             *
             */
            return gb_tessellator_vertex_on_edge_or_right(gb_mesh_edge_org(redge), gb_mesh_edge_dst(ledge), gb_mesh_edge_org(ledge));
        }

        /* 
         *         event    .     
         *           . ------ .              
         *         .            . redge      
         * ledge .                .           
         *     .                    .       
         *   .                                 
         * .                                         
         *
         */
        return gb_tessellator_vertex_on_edge_or_left(event, gb_mesh_edge_dst(redge), gb_mesh_edge_org(redge));
    }
    /*  event
     *     .
     *       . redge
     *         .
     */
    else if (gb_mesh_edge_dst(redge) == event)
    {
        /* 
         *             .      event        
         *           . -------- .                      
         *         .              . redge      
         * ledge .                  .           
         *     .                      .       
         *   .                                 
         * .                                         
         *
         */
        return gb_tessellator_vertex_on_edge_or_right(event, gb_mesh_edge_dst(ledge), gb_mesh_edge_org(ledge));
    }

    /* 
     *             .              
     *           .          .                      
     *         . ld       rd  . redge      
     * ledge . ------ . ------- .           
     *     .   > 0 event  < 0     .       
     *   .                                 
     * .       
     *
     *             .              
     *           .          .                      
     *         .              . redge      
     * ledge . ---------------- . ----------- .          
     *     .                      .  rd > 0  event
     *   .                                 
     * .       |------------------------------|
     *                      ld > 0
     *
     *                         .              
     *                       .          .                      
     *        ld < 0       .              . redge      
     *  . -------- ledge . ---------------- .           
     * event           .                      .       
     *               .                                 
     *             .      
     *  |-----------------------------------|
     *                 rd < 0
     */
    gb_float_t ld = gb_tessellator_vertex_to_edge_distance_h(event, gb_mesh_edge_dst(ledge), gb_mesh_edge_org(ledge));
    gb_float_t rd = gb_tessellator_vertex_to_edge_distance_h(event, gb_mesh_edge_dst(redge), gb_mesh_edge_org(redge));

    // ledge <= redge?
    return ld >= rd;
}
static tb_long_t gb_tessellator_active_region_comp(tb_element_ref_t element, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)element->priv;
    tb_assert_abort(impl);

    // lregion <= rregion ? -1 : 1
    return (!gb_tessellator_active_region_leq(impl->event, (gb_tessellator_active_region_ref_t)ldata, (gb_tessellator_active_region_ref_t)rdata) << 1) - 1;
}
#ifdef __gb_debug__
static tb_char_t const* gb_tessellator_active_region_cstr(tb_element_ref_t element, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_active_region_ref_t region = (gb_tessellator_active_region_ref_t)data;
    tb_assert_and_check_return_val(region, tb_null);

    // the edge
    gb_mesh_edge_ref_t edge = region->edge;

    // make info
    tb_long_t size = tb_snprintf(   cstr
                                ,   maxn
                                ,   "(%{point} => %{point}, winding: %ld, inside: %d)"
                                ,   gb_tessellator_vertex_point(gb_mesh_edge_org(edge))
                                ,   gb_tessellator_vertex_point(gb_mesh_edge_dst(edge))
                                ,   region->winding
                                ,   region->inside);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
#endif
/* insert region in ascending order and save the region position
 *
 * r0 <---- r1 <------ r2 <------- r3 <--- ... <---- 
 *                              region_tail
 *                 <----------------|
 *                      insert
 */
static gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert_done(gb_tessellator_impl_t* impl, tb_size_t tail, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region && region->edge);

    // trace
    tb_trace_d("insert: %{point} => %{point}", gb_tessellator_vertex_point(gb_mesh_edge_org(region->edge)), gb_tessellator_vertex_point(gb_mesh_edge_dst(region->edge)));

    // reverse to find the inserted position
    tb_size_t itor = tb_rfind_if(impl->active_regions, tb_iterator_head(impl->active_regions), tail, tb_predicate_le, region);

    // insert the region to the next position
    itor = tb_list_insert_next(impl->active_regions, itor, region);
    tb_assert_abort(itor != tb_iterator_tail(impl->active_regions));

    // get the real region reference 
    region = tb_iterator_item(impl->active_regions, itor);
    tb_assert_abort(region);

    // save the region position
    region->position = itor;

    // save the region reference to the edge
    gb_tessellator_edge_region_set(region->edge, region);

    // ok
    return region;
}
/* insert region for the bounds in ascending order
 *
 * dst(event)
 * / \
 *  |  region
 *  |
 * org
 */
static tb_void_t gb_tessellator_active_regions_insert_bounds(gb_tessellator_impl_t* impl, gb_float_t x, gb_float_t y_org, gb_float_t y_dst)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->active_regions);

    // init two points of the new edge
    gb_point_t org;
    gb_point_t dst;
    gb_point_make(&org, x, y_org);
    gb_point_make(&dst, x, y_dst);

    // make edge
    gb_mesh_edge_ref_t edge = gb_tessellator_mesh_make_edge(impl, &org, &dst);
    tb_assert_abort(edge);

    // update the current sweep event for inserting region in ascending order
    impl->event = gb_mesh_edge_dst(edge);

    // make region
    gb_tessellator_active_region_t region;
    region.edge     = edge;
    region.winding  = 0;
    region.inside   = 0;
    region.bounds   = 1;

    // insert region
    gb_tessellator_active_regions_insert(impl, &region);
}
#if GB_ACTIVE_REGION_TEST_ENABLE && defined(__gb_debug__)
static tb_void_t gb_tessellator_active_regions_test_insert(gb_tessellator_impl_t* impl, gb_float_t sweep_xb, gb_float_t sweep_xe, gb_float_t sweep_y)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->active_regions);

    // init coordinates
    tb_long_t xb = tb_random_range(tb_null, gb_float_to_long(sweep_xb), gb_float_to_long(sweep_xe));
    tb_long_t xe = tb_random_range(tb_null, gb_float_to_long(sweep_xb), gb_float_to_long(sweep_xe));
    tb_long_t yb = tb_random_range(tb_null, 1, 200);
    tb_long_t ye = tb_random_range(tb_null, 1, 200);

    // init two points of the new edge
    gb_point_t org;
    gb_point_t dst;
    gb_point_make(&org, gb_long_to_float(xb), sweep_y + gb_long_to_float(yb));
    gb_point_make(&dst, gb_long_to_float(xe), sweep_y - gb_long_to_float(ye));

    // make edge
    gb_mesh_edge_ref_t edge = gb_tessellator_mesh_make_edge(impl, &org, &dst);
    tb_assert_abort(edge);

    // make region
    gb_tessellator_active_region_t region;
    region.edge     = edge;
    region.winding  = 0;
    region.inside   = 0;
    region.bounds   = 1;

    // insert region
    gb_tessellator_active_regions_insert(impl, &region);
}
static tb_void_t gb_tessellator_active_regions_test(gb_tessellator_impl_t* impl, gb_float_t sweep_xb, gb_float_t sweep_xe, gb_float_t sweep_y)
{
    // check
    tb_assert_abort(impl && impl->active_regions);

    // make the current sweep event point
    gb_point_t point;
    gb_point_make(&point, gb_avg(sweep_xb, sweep_xe), sweep_y);

    // update the current sweep event for inserting region in ascending order
    tb_byte_t vertex[sizeof(gb_mesh_vertex_t) + sizeof(gb_tessellator_vertex_t)];
    impl->event = (gb_mesh_vertex_ref_t)vertex;
    gb_tessellator_vertex_point_set(impl->event, point);

    // insert some regions 
    __tb_volatile__ tb_size_t count = 20;
    while (count--) gb_tessellator_active_regions_test_insert(impl, sweep_xb, sweep_xe, sweep_y);

    // make the codes for drawing sweep line
    tb_printf(  "    gb_canvas_color_set(canvas, GB_COLOR_BLACK);\n");
    tb_printf(  "    gb_canvas_draw_line2i(canvas, %ld, %ld, %ld, %ld);\n"
            ,   gb_float_to_long(sweep_xb)
            ,   gb_float_to_long(sweep_y)
            ,   gb_float_to_long(sweep_xe)
            ,   gb_float_to_long(sweep_y));

    // dump the codes for drawing
    tb_size_t index = 1;
    tb_for_all_if (gb_tessellator_active_region_ref_t, region, impl->active_regions, region)
    {
        // the edge
        gb_mesh_edge_ref_t edge = region->edge;

        // the points
        gb_point_ref_t org = gb_tessellator_vertex_point(gb_mesh_edge_org(edge));
        gb_point_ref_t dst = gb_tessellator_vertex_point(gb_mesh_edge_dst(edge));

        // make the codes
        tb_printf(  "    gb_canvas_color_set(canvas, gb_color_from_index(%lu));\n", index++);
        tb_printf(  "    gb_canvas_draw_line2i(canvas, %ld, %ld, %ld, %ld);\n"
                ,   gb_float_to_long(org->x)
                ,   gb_float_to_long(org->y)
                ,   gb_float_to_long(dst->x)
                ,   gb_float_to_long(dst->y));
    }
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_active_regions_make(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && bounds && bounds->w > 0 && bounds->h > 0);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);

    // init active regions
    if (!impl->active_regions) 
    {
        // make active region element
        tb_element_t element = tb_element_mem(sizeof(gb_tessellator_active_region_t), tb_null, impl);

        // init the comparator 
        element.comp = gb_tessellator_active_region_comp;

#ifdef __gb_debug__
        // init the c-string function for tb_list_dump
        element.cstr = gb_tessellator_active_region_cstr;
#endif

        // make active regions
        impl->active_regions = tb_list_init(0, element);
    }
    tb_assert_abort_and_check_return_val(impl->active_regions, tb_false);

    // clear active regions first
    tb_list_clear(impl->active_regions);

    /* insert two regions for the bounds to avoid special cases
     *
     * their coordinates are big enough that they will never be merged with real input features.
     * 
     * (xb, yb)
     *   / \         .        .    .     / \
     *    |  region    .     .     .      |  region
     *    |              .  .    .        |
     *                                 (xe, ye)
     */
    gb_float_t xb = bounds->x;
    gb_float_t yb = bounds->y;
    gb_float_t xe = bounds->x + bounds->w;
    gb_float_t ye = bounds->y + bounds->h;
    gb_tessellator_active_regions_insert_bounds(impl, xb, ye, yb);
    gb_tessellator_active_regions_insert_bounds(impl, xe, ye, yb);

#if GB_ACTIVE_REGION_TEST_ENABLE && defined(__gb_debug__)
    // test the active regions
    gb_tessellator_active_regions_test(impl, xb, xe, gb_avg(yb, ye));
#endif

    // ok
    return tb_list_size(impl->active_regions);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_find(gb_tessellator_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_abort(impl && impl->active_regions && edge);

    // make a temporary region with the given edge for finding the real region containing it
    gb_tessellator_active_region_t region_temp;
    region_temp.edge = edge;

    /* reverse to find the region containing the given edge from the regions
     *
     * region.edge
     * |
     * .                .     .
     * .                .      .
     * .     region1    .       .
     * .                .        . region3
     * .         .      .         .
     * .       .        . region2  .
     * .     . edge     .
     *          |
     *        found                <= find direction
     *
     *
     */
    tb_size_t itor = tb_rfind_all_if(impl->active_regions, tb_predicate_le, &region_temp);

    // get the found item
    return (itor != tb_iterator_tail(impl->active_regions))? (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, itor) : tb_null;
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_right(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions);
    tb_assert_abort(region && region->position != tb_iterator_tail(impl->active_regions));
    
    // get the next position
    tb_size_t position = tb_iterator_next(impl->active_regions, region->position);

    // no right region?
    tb_check_return_val(position != tb_iterator_tail(impl->active_regions), tb_null);

    // ok
    return (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, position);
}
tb_void_t gb_tessellator_active_regions_remove(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region && region->edge);
    tb_assert_abort(region->position != tb_iterator_tail(impl->active_regions));

    // clear the region reference for the edge
    gb_tessellator_edge_region_set(region->edge, tb_null);

    // remove it
    tb_list_remove(impl->active_regions, region->position);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region);

    // insert it
    return gb_tessellator_active_regions_insert_done(impl, tb_iterator_tail(impl->active_regions), region);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert_before(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_tail, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region_tail && region);
    tb_assert_abort(region_tail->position != tb_iterator_tail(impl->active_regions));

    // insert it
    return gb_tessellator_active_regions_insert_done(impl, region_tail->position, region);
}

