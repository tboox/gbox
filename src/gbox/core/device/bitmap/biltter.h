/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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

    /* done biltter by pixel
     *
     * @param biltter               the biltter
     * @param x                     the x-coordinate
     * @param y                     the y-coordinate
     */
    tb_void_t                       (*done_p)(struct __gb_bitmap_biltter_t* biltter, tb_long_t x, tb_long_t y);

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

/* done biltter by pixel
 *
 * @param biltter       the biltter
 * @param x             the x-coordinate
 * @param y             the y-coordinate
 */
tb_void_t               gb_bitmap_biltter_done_p(gb_bitmap_biltter_ref_t biltter, tb_long_t x, tb_long_t y);

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


