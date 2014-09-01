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
 * @file        cube.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "cube.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_cutter_cube_done(gb_point_ref_t pb, gb_point_ref_t cpb, gb_point_ref_t cpe, gb_point_ref_t pe, gb_cutter_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_return(func && pb && cpb && cpe && pe && priv);

    // init
    gb_float_t mxb = gb_lsh(cpb->x - pb->x, 1) + cpb->x - pe->x;
    gb_float_t myb = gb_lsh(cpb->y - pb->y, 1) + cpb->y - pe->y;
    gb_float_t mxe = gb_lsh(cpe->x - pe->x, 1) + cpe->x - pb->x;
    gb_float_t mye = gb_lsh(cpe->y - pe->y, 1) + cpe->y - pb->y;

    mxb = gb_fabs(mxb);
    myb = gb_fabs(myb);
    mxe = gb_fabs(mxe);
    mye = gb_fabs(mye);

    if (mxe < mxb) mxb = mxe;
    if (mye < myb) myb = mye;

    // ok?
    if (mxb + myb <= GB_ONE) func(GB_PATH_CODE_LINE, pe, priv);
    else
    {
        gb_point_t cp0, cp1, cp2, pb0, pe0, p0;

        cp0.x = gb_rsh(cpb->x + cpe->x, 1);
        cp0.y = gb_rsh(cpb->y + cpe->y, 1);

        cp1.x = gb_rsh(pb->x + cpb->x, 1);
        cp1.y = gb_rsh(pb->y + cpb->y, 1);

        cp2.x = gb_rsh(cpe->x + pe->x, 1);
        cp2.y = gb_rsh(cpe->y + pe->y, 1);

        pb0.x = gb_rsh(cp0.x + cp1.x, 1);
        pb0.y = gb_rsh(cp0.y + cp1.y, 1);

        pe0.x = gb_rsh(cp0.x + cp2.x, 1);
        pe0.y = gb_rsh(cp0.y + cp2.y, 1);

        p0.x = gb_rsh(pb0.x + pe0.x, 1);
        p0.y = gb_rsh(pb0.y + pe0.y, 1);

        gb_cutter_cube_done(pb, &cp1, &pb0, &p0, func, priv);
        gb_cutter_cube_done(&p0, &pe0, &cp2, pe, func, priv);
    }
}

