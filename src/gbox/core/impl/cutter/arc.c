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

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_void_t gb_cutter_arc_done(gb_arc_ref_t arc, gb_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_return(arc && func);

    // the sin and cos for the start angle
    gb_float_t  bs;
    gb_float_t  bc;
    gb_sos(gb_degree_to_radian(arc->ab), &bs, &bc);

    // init
    tb_size_t   rxi = (tb_size_t)gb_float_to_long(arc->rx);
    tb_size_t   ryi = (tb_size_t)gb_float_to_long(arc->ry);
    tb_int64_t  rxf = (tb_int64_t)gb_float_to_fixed(arc->rx);
    tb_int64_t  ryf = (tb_int64_t)gb_float_to_fixed(arc->ry);
    tb_int64_t  an = (tb_int64_t)gb_float_to_fixed(gb_degree_to_radian(arc->an)); if (an < 0) an = -an;
    tb_size_t   xn = rxi < 90? ((rxi >> 2) + 16): ((rxi >> 4) + 15);
    tb_size_t   yn = ryi < 90? ((ryi >> 2) + 16): ((ryi >> 4) + 15);
    tb_size_t   n = (xn + yn) >> 1;
    tb_int64_t  a = TB_FIXED_ONE - (((an * an) / (n * n)) >> 17);
    tb_int64_t  b = an / n - (((an * an * an) / (3 * n * n * n)) >> 33); if (arc->an < 0) b = -b;
    tb_int64_t  x0 = (tb_int64_t)gb_float_to_fixed(arc->c0.x);
    tb_int64_t  y0 = (tb_int64_t)gb_float_to_fixed(arc->c0.y);
    tb_int64_t  x1 = bc;
    tb_int64_t  y1 = bs;
    tb_int64_t  x2 = 0;
    tb_int64_t  y2 = 0;

    // head
    gb_point_t pb, pt;
    pb.x = arc->c0.x + gb_mul(arc->rx, bc);
    pb.y = arc->c0.y + gb_mul(arc->ry, bs);

    // done
    func(GB_PATH_CODE_MOVE, &pb, priv);

    // walk
    while (n--)
    {
        x2 = (a * x1 - b * y1) >> 16;
        y2 = (b * x1 + a * y1) >> 16;

        pt.x = gb_fixed_to_float((tb_fixed_t)(x0 + ((x2 * rxf) >> 16)));
        pt.y = gb_fixed_to_float((tb_fixed_t)(y0 + ((y2 * ryf) >> 16)));

        // done
        func(GB_PATH_CODE_LINE, &pt, priv);

        // next
        x1 = x2;
        y1 = y2;
    }
}

