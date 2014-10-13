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
 * @file        lines.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_stroke_lines"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "lines.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

/* using half-bresenham algorithm
 *
 *                    draw 1/2 line
 *                 /      
 *               /
 *             /
 *           /
 *         *
 *       /
 *     /
 *   /
 * /  
 *  draw 1/2 line
 */
#if 0
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_biltter_ref_t biltter, tb_fixed6_t xb, tb_fixed6_t yb, tb_fixed6_t xe, tb_fixed6_t ye)
{
    // the dx and the x-increased bytes
    tb_long_t dx    = xe - xb;
    tb_long_t xinc  = 1;
    if (dx < 0) 
    {
        dx = -dx;
        xinc = -1;
    }

    // the dy and the y-increased bytes
    tb_long_t dy    = ye - yb;
    tb_long_t yinc  = 1;
    if (dy < 0) 
    {
        dy = -dy;
        yinc = -1;
    }

    // dx * 2, dy * 2 for float => fixed
    tb_long_t dx2 = dx << 1;
    tb_long_t dy2 = dy << 1;

    // draw 1 / 2 line
    tb_long_t dx12 = (dx + 1) >> 1;
    tb_long_t dy12 = (dy + 1) >> 1;
   
    // |slope| < 1?
    if (dx > dy)
    {
        // done 
        tb_long_t i = 0;
        tb_long_t eb = dy2 - dx;
        tb_long_t ee = eb;
        for (i = 0; i < dx12; ++i)
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, xb, yb);
            gb_bitmap_biltter_done_p(biltter, xe, ye);

            /* (x, y)
             * e / 1 = dy / dx
             * => y++ if e = dy / dx > 0.5 
             * => y++ if e = dy * 2 - dx > 0
             *
             * (x + 1, y)
             * e / 2 = dy / dx
             * => y++ if e = dy * 2 / dx > 0.5 
             * => y++ if e = dy * 4 - dx > 0
             * => e += dy * 2
             *
             * (x, y + 1)
             * (e + 1) / 1 = dy / dx
             * => y++ if e = dy / dx - 1 > 0.5 
             * => y++ if e = dy * 2 - dx - dx * 2 > 0
             * => e -= dx * 2
             *
             *  -------------------------------------------
             * |          |          |          |          |
             * |          |          |          |          |
             * |          |          |          |        * |
             * |          |          |          |  *      e|
             * |----------------------------*--------------|
             * |          |          |*         |          | dy
             * |          |    *     |          |          |
             * |        * |          |e         |         1|
             * | *        |e         |          |          |
             * O------------------------------------------> 
             *      1               dx
             */
            if (eb > 0)
            {
                eb -= dx2;
                yb += yinc;
            }
            
            if (ee >= 0)
            {
                ee -= dx2;
                ye -= yinc;
            }

            eb += dy2;
            ee += dy2;
            xb += xinc;
            xe -= xinc;
        }


        /* draw pm if line is (qb, ..., pm, ..., qe)
         * e.g. (0, 1, 2)
         */
        if (!(dx & 0x1)) gb_bitmap_biltter_done_p(biltter, xb, yb);
    }
    else
    {
        // done
        tb_long_t i = 0;
        tb_long_t eb = dx2 - dy;
        tb_long_t ee = eb;
        for (i = 0; i < dy12; ++i)
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, xb, yb);
            gb_bitmap_biltter_done_p(biltter, xe, ye);

            if (eb > 0)
            {
                eb -= dy2;
                xb += xinc;
            }

            if (ee >= 0)
            {
                ee -= dy2;
                xe -= xinc;
            }

            eb += dx2;
            ee += dx2;
            yb += yinc;
            ye -= yinc;
        }

        if (!(dy & 0x1)) gb_bitmap_biltter_done_p(biltter, xb, yb);
    }
}
#elif 1
static tb_size_t gb_bitmap_render_stroke_line_generic(gb_bitmap_biltter_ref_t biltter, tb_fixed6_t xb, tb_fixed6_t yb, tb_fixed6_t xe, tb_fixed6_t ye)
{
    // round coordinates
    tb_long_t ixb = tb_fixed6_round(xb);
    tb_long_t iyb = tb_fixed6_round(yb);
    tb_long_t ixe = tb_fixed6_round(xe);
    tb_long_t iye = tb_fixed6_round(ye);

    // horizontal?
    if (iyb == iye) return 'h';

    // vertical?
    if (ixb == ixe) return 'v';

    // the dx and dy
    tb_fixed6_t dx = xe - xb;
    tb_fixed6_t dy = ye - yb;

    /* more horizontal?
     *                       .
     *                  .
     *            .
     *      .
     * .
     */
    if (tb_fixed6_abs(dx) > tb_fixed6_abs(dy))
    {
        // reverse it for xb => xe
        if (xb > xe)
        {
            tb_swap(tb_fixed6_t, xb, xe);
            tb_swap(tb_fixed6_t, yb, ye);
            tb_swap(tb_fixed6_t, ixb, ixe);
        }

        // too short? ignore it
        tb_check_return_val(ixb != ixe, 0);

        // compute the slope
        tb_fixed_t slope = tb_fixed6_div(dy, dx);

        /* compute the more accurate start y-coordinate
         *
         * yb + (ixb - xb + 0.5) * dy / dx
         * => yb + ((0.5 - xb) % 1) * dy / dx
         */
        tb_fixed_t start_y = tb_fixed6_to_fixed(yb) + ((slope * ((TB_FIXED6_HALF - xb) & 63)) >> 6);

        // check
        tb_assert_abort(ixb < ixe);

        // done
        do
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, ixb, tb_fixed_round(start_y));

            // update the y-coordinate
            start_y += slope;
           
        } while (ixb++ < ixe);
    }
    /* more vertical?
     *
     *       .
     *      .
     *     .
     *    .
     *   .
     *  .
     * .
     */
    else 
    {
        // reverse it for yb => ye
        if (yb > ye)
        {
            tb_swap(tb_fixed6_t, xb, xe);
            tb_swap(tb_fixed6_t, yb, ye);
            tb_swap(tb_fixed6_t, iyb, iye);
        }

        // too short? ignore it
        tb_check_return_val(iyb != iye, 0);

        // compute the slope
        tb_fixed_t slope = tb_fixed6_div(dx, dy);

        /* compute the more accurate start x-coordinate
         *
         * xb + (iyb - yb + 0.5) * dx / dy
         * => xb + ((0.5 - yb) % 1) * dx / dy
         */
        tb_fixed_t start_x = tb_fixed6_to_fixed(xb) + ((slope * ((TB_FIXED6_HALF - yb) & 63)) >> 6);

        // check
        tb_assert_abort(iyb < iye);

        // done
        do
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, tb_fixed_round(start_x), iyb);

            // update the x-coordinate
            start_x += slope;
           
        } while (iyb++ < iye);
    }

    // ok
    return 0;
}
#else
/* using bresenham algorithm
 *
 *                 /      
 *               /
 *             /
 *           /
 *         /
 *       /
 *     /
 *   /
 * /  
 *  
 */
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_biltter_ref_t biltter, tb_fixed6_t xb, tb_fixed6_t yb, tb_fixed6_t xe, tb_fixed6_t ye)
{
    // the dx and the x-increased bytes
    tb_long_t dx    = xe - xb;
    tb_long_t xinc  = 1;
    if (dx < 0) 
    {
        dx = -dx;
        xinc = -xinc;
    }

    // the dy and the y-increased bytes
    tb_long_t dy    = ye - yb;
    tb_long_t yinc  = 1;
    if (dy < 0) 
    {
        dy = -dy;
        yinc = -yinc;
    }

    // dx * 2, dy * 2 for float => fixed
    tb_long_t dx2 = dx << 1;
    tb_long_t dy2 = dy << 1;

    // |slope| < 1?
    if (dx > dy)
    {
        // done 
        tb_long_t i = 0;
        tb_long_t e = dy2 - dx;
        for (i = 0; i < dx; ++i)
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, xb, yb);

            /* (x, y)
             * e / 1 = dy / dx
             * => y++ if e = dy / dx > 0.5 
             * => y++ if e = dy * 2 - dx > 0
             *
             * (x + 1, y)
             * e / 2 = dy / dx
             * => y++ if e = dy * 2 / dx > 0.5 
             * => y++ if e = dy * 4 - dx > 0
             * => e += dy * 2
             *
             * (x, y + 1)
             * (e + 1) / 1 = dy / dx
             * => y++ if e = dy / dx - 1 > 0.5 
             * => y++ if e = dy * 2 - dx - dx * 2 > 0
             * => e -= dx * 2
             *
             *  -------------------------------------------
             * |          |          |          |          |
             * |          |          |          |          |
             * |          |          |          |        * |
             * |          |          |          |  *      e|
             * |----------------------------*--------------|
             * |          |          |*         |          | dy
             * |          |    *     |          |          |
             * |        * |          |e         |         1|
             * | *        |e         |          |          |
             * O------------------------------------------> 
             *      1               dx
             */
            if (e > 0)
            {
                e -= dx2;
                yb += yinc;
            }

            e += dy2;
            xb += xinc;
        }
    }
    else
    {
        // done
        tb_long_t i = 0;
        tb_long_t e = dx2 - dy;
        for (i = 0; i < dy; ++i)
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, xb, yb);

            if (e > 0)
            {
                e -= dy2;
                xb += xinc;
            }

            e += dx2;
            yb += yinc;
        }
    }

    // check
    tb_assert_abort(xb == xe && yb == ye);
}
#endif
static tb_void_t gb_bitmap_render_stroke_line_vertical(gb_bitmap_biltter_ref_t biltter, tb_fixed6_t xb, tb_fixed6_t yb, tb_fixed6_t xe, tb_fixed6_t ye)
{
    // ensure the order
    if (yb > ye) 
    {
        // swap it
        tb_swap(tb_long_t, yb, ye);
    }

    // done
    gb_bitmap_biltter_done_v(biltter, tb_fixed6_round(xb), tb_fixed6_round(yb), tb_fixed6_round(ye - yb + TB_FIXED6_ONE));
}
static tb_void_t gb_bitmap_render_stroke_line_horizontal(gb_bitmap_biltter_ref_t biltter, tb_fixed6_t xb, tb_fixed6_t yb, tb_fixed6_t xe, tb_fixed6_t ye)
{
    // ensure the order
    if (xb > xe) 
    {
        // swap it
        tb_swap(tb_long_t, xb, xe);
    }

    // done
    gb_bitmap_biltter_done_h(biltter, tb_fixed6_round(xb), tb_fixed6_round(yb), tb_fixed6_round(xe - xb + TB_FIXED6_ONE));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_stroke_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && points && count && !(count & 0x1));

    // done
    tb_size_t       i   = 0;
    tb_size_t       ok  = 0;
    gb_point_ref_t  pb  = tb_null;
    gb_point_ref_t  pe  = tb_null;
    tb_fixed6_t     xb  = 0;
    tb_fixed6_t     yb  = 0;
    tb_fixed6_t     xe  = 0;
    tb_fixed6_t     ye  = 0;
    for (i = 0; i < count; i += 2)
    {
        // line: pb => pe
        pb = points + i;
        pe = points + i + 1;

        // (xb, yb) => (xe, ye)
        xb = gb_float_to_fixed6(pb->x);
        yb = gb_float_to_fixed6(pb->y);
        xe = gb_float_to_fixed6(pe->x);
        ye = gb_float_to_fixed6(pe->y);

        // done generic line
        if ((ok = gb_bitmap_render_stroke_line_generic(&device->biltter, xb, yb, xe, ye)))
        {
            // check
            tb_assert_abort(ok == 'h' || ok == 'v');

            // done horizontal line
            if (ok == 'h') gb_bitmap_render_stroke_line_horizontal(&device->biltter, xb, yb, xe, ye);
            // done vertical line
            else gb_bitmap_render_stroke_line_vertical(&device->biltter, xb, yb, xe, ye);
        }
    }
}
