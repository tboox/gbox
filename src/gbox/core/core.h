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
 * @file        core.h
 * @defgroup    core
 */
#ifndef GB_CORE_H
#define GB_CORE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "matrix.h"
#include "pixmap.h"
#include "bitmap.h"
#include "window.h"
#include "paint.h"
#include "canvas.h"
#include "device.h"
#include "svg/svg.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! init core
 *
 * @return  tb_true or tb_false
 */
tb_bool_t   gb_core_init(tb_noarg_t);

/*! exit core
 */
tb_void_t   gb_core_exit(tb_noarg_t);

__tb_extern_c_leave__
#endif


