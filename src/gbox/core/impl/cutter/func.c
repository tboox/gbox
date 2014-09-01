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
 * @file        func.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "func.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_cutter_func_add_for_path(tb_size_t code, gb_point_ref_t point, tb_cpointer_t priv)
{
    // check
    gb_path_ref_t path = (gb_path_ref_t)priv;
    tb_assert_and_check_return(path);

    // done
    switch (code)
    {
    case GB_PATH_CODE_MOVE:
        if (point) gb_path_move_to(path, point);
        break;
    case GB_PATH_CODE_LINE:
        if (point) gb_path_line_to(path, point);
        break;
    case GB_PATH_CODE_CLOS:
        gb_path_clos(path);
        break;
    default:
        break;
    }
}
tb_void_t gb_cutter_func_add_to_for_path(tb_size_t code, gb_point_ref_t point, tb_cpointer_t priv)
{
    // check
    gb_path_ref_t path = (gb_path_ref_t)priv;
    tb_assert_and_check_return(path);

    // done
    switch (code)
    {
    case GB_PATH_CODE_MOVE:
    case GB_PATH_CODE_LINE:
        if (point) gb_path_line_to(path, point);
        break;
    case GB_PATH_CODE_CLOS:
        gb_path_clos(path);
        break;
    default:
        break;
    }
}
