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
 * @author		ruki
 * @file		pixmap.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "pixmap.h"
#include "pixmap/pal8.h"
#include "pixmap/rgb565.h"
#include "pixmap/rgb888.h"
#include "pixmap/argb1555.h"
#include "pixmap/xrgb1555.h"
#include "pixmap/argb4444.h"
#include "pixmap/xrgb4444.h"
#include "pixmap/argb8888.h"
#include "pixmap/xrgb8888.h"
#include "pixmap/rgba5551.h"
#include "pixmap/rgbx5551.h"
#include "pixmap/rgba4444.h"
#include "pixmap/rgbx4444.h"
#include "pixmap/rgba8888.h"
#include "pixmap/rgbx8888.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals 
 */

// the pixmaps for opaque and little endian
static gb_pixmap_ref_t g_pixmaps_lo[] =
{
	&g_pixmap_o_pal8
,	&g_pixmap_lo_rgb565
,	&g_pixmap_lo_rgb888
,	&g_pixmap_lo_argb1555
,	&g_pixmap_lo_xrgb1555
,	&g_pixmap_lo_argb4444
,	&g_pixmap_lo_xrgb4444
,	&g_pixmap_lo_argb8888
,	&g_pixmap_lo_xrgb8888
,	&g_pixmap_lo_rgba5551
,	&g_pixmap_lo_rgbx5551
,	&g_pixmap_lo_rgba4444
,	&g_pixmap_lo_rgbx4444
,	&g_pixmap_lo_rgba8888
,	&g_pixmap_lo_rgbx8888

,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null

};

// the pixmaps for opaque and big endian
static gb_pixmap_ref_t g_pixmaps_bo[] =
{
	&g_pixmap_o_pal8
,	&g_pixmap_bo_rgb565
,	&g_pixmap_bo_rgb888
,	&g_pixmap_bo_argb1555
,	&g_pixmap_bo_xrgb1555
,	&g_pixmap_bo_argb4444
,	&g_pixmap_bo_xrgb4444
,	&g_pixmap_bo_argb8888
,	&g_pixmap_bo_xrgb8888
,	&g_pixmap_bo_rgba5551
,	&g_pixmap_bo_rgbx5551
,	&g_pixmap_bo_rgba4444
,	&g_pixmap_bo_rgbx4444
,	&g_pixmap_bo_rgba8888
,	&g_pixmap_bo_rgbx8888

,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null

};

// the pixmaps for alpha and little endian
static gb_pixmap_ref_t g_pixmaps_la[] =
{
	&g_pixmap_a_pal8
,	&g_pixmap_la_rgb565
,	&g_pixmap_la_rgb888
,	&g_pixmap_la_argb1555
,	&g_pixmap_la_xrgb1555
,	&g_pixmap_la_argb4444
,	&g_pixmap_la_xrgb4444
,	&g_pixmap_la_argb8888
,	&g_pixmap_la_xrgb8888
,	&g_pixmap_la_rgba5551
,	&g_pixmap_la_rgbx5551
,	&g_pixmap_la_rgba4444
,	&g_pixmap_la_rgbx4444
,	&g_pixmap_la_rgba8888
,	&g_pixmap_la_rgbx8888

,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null

};

// the pixmaps for alpha and big endian
static gb_pixmap_ref_t g_pixmaps_ba[] =
{
	&g_pixmap_a_pal8
,	&g_pixmap_ba_rgb565
,	&g_pixmap_ba_rgb888
,	&g_pixmap_ba_argb1555
,	&g_pixmap_ba_xrgb1555
,	&g_pixmap_ba_argb4444
,	&g_pixmap_ba_xrgb4444
,	&g_pixmap_ba_argb8888
,	&g_pixmap_ba_xrgb8888
,	&g_pixmap_ba_rgba5551
,	&g_pixmap_ba_rgbx5551
,	&g_pixmap_ba_rgba4444
,	&g_pixmap_ba_rgbx4444
,	&g_pixmap_ba_rgba8888
,	&g_pixmap_ba_rgbx8888

,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null
,	tb_null

};

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementions
 */
gb_pixmap_ref_t gb_pixmap(tb_size_t pixfmt, tb_byte_t alpha)
{
    // big endian?
	tb_size_t bendian = GB_PIXFMT_BE(pixfmt); 
    
    // the pixfmt
    pixfmt = GB_PIXFMT(pixfmt);
    tb_assert_abort(pixfmt);

    // opaque?
	if (alpha > GB_ALPHA_MAXN)
	{
        // check
		tb_assert_abort(pixfmt && (pixfmt - 1) < tb_arrayn(g_pixmaps_lo));

        // ok
		return bendian? g_pixmaps_bo[pixfmt - 1] : g_pixmaps_lo[pixfmt - 1];
	}
    // alpha?
	else if (alpha >= GB_ALPHA_MINN)
	{
        // check
		tb_assert_abort(pixfmt && (pixfmt - 1) < tb_arrayn(g_pixmaps_la));

        // ok
		return bendian? g_pixmaps_ba[pixfmt - 1] : g_pixmaps_la[pixfmt - 1];
	}

	// transparent
	return tb_null;
}

