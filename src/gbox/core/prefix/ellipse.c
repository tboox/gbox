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
 * @file        ellipse.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ellipse.h"
#include "point.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_ellipse_make(gb_ellipse_ref_t ellipse, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    // check
    tb_assert_abort(ellipse);

    // make it
    ellipse->c.x    = x0;
    ellipse->c.y    = y0;
    ellipse->rx     = rx;
    ellipse->ry     = ry;
}
tb_void_t gb_ellipse_imake(gb_ellipse_ref_t ellipse, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    gb_ellipse_make(ellipse, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(rx), gb_long_to_float(ry));
}
tb_void_t gb_ellipse_make_from_rect(gb_ellipse_ref_t ellipse, gb_rect_ref_t rect)
{
    // check
    tb_assert_abort(ellipse && rect);

    // the radius
    gb_float_t rx = gb_half(rect->w);
    gb_float_t ry = gb_half(rect->h);

    // make it
    gb_ellipse_make(ellipse, rect->x + rx, rect->y + ry, rx, ry);
}

