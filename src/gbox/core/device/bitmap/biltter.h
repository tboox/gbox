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
 * @file        biltter.h
 * @ingroup     core
 */
#ifndef GB_CORE_DEVICE_BITMAP_BILLTER_H
#define GB_CORE_DEVICE_BITMAP_BILLTER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the bitmap biltter solid type
typedef struct __gb_bitmap_biltter_solid_t
{
    // the pixel
    gb_pixel_t                      pixel;

    // the alpha
    tb_byte_t                       alpha;

}gb_bitmap_biltter_solid_t;

// the bitmap biltter type
typedef struct __gb_bitmap_biltter_t
{
    union 
    {
        // the solid biltter
        gb_bitmap_biltter_solid_t    solid;

    }u;

    // the bitmap
    gb_bitmap_ref_t                 bitmap;

    // the pixmap
    gb_pixmap_ref_t                 pixmap;

    // the btp of the bitmap 
    tb_size_t                       btp;

    // the row bytes of the bitmap
    tb_size_t                       row_bytes;

    /* exit the biltter
     *
     * @param biltter               the biltter 
     */
    tb_void_t                       (*exit)(struct __gb_bitmap_biltter_t* biltter);

    /* done biltter by horizontal
     *
     * @param biltter               the biltter
     * @param x                     the start x-coordinate
     * @param y                     the start y-coordinate
     * @param w                     the width
     */
    tb_void_t                       (*done_h)(struct __gb_bitmap_biltter_t* biltter, tb_long_t x, tb_long_t y, tb_long_t w);

    /* done biltter by vertical
     *
     * @param biltter               the biltter
     * @param x                     the start x-coordinate
     * @param y                     the start y-coordinate
     * @param h                     the height
     */
    tb_void_t                       (*done_v)(struct __gb_bitmap_biltter_t* biltter, tb_long_t x, tb_long_t y, tb_long_t h);

    /* done biltter by rect
     *
     * @param biltter               the biltter
     * @param x                     the start x-coordinate
     * @param y                     the start y-coordinate
     * @param w                     the width
     * @param h                     the height
     */
    tb_void_t                       (*done_r)(struct __gb_bitmap_biltter_t* biltter, tb_long_t x, tb_long_t y, tb_long_t w, tb_long_t h);

}gb_bitmap_biltter_t, *gb_bitmap_biltter_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init biltter
 *
 * @param biltter       the biltter
 * @param bitmap        the bitmap
 * @param paint         the paint
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               gb_bitmap_biltter_init(gb_bitmap_biltter_ref_t biltter, gb_bitmap_ref_t bitmap, gb_paint_ref_t paint);

/* exit biltter
 *
 * @param biltter       the biltter
 */
tb_void_t               gb_bitmap_biltter_exit(gb_bitmap_biltter_ref_t biltter);

/* done biltter by horizontal
 *
 * @param biltter       the biltter
 * @param x             the start x-coordinate
 * @param y             the start y-coordinate
 * @param w             the width
 */
tb_void_t               gb_bitmap_biltter_done_h(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w);

/* done biltter by vertical
 *
 * @param biltter       the biltter
 * @param x             the start x-coordinate
 * @param y             the start y-coordinate
 * @param h             the height
 */
tb_void_t               gb_bitmap_biltter_done_v(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t h);

/* done biltter by rect
 *
 * @param biltter       the biltter
 * @param x             the start x-coordinate
 * @param y             the start y-coordinate
 * @param w             the width
 * @param h             the height
 */
tb_void_t               gb_bitmap_biltter_done_r(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y, tb_long_t w, tb_long_t h);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif


