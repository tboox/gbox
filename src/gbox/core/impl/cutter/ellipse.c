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

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_cutter_ellipse_done(gb_ellipse_ref_t ellipse, gb_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_return(func && ellipse);

    // init
    tb_size_t   rxi = (tb_size_t)gb_float_to_long(ellipse->rx);
    tb_size_t   ryi = (tb_size_t)gb_float_to_long(ellipse->ry);
    tb_int64_t  rxf = (tb_int64_t)gb_float_to_fixed(ellipse->rx);
    tb_int64_t  ryf = (tb_int64_t)gb_float_to_fixed(ellipse->ry);
    tb_int64_t  pi = (tb_int64_t)TB_FIXED_PI;
    tb_size_t   xn = rxi < 90? ((rxi << 2) / 3 + 16): ((rxi / 3) + 92);
    tb_size_t   yn = ryi < 90? ((ryi << 2) / 3 + 16): ((ryi / 3) + 92);
    tb_size_t   n = (xn + yn) >> 1;
    tb_int64_t  a = TB_FIXED_ONE - (((pi * pi) / (n * n)) >> 15);
    tb_int64_t  b = (pi << 1) / n - (((pi * pi * pi) / (3 * n * n * n)) >> 30);
    tb_int64_t  x0 = (tb_int64_t)gb_float_to_fixed(ellipse->c0.x);
    tb_int64_t  y0 = (tb_int64_t)gb_float_to_fixed(ellipse->c0.y);
    tb_int64_t  x1 = TB_FIXED_ONE;
    tb_int64_t  y1 = 0;
    tb_int64_t  x2 = 0;
    tb_int64_t  y2 = 0;

    // head
    gb_point_t pb, pt;
    pb.x = ellipse->c0.x + ellipse->rx;
    pb.y = ellipse->c0.y;

    // done
    func(GB_PATH_CODE_MOVE, &pb, priv);

    // walk
    while (n--)
    {
        x2 = (a * x1 - b * y1) >> 16;
        y2 = (b * x1 + a * y1) >> 16;

        pt.x = gb_fixed_to_float(x0 + ((x2 * rxf) >> 16));
        pt.y = gb_fixed_to_float(y0 + ((y2 * ryf) >> 16));

        // done
        func(GB_PATH_CODE_LINE, &pt, priv);

        // next
        x1 = x2;
        y1 = y2;
    }
    
    // close 
    func(GB_PATH_CODE_CLOS, tb_null, priv);
}

