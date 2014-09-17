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
 * @file        geometry.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "geometry.h"
#include "../matrix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the quad points for the clockwise unit circle
static gb_point_t g_quad_points_for_unit_circle[] = 
{
#if 1

    // move-to
    { GB_ONE,           0               }

    // part1
,   { GB_ONE,           GB_TAN_PIOVER8  }
,   { GB_SQRT2_OVER2,   GB_SQRT2_OVER2  }
,   { GB_TAN_PIOVER8,   GB_ONE          }
,   { 0,                GB_ONE          }

    // part2
,   { -GB_TAN_PIOVER8,  GB_ONE          }
,   { -GB_SQRT2_OVER2,  GB_SQRT2_OVER2  }
,   { -GB_ONE,          GB_TAN_PIOVER8  }
,   { -GB_ONE,          0               }

    // part3
,   { -GB_ONE,          -GB_TAN_PIOVER8 }
,   { -GB_SQRT2_OVER2,  -GB_SQRT2_OVER2 }
,   { -GB_TAN_PIOVER8,  -GB_ONE         }
,   { 0,                -GB_ONE         }

    // part4
,   { GB_TAN_PIOVER8,   -GB_ONE         }
,   { GB_SQRT2_OVER2,   -GB_SQRT2_OVER2 }
,   { GB_ONE,           -GB_TAN_PIOVER8 }
,   { GB_ONE,           0               }

#else // counter-clockwise

    // move-to
    { GB_ONE,            0}

    // part1
,   { GB_ONE,           -GB_TAN_PIOVER8 }    
,   { GB_SQRT2_OVER2,   -GB_SQRT2_OVER2 }
,   { GB_TAN_PIOVER8,   -GB_ONE         }
,   { 0,                -GB_ONE         }

    // part2
,   { -GB_TAN_PIOVER8,  -GB_ONE         }
,   { -GB_SQRT2_OVER2,  -GB_SQRT2_OVER2 }
,   { -GB_ONE,          -GB_TAN_PIOVER8 }
,   { -GB_ONE,          0               }

    // part3
,   { -GB_ONE,          GB_TAN_PIOVER8  }
,   { -GB_SQRT2_OVER2,  GB_SQRT2_OVER2  }
,   { -GB_TAN_PIOVER8,  GB_ONE          }
,   { 0,                GB_ONE          }

    // part4
,   { GB_TAN_PIOVER8,   GB_ONE          }
,   { GB_SQRT2_OVER2,   GB_SQRT2_OVER2  }
,   { GB_ONE,           GB_TAN_PIOVER8  }
,   { GB_ONE,           0               }
#endif
};

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_geometry_make_quad(gb_point_ref_t pb, gb_point_ref_t cp, gb_point_ref_t pe, gb_geometry_line_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_return(func && pb && cp && pe);

    // init
    gb_float_t mx = cp->x - gb_rsh(pb->x + pe->x, 1);
    gb_float_t my = cp->y - gb_rsh(pb->y + pe->y, 1);

    // ok?
    if (gb_fabs(mx) + gb_fabs(my) <= GB_ONE) func(pe, priv);
    else
    {
        gb_point_t p0, cpb, cpe;
        cpb.x = gb_rsh(pb->x + cp->x, 1);
        cpb.y = gb_rsh(pb->y + cp->y, 1);
        cpe.x = gb_rsh(cp->x + pe->x, 1);
        cpe.y = gb_rsh(cp->y + pe->y, 1);
        p0.x = gb_rsh(cpb.x + cpe.x, 1);
        p0.y = gb_rsh(cpb.y + cpe.y, 1);

        gb_geometry_make_quad(pb, &cpb, &p0, func, priv);
        gb_geometry_make_quad(&p0, &cpe, pe, func, priv);
    }
}
tb_void_t gb_geometry_make_cube(gb_point_ref_t pb, gb_point_ref_t cpb, gb_point_ref_t cpe, gb_point_ref_t pe, gb_geometry_line_func_t func, tb_cpointer_t priv)
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
    if (mxb + myb <= GB_ONE) func(pe, priv);
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

        gb_geometry_make_cube(pb, &cp1, &pb0, &p0, func, priv);
        gb_geometry_make_cube(&p0, &pe0, &cp2, pe, func, priv);
    }
}
tb_void_t gb_geometry_make_arc(gb_arc_ref_t arc, gb_geometry_quad_func_t func, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(arc && func);

    // the direction and sweep angle
    tb_bool_t  clockwise = tb_true;
    gb_float_t sweep_angle = arc->an;
    if (sweep_angle < 0)
    {
        // counter-clockwise
        clockwise = tb_false;

        // |sweep_angle|
        sweep_angle = -sweep_angle;
    }
    if (sweep_angle > GB_DEGREE_360) sweep_angle = GB_DEGREE_360;

    // make quad points count
    tb_size_t count = 1 + ((gb_float_to_long(sweep_angle) / 45) << 1);
    tb_assert_abort(count & 0x1);

    // make quad points
    gb_point_t points[tb_arrayn(g_quad_points_for_unit_circle)];
    tb_memcpy(points, g_quad_points_for_unit_circle, count * sizeof(gb_point_t));

    // patch the last quad pair
    gb_float_t patched_angle = sweep_angle - gb_long_to_float(((count - 1) * 45) >> 1);
    if (gb_nz(patched_angle))
    {
        // the patched angle must be larger than zero
        tb_assert_abort(gb_bz(patched_angle));

        // the stop point for the unit circle
        gb_point_t stop_point;
        gb_sincos(gb_degree_to_radian(sweep_angle), &stop_point.y, &stop_point.x);

        // the last point
        gb_point_t last_point = points[count - 1];

        // init matrix
        gb_matrix_t matrix;
        gb_matrix_init_sincos(&matrix, last_point.y, last_point.x);

        /* compute tan(patched_angle/2)
         *
         * tan(x) = x + x^3/3 while x < pi/2
         */
        gb_float_t angle = gb_degree_to_radian(gb_rsh(patched_angle, 1));
        gb_float_t tan_angle = angle + gb_idiv(gb_mul(gb_mul(angle, angle), angle), 3);

        /* compute the ctrl point, last => ctrl => stop
         *
         * |     last
         * |      /       ctrl
         * |     /       /
         * |    /      /
         * |   /     /
         * |  /    /         stop
         * | /   /     
         * |/ /        
         * ------------------- last^
         *                    | 
         *                    | tan(patched_angle/2)
         *                    |
         *                   ctrl^
         *
         * ctrl^:   (1, tan(patched_angle/2))
         * matrix:  rotate_sincos(last_point.y, last_point.x)
         *
         * ctrl = ctrl^ * matrix
         */
        gb_point_t ctrl_point = gb_point_make(GB_ONE, tan_angle);
        gb_matrix_apply_point(&matrix, &ctrl_point);

        // patch the last quad pair
        points[count++] = ctrl_point;
        points[count++] = stop_point;
    }

    // the start point for arc
    gb_point_t start_point;
    gb_sincos(gb_degree_to_radian(arc->ab), &start_point.y, &start_point.x);

    // init matrix
    gb_matrix_t matrix;
    gb_matrix_init_scale(&matrix, arc->rx, arc->ry);
    gb_matrix_sincos(&matrix, start_point.y, start_point.x);
    if (!clockwise) gb_matrix_scale(&matrix, GB_ONE, -GB_ONE);
    gb_matrix_translate(&matrix, arc->c0.x, arc->c0.y);

    // apply matrix for the first point
    gb_matrix_apply_point(&matrix, points);

    // done func
    func(tb_null, points, priv);

    // walk points
    gb_point_t* pb = points + 1;
    gb_point_t* pe = points + count;
    for (; pb < pe; pb += 2) 
    {
        // apply matrix for the quad points
        gb_matrix_apply_point(&matrix, pb);
        gb_matrix_apply_point(&matrix, pb + 1);

        // done
        func(pb, pb + 1, priv);
    }
}
