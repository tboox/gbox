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
 * @file        demo.h
 *
 */
#ifndef GB_DEMO_H
#define GB_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "gbox/gbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the main decl
#define GB_DEMO_MAIN_DECL(name)         tb_int_t gb_demo_##name##_main(tb_int_t argc, tb_char_t** argv)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

// core
GB_DEMO_MAIN_DECL(core_path);
GB_DEMO_MAIN_DECL(core_bitmap);
GB_DEMO_MAIN_DECL(core_vector);

// utils
GB_DEMO_MAIN_DECL(utils_mesh);
GB_DEMO_MAIN_DECL(utils_geometry);

// other
GB_DEMO_MAIN_DECL(other_test);

#endif


