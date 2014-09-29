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
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_ref_t bitmap, gb_pixmap_ref_t pixmap, gb_pixel_t pixel, tb_byte_t alpha, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // the factors
    tb_byte_t*                  data        = gb_bitmap_data(bitmap);
    tb_size_t                   btp         = pixmap->btp;
    tb_size_t                   row_bytes   = gb_bitmap_row_bytes(bitmap);
    gb_pixmap_func_pixel_set_t  pixel_set   = pixmap->pixel_set;

    // the dx and the x-increased bytes
    tb_long_t dx    = xe - xb;
    tb_long_t xinc  = btp;
    if (dx < 0) 
    {
        dx = -dx;
        xinc = -xinc;
    }

    // the dy and the y-increased bytes
    tb_long_t dy    = ye - yb;
    tb_long_t yinc  = row_bytes;
    if (dy < 0) 
    {
        dy = -dy;
        yinc = -yinc;
    }

    // dx * 2, dy * 2 for float => fixed
    tb_long_t dx2 = dx << 1;
    tb_long_t dy2 = dy << 1;

    // draw 1 / 2 line
    tb_long_t dx12 = (dx + 1) >> 1;
    tb_long_t dy12 = (dy + 1) >> 1;
   
    // |slope| < 1?
    tb_byte_t* qb = data + yb * row_bytes + xb * btp;
    tb_byte_t* qe = data + ye * row_bytes + xe * btp;
    if (dx > dy)
    {
        // done 
        tb_long_t i = 0;
        tb_long_t eb = dy2 - dx;
        tb_long_t ee = eb;
        for (i = 0; i < dx12; ++i)
        {
            // set pixel
            pixel_set(qb, pixel, alpha);
            pixel_set(qe, pixel, alpha);

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
                qb += yinc;
            }
            
            if (ee >= 0)
            {
                ee -= dx2;
                qe -= yinc;
            }

            eb += dy2;
            ee += dy2;
            qb += xinc;
            qe -= xinc;
        }


        /* draw pm if line is (qb, ..., pm, ..., qe)
         * e.g. (0, 1, 2)
         */
        if (!(dx & 0x1)) pixel_set(qb, pixel, alpha);
    }
    else
    {
        // done
        tb_long_t i = 0;
        tb_long_t eb = dx2 - dy;
        tb_long_t ee = eb;
        for (i = 0; i < dy12; ++i)
        {
            pixel_set(qb, pixel, alpha);
            pixel_set(qe, pixel, alpha);

            if (eb > 0)
            {
                eb -= dy2;
                qb += xinc;
            }

            if (ee >= 0)
            {
                ee -= dy2;
                qe -= xinc;
            }

            eb += dx2;
            ee += dx2;
            qb += yinc;
            qe -= yinc;
        }

        if (!(dy & 0x1)) pixel_set(qb, pixel, alpha);
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
static tb_void_t gb_bitmap_render_stroke_line_generic(gb_bitmap_ref_t bitmap, gb_pixmap_ref_t pixmap, gb_pixel_t pixel, tb_byte_t alpha, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // the factors
    tb_byte_t*                  data        = gb_bitmap_data(bitmap);
    tb_size_t                   btp         = pixmap->btp;
    tb_size_t                   row_bytes   = gb_bitmap_row_bytes(bitmap);
    gb_pixmap_func_pixel_set_t  pixel_set   = pixmap->pixel_set;

    // the dx and the x-increased bytes
    tb_long_t dx    = xe - xb;
    tb_long_t xinc  = btp;
    if (dx < 0) 
    {
        dx = -dx;
        xinc = -xinc;
    }

    // the dy and the y-increased bytes
    tb_long_t dy    = ye - yb;
    tb_long_t yinc  = row_bytes;
    if (dy < 0) 
    {
        dy = -dy;
        yinc = -yinc;
    }

    // dx * 2, dy * 2 for float => fixed
    tb_long_t dx2 = dx << 1;
    tb_long_t dy2 = dy << 1;

    // |slope| < 1?
    tb_byte_t* q = data + yb * row_bytes + xb * btp;
    if (dx > dy)
    {
        // done 
        tb_long_t i = 0;
        tb_long_t e = dy2 - dx;
        for (i = 0; i < dx; ++i)
        {
            // set pixel
            pixel_set(q, pixel, alpha);

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
                q += yinc;
            }

            e += dy2;
            q += xinc;
        }
    }
    else
    {
        // done
        tb_long_t i = 0;
        tb_long_t e = dx2 - dy;
        for (i = 0; i < dy; ++i)
        {
            pixel_set(q, pixel, alpha);

            if (e > 0)
            {
                e -= dy2;
                q += xinc;
            }

            e += dx2;
            q += yinc;
        }
    }

    // check
    tb_assert_abort(q == data + ye * row_bytes + xe * btp);
}
#endif
static tb_void_t gb_bitmap_render_stroke_line_vertical(gb_bitmap_ref_t bitmap, gb_pixmap_ref_t pixmap, gb_pixel_t pixel, tb_byte_t alpha, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // the factors
    tb_byte_t*  data        = gb_bitmap_data(bitmap);
    tb_size_t   btp         = pixmap->btp;
    tb_size_t   row_bytes   = gb_bitmap_row_bytes(bitmap);
    if (yb > ye) 
    {
        tb_long_t yt = yb;
        yb = ye;
        ye = yt;
    }

    // done
    tb_long_t                   h = ye - yb + 1;
    tb_byte_t*                  q = data + yb * row_bytes + xb * btp;
    gb_pixmap_func_pixel_set_t  pixel_set = pixmap->pixel_set;
    while (h--)
    {
        pixel_set(q, pixel, alpha);
        q += row_bytes;
    }
}
static tb_void_t gb_bitmap_render_stroke_line_horizontal(gb_bitmap_ref_t bitmap, gb_pixmap_ref_t pixmap, gb_pixel_t pixel, tb_byte_t alpha, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // the factors
    tb_byte_t*  data        = gb_bitmap_data(bitmap);
    tb_size_t   btp         = pixmap->btp;
    tb_size_t   row_bytes   = gb_bitmap_row_bytes(bitmap);
    if (xb > xe) 
    {
        tb_long_t xt = xb;
        xb = xe;
        xe = xt;
    }

    // done
    if (xb != xe) pixmap->pixels_fill(data + yb * row_bytes + xb * btp, pixel, xe - xb + 1, alpha);
    else pixmap->pixel_set(data + yb * row_bytes + xb * btp, pixel, alpha);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_bitmap_render_stroke_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && device->base.paint && device->pixmap && device->bitmap && gb_bitmap_data(device->bitmap));
    tb_assert_abort(points && count && !(count & 0x1));

    // done
    tb_size_t       i       = 0;
    gb_point_ref_t  pb      = tb_null;
    gb_point_ref_t  pe      = tb_null;
    gb_pixmap_ref_t pixmap  = device->pixmap;
    gb_pixel_t      pixel   = pixmap->pixel(gb_paint_color(device->base.paint));
    tb_byte_t       alpha   = gb_paint_alpha(device->base.paint);
    gb_bitmap_ref_t bitmap  = device->bitmap;
    tb_long_t       xb      = 0;
    tb_long_t       yb      = 0;
    tb_long_t       xe      = 0;
    tb_long_t       ye      = 0;
    for (i = 0; i < count; i += 2)
    {
        // line: pb => pe
        pb = points + i;
        pe = points + i + 1;

        // (xb, yb) => (xe, ye)
        xb = gb_round(pb->x);
        yb = gb_round(pb->y);
        xe = gb_round(pe->x);
        ye = gb_round(pe->y);

        // horizontal?
        if (yb == ye) gb_bitmap_render_stroke_line_horizontal(bitmap, pixmap, pixel, alpha, xb, yb, xe, ye);
        // vertical?
        else if (xb == xe) gb_bitmap_render_stroke_line_vertical(bitmap, pixmap, pixel, alpha, xb, yb, xe, ye);
        // generic?
        else gb_bitmap_render_stroke_line_generic(bitmap, pixmap, pixel, alpha, xb, yb, xe, ye);
    }
}
