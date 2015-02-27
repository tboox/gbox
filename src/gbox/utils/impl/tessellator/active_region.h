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
 * @file        active_region.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_ACTIVE_REGION_H
#define GB_UTILS_IMPL_TESSELLATOR_ACTIVE_REGION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the active region type
 *
 *  /.\                                     .              /.\
 *   .                 .                   .   .            .
 *   .               .   .                .       .         .
 *   .  region1    .       .             .           .      .
 *   .           .           .  region3 .                   .
 *   . ------- . - region2 --- . ---- event --------------- . ----- sweep line
 *   .       .                   .     .                    .
 *   .    edge2                edge3   .    region4         . region5
 *   .                                 .                    .
 * edge1                             edge4                edge5
 *(bound)                                                (bound)
 */
typedef struct __gb_tessellator_active_region_t
{
    // the region position
    tb_size_t               position;

    // the left edge and it goes up
    gb_mesh_edge_ref_t      edge;

    // the winding
    tb_long_t               winding;

    // is inside?
    tb_uint8_t              inside : 1;

    // is bounds?
    tb_uint8_t              bounds : 1;

}gb_tessellator_active_region_t, *gb_tessellator_active_region_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make the active regions and all regions are sorted
 *
 * @param impl                      the tessellator impl
 * @param bounds                    the bounds of the polygon
 *
 * @return                          tb_true or tb_false
 */
tb_bool_t                           gb_tessellator_active_regions_make(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds);

/* insert a new active region in ascending order
 *
 * @code
    {
        // init region
        gb_tessellator_active_region_t region;
        region.edge         = ...; //< must be initialized
        region.inside       = 0;
        region.winding      = 0;
        ...

        // insert region
        gb_tessellator_active_regions_insert(active_regions, &region);
    }
 * @endcode
 *
 * @param impl                      the tessellator impl
 * @param region                    the new region
 */
tb_void_t                           gb_tessellator_active_regions_insert(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

/* insert a new active region before the tail region in ascending order
 *
 * r0 <---- r1 <------ r2 <------- r3 <--- ... <---- 
 *                              region_tail
 *                 <----------------|
 *                      insert
 *
 * @param impl                      the tessellator impl
 * @param region_tail               the tail region
 * @param region                    the new region
 */
tb_void_t                           gb_tessellator_active_regions_insert_before(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_tail, gb_tessellator_active_region_ref_t region_new);

/* remove region from the regions
 *
 * @param impl                      the tessellator impl
 * @param region                    the removed region
 */
tb_void_t                           gb_tessellator_active_regions_remove(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

/* find the region containing the given edge from the regions
 *
 *         =>
 * .                .
 * .                .
 * .     region     .
 * .                .
 * .         .      .
 * .       .        .
 * .     . edge     .
 *
 * @param impl                      the tessellator impl
 * @param edge                      the edge
 *
 * @return                          the region
 */
gb_tessellator_active_region_ref_t  gb_tessellator_active_regions_find(gb_tessellator_impl_t* impl, gb_mesh_edge_ref_t edge);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


