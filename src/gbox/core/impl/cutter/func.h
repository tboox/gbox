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
 * @file        func.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_CUTTER_FUNC_H
#define GB_CORE_IMPL_CUTTER_FUNC_H

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

/* add the point for path
 *
 * @param code          the path code
 * @param point         the point
 * @param priv          the user private data, @note muse be path
 *
 * <pre>
 *  ------
 * |      |  ----------
 * |      | |          |
 * |      |  ----------
 *  ------
 * </pre>
 */
tb_void_t               gb_cutter_func_add_for_path(tb_size_t code, gb_point_ref_t point, tb_cpointer_t priv);

/* add the point to last for path
 *
 * @param code          the path code
 * @param point         the point
 * @param priv          the user private data, @note muse be path
 *
 * <pre>
 *  ------.
 * |      |\ ----------
 * |      | |          |
 * |      |  ----------
 *  ------
 * </pre>
 */
tb_void_t               gb_cutter_func_add_to_for_path(tb_size_t code, gb_point_ref_t point, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


