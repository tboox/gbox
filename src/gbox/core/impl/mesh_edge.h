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
 * @file        mesh_edge.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_MESH_EDGE_H
#define GB_CORE_IMPL_MESH_EDGE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// get the edge sym
#define gb_mesh_edge_sym(edge)                  ((edge)->sym)

// set the edge sym
#define gb_mesh_edge_sym_set(edge, val)         ((edge)->sym = (val))

// get the edge org
#define gb_mesh_edge_org(edge)                  ((edge)->org)

// set the edge org
#define gb_mesh_edge_org_set(edge, val)         ((edge)->org = (val))

// get the edge dst
#define gb_mesh_edge_dst(edge)                  ((edge)->sym->org)

// set the edge dst
#define gb_mesh_edge_dst_set(edge, val)         ((edge)->sym->org = (val))

// get the edge lface
#define gb_mesh_edge_lface(edge)                ((edge)->lface)

// set the edge lface
#define gb_mesh_edge_lface_set(edge, val)       ((edge)->lface = (val))

// get the edge rface
#define gb_mesh_edge_rface(edge)                ((edge)->sym->lface)

// set the edge lface
#define gb_mesh_edge_rface_set(edge, val)       ((edge)->sym->lface = (val))

// get the edge onext
#define gb_mesh_edge_onext(edge)                ((edge)->onext)

// set the edge onext
#define gb_mesh_edge_onext_set(edge, val)       ((edge)->onext = (val))

// get the edge oprev
#define gb_mesh_edge_oprev(edge)                ((edge)->sym->lnext)

// set the edge oprev
#define gb_mesh_edge_oprev_set(edge, val)       ((edge)->sym->lnext = (val))

// get the edge lnext
#define gb_mesh_edge_lnext(edge)                ((edge)->lnext)

// set the edge lnext
#define gb_mesh_edge_lnext_set(edge, val)       ((edge)->lnext = (val))

// get the edge lprev
#define gb_mesh_edge_lprev(edge)                ((edge)->onext->sym)

// set the edge lprev
#define gb_mesh_edge_lprev_set(edge, val)       ((edge)->onext->sym = (val))

// get the edge rnext
#define gb_mesh_edge_rnext(edge)                (gb_mesh_edge_oprev(edge)->sym)

// set the edge rnext
#define gb_mesh_edge_rnext_set(edge, val)       (gb_mesh_edge_oprev(edge)->sym = (val))

// get the edge rprev
#define gb_mesh_edge_rprev(edge)                ((edge)->sym->onext)

// set the edge rprev
#define gb_mesh_edge_rprev_set(edge, val)       ((edge)->sym->onext = (val))

// get the edge dnext
#define gb_mesh_edge_dnext(edge)                (gb_mesh_edge_rprev(edge)->sym)

// set the edge dnext
#define gb_mesh_edge_dnext_set(edge, val)       (gb_mesh_edge_rprev(edge)->sym = (val))

// get the edge dprev
#define gb_mesh_edge_dprev(edge)                ((edge)->lnext->sym)

// set the edge rprev
#define gb_mesh_edge_dprev_set(edge, val)       ((edge)->lnext->sym = (val))

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the mesh edge list ref type
typedef struct{}*           gb_mesh_edge_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the mesh edge list 
 *
 * @param func              the item func
 *
 * @return                  the edge list
 */
gb_mesh_edge_list_ref_t     gb_mesh_edge_list_init(tb_item_func_t func);

/* exit the mesh edge list
 *
 * @param list              the edge list
 */
tb_void_t                   gb_mesh_edge_list_exit(gb_mesh_edge_list_ref_t list);

/* clear the mesh edge list
 *
 * @param list              the edge list
 */
tb_void_t                   gb_mesh_edge_list_clear(gb_mesh_edge_list_ref_t list);

/*! the mesh edge count
 *
 * @param list              the list
 *
 * @return                  the item count
 */
tb_size_t                   gb_mesh_edge_list_size(gb_mesh_edge_list_ref_t list);

/*! the mesh edge maximum count
 *
 * @param list              the list
 *
 * @return                  the item max count
 */
tb_size_t                   gb_mesh_edge_list_maxn(gb_mesh_edge_list_ref_t list);

/*! make a bare and unconnected edge
 *
 * @param list              the list
 *
 * @return                  the new edge
 */
gb_mesh_edge_ref_t          gb_mesh_edge_list_make(gb_mesh_edge_list_ref_t list);

/*! kill the edge, release the storage
 *
 * @param list              the list
 * @param edge              the edge
 */
tb_void_t                   gb_mesh_edge_list_kill(gb_mesh_edge_list_ref_t list, gb_mesh_edge_ref_t edge);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


