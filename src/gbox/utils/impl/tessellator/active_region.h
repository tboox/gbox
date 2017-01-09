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

/* get the left region of the given region
 *
 * @param impl                      the tessellator impl
 * @param region                    the region
 *
 * @return                          the left region
 */
gb_tessellator_active_region_ref_t  gb_tessellator_active_regions_left(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

/* get the right region of the given region
 *
 * @param impl                      the tessellator impl
 * @param region                    the region
 *
 * @return                          the right region
 */
gb_tessellator_active_region_ref_t  gb_tessellator_active_regions_right(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

/* the region1 is in the left of the region2
 *
 * @param impl                      the tessellator impl
 * @param region1                   the region1
 * @param region2                   the region2
 *
 * @return                          tb_true or tb_false
 */
tb_bool_t                           gb_tessellator_active_regions_in_left(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region1, gb_tessellator_active_region_ref_t region2);

/* remove region from the regions
 *
 * @param impl                      the tessellator impl
 * @param region                    the removed region
 */
tb_void_t                           gb_tessellator_active_regions_remove(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

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
 * @param region                    the initialized region, but be not real region
 *
 * @return                          the real new region
 */
gb_tessellator_active_region_ref_t  gb_tessellator_active_regions_insert(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region);

/* insert a new active region after the previous region in ascending order
 *
 * r0 ----> r1 ------> r2 -------> r3 ---> ... ---->
 *                 region_prev
 *                     |----------------->
 *                           insert
 *
 * @param impl                      the tessellator impl
 * @param region_prev               the previous region
 * @param region                    the initialized region, but be not real region
 *
 * @return                          the real new region
 */
gb_tessellator_active_region_ref_t  gb_tessellator_active_regions_insert_after(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_prev, gb_tessellator_active_region_ref_t region);

#ifdef __gb_debug__
/* check the active regions
 *
 * @param impl                      the tessellator impl
 */
tb_void_t                           gb_tessellator_active_regions_check(gb_tessellator_impl_t* impl);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


