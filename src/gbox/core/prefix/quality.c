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
 * @file        quality.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "quality.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the quality
static tb_atomic_t g_quality = GB_QUALITY_TOP;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_size_t gb_quality(tb_noarg_t)
{
    return (tb_size_t)tb_atomic_get(&g_quality);
}
tb_void_t gb_quality_set(tb_size_t quality)
{
    // check
    tb_assert_and_check_return(quality <= GB_QUALITY_TOP);

    // save quality
    tb_atomic_set(&g_quality, quality);
}

