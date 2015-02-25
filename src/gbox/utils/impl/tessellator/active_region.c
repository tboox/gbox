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

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_long_t gb_tessellator_active_region_comp(tb_element_ref_t element, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
    // the regions
    gb_tessellator_active_region_ref_t lregion = (gb_tessellator_active_region_ref_t)ldata;
    gb_tessellator_active_region_ref_t rregion = (gb_tessellator_active_region_ref_t)rdata;
    tb_assert_abort(lregion && rregion);

    // TODO
    return 0;
}
static tb_bool_t gb_tessellator_active_region_find(tb_iterator_ref_t iterator, tb_cpointer_t item, tb_cpointer_t value)
{
    return tb_iterator_comp(iterator, item, value) <= 0;
}
#ifdef __gb_debug__
static tb_char_t const* gb_tessellator_active_region_cstr(tb_element_ref_t element, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_active_region_ref_t region = (gb_tessellator_active_region_ref_t)data;
    tb_assert_and_check_return_val(region, tb_null);

    // the left edge
    gb_mesh_edge_ref_t edge_left = region->edge_left;

    // make info
    tb_long_t size = tb_snprintf(   cstr
                                ,   maxn
                                ,   "(%{point} => %{point}, winding: %ld, inside: %d)"
                                ,   gb_tessellator_vertex_point(gb_mesh_edge_org(edge_left))
                                ,   gb_tessellator_vertex_point(gb_mesh_edge_dst(edge_left))
                                ,   region->winding
                                ,   region->inside);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
#endif
static tb_void_t gb_tessellator_active_regions_insert_done(gb_tessellator_impl_t* impl, tb_size_t tail, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region && region->edge_left);

    // reverse to find the inserted position
    tb_size_t itor = tb_rfind_if(impl->active_regions, tb_iterator_head(impl->active_regions), tail, gb_tessellator_active_region_find, region);

    // insert the region to the next position
    itor = tb_list_insert_next(impl->active_regions, itor, region);
    tb_assert_abort(itor != tb_iterator_tail(impl->active_regions));

    // get the real region reference 
    region = tb_iterator_item(impl->active_regions, itor);
    tb_assert_abort(region);

    // save the region position
    region->position = itor;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_active_regions_make(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl);

    // the mesh
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh);

    // init active regions
    if (!impl->active_regions) 
    {
        // make active region element
        tb_element_t element = tb_element_mem(sizeof(gb_tessellator_active_region_t), tb_null, tb_null);

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

    // TODO
#ifdef __tb_debug__
    tb_list_dump(impl->active_regions);
#endif

    // ok
    return tb_list_size(impl->active_regions);
}
tb_void_t gb_tessellator_active_regions_insert(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region);

    // insert it
    gb_tessellator_active_regions_insert_done(impl, tb_iterator_tail(impl->active_regions), region);
}
tb_void_t gb_tessellator_active_regions_insert_before(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_tail, gb_tessellator_active_region_ref_t region_new)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region_tail && region_new);
    tb_assert_abort(region_tail->position != tb_iterator_tail(impl->active_regions));

    // insert it
    gb_tessellator_active_regions_insert_done(impl, region_tail->position, region_new);
}
tb_void_t gb_tessellator_active_regions_remove(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_abort(impl && impl->active_regions && region);
    tb_assert_abort(region->position != tb_iterator_tail(impl->active_regions));

    // remove it
    tb_list_remove(impl->active_regions, region->position);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_find(gb_tessellator_impl_t* impl, gb_mesh_edge_ref_t edge_left)
{
    // check
    tb_assert_abort(impl && impl->active_regions && edge_left);

    // make a temporary region with the given left edge for finding it
    gb_tessellator_active_region_t region_temp;
    region_temp.edge_left = edge_left;

    // find it
    tb_size_t itor = tb_find_all(impl->active_regions, &region_temp);

    // get the found item
    return (itor != tb_iterator_tail(impl->active_regions))? (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, itor) : tb_null;
}

