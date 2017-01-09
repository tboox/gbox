/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        lines.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bitmap_lines"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "lines.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
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
        tb_assert(ixb < ixe);

        // done
        do
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, ixb, tb_fixed_round(start_y));

            // update the y-coordinate
            start_y += slope;
           
        } while (++ixb < ixe);
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
        tb_assert(iyb < iye);

        // done
        do
        {
            // done biltter
            gb_bitmap_biltter_done_p(biltter, tb_fixed_round(start_x), iyb);

            // update the x-coordinate
            start_x += slope;
           
        } while (++iyb < iye);
    }

    // ok
    return 0;
}
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
    tb_assert(device && points && count && !(count & 0x1));

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
            tb_assert(ok == 'h' || ok == 'v');

            // done horizontal line
            if (ok == 'h') gb_bitmap_render_stroke_line_horizontal(&device->biltter, xb, yb, xe, ye);
            // done vertical line
            else gb_bitmap_render_stroke_line_vertical(&device->biltter, xb, yb, xe, ye);
        }
    }
}
