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
 * @file        arc.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "arc.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_arc_make(gb_arc_ref_t arc, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    // check
    tb_assert(arc);

    // make it
    arc->c.x    = x0;
    arc->c.y    = y0;
    arc->rx     = rx;
    arc->ry     = ry;
    arc->ab     = ab;
    arc->an     = an;
}
tb_void_t gb_arc_imake(gb_arc_ref_t arc, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an)
{
    gb_arc_make(arc, gb_long_to_float(x0), gb_long_to_float(y0), gb_long_to_float(rx), gb_long_to_float(ry), gb_long_to_float(ab), gb_long_to_float(an));
}

