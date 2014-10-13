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
#if 1
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_biltter_ref_t biltter, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
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
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_biltter_ref_t biltter, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
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
static tb_void_t gb_bitmap_render_stroke_line_vertical(gb_bitmap_biltter_ref_t biltter, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // ensure the order
    if (yb > ye) 
    {
        // swap it
        tb_swap(tb_long_t, yb, ye);
    }

    // done
    gb_bitmap_biltter_done_v(biltter, xb, yb, ye - yb + 1);
}
static tb_void_t gb_bitmap_render_stroke_line_horizontal(gb_bitmap_biltter_ref_t biltter, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // ensure the order
    if (xb > xe) 
    {
        // swap it
        tb_swap(tb_long_t, xb, xe);
    }

    // done
    gb_bitmap_biltter_done_h(biltter, xb, yb, xe - xb + 1);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_stroke_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && points && count && !(count & 0x1));

    // done
    tb_size_t       i       = 0;
    gb_point_ref_t  pb      = tb_null;
    gb_point_ref_t  pe      = tb_null;
    tb_long_t       xb      = 0;
    tb_long_t       yb      = 0;
    tb_long_t       xe      = 0;
    tb_long_t       ye      = 0;
    for (i = 0; i < count; i += 2)
    {
        // line: pb => pe
        pb = points + i;
        pe = points + i + 1;

        // FIXME: 45 degree and (0.5, 1) => (1, 1.5)?
        // ...

        // (xb, yb) => (xe, ye)
        xb = gb_round(pb->x);
        yb = gb_round(pb->y);
        xe = gb_round(pe->x);
        ye = gb_round(pe->y);

        // horizontal?
        if (yb == ye) gb_bitmap_render_stroke_line_horizontal(&device->biltter, xb, yb, xe, ye);
        // vertical?
        else if (xb == xe) gb_bitmap_render_stroke_line_vertical(&device->biltter, xb, yb, xe, ye);
        // generic?
        else gb_bitmap_render_stroke_line_generic(&device->biltter, xb, yb, xe, ye);
    }
}
