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
 * @file        skia.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device_skia"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <skia/core/SkCanvas.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// float => skia scalar
#ifdef SK_SCALAR_IS_FLOAT
#   define gb_float_to_sk(x)   (SkScalar)gb_float_to_tb(x)
#else
#   define gb_float_to_sk(x)   (SkScalar)gb_float_to_fixed(x)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
// the skia device type
typedef struct __gb_skia_device_t
{
    // the base
    gb_device_impl_t            base;

    // the path
    SkPath*                     path;

    // the paint
    SkPaint*                    paint;

    // the bitmap
    SkBitmap*                   bitmap;

    // the canvas
    SkCanvas*                   canvas;

    // the points
    SkPoint*                    points;

    // the points count
    tb_size_t                   points_count;

}gb_skia_device_t, *gb_skia_device_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_skia_apply_matrix(gb_skia_device_ref_t impl, gb_matrix_ref_t matrix)
{
    // check
    tb_assert_and_check_return(impl && impl->canvas && matrix);

    // init matrix
	SkMatrix matrix_skia;
    matrix_skia.reset();
    matrix_skia.setScaleX(gb_float_to_sk(matrix->sx));
    matrix_skia.setScaleY(gb_float_to_sk(matrix->sy));
    matrix_skia.setSkewX(gb_float_to_sk(matrix->kx));
    matrix_skia.setSkewY(gb_float_to_sk(matrix->ky));
    matrix_skia.setTranslateX(gb_float_to_sk(matrix->tx));
    matrix_skia.setTranslateY(gb_float_to_sk(matrix->ty));

    // apply matrix
	impl->canvas->setMatrix(matrix_skia);
}
static tb_void_t gb_device_skia_apply_paint(gb_skia_device_ref_t impl, gb_paint_ref_t paint)
{
    // check
    tb_assert_and_check_return(impl && impl->paint && paint);

    // init paint
    impl->paint->reset();

    // init style
    tb_size_t mode = gb_paint_mode(paint);
    switch (mode)
    {
    case GB_PAINT_MODE_FILL:
	    impl->paint->setStyle(SkPaint::kFill_Style);
        break;
    case GB_PAINT_MODE_STOK:
	    impl->paint->setStyle(SkPaint::kStroke_Style);
        break;
    case GB_PAINT_MODE_FILL_STOK:
	    impl->paint->setStyle(SkPaint::kStrokeAndFill_Style);
        break;
    default:
        tb_trace_e("invalid paint mode: %lu", gb_paint_mode(paint));
        break;
    }

    // init paint for shader
    if (gb_paint_shader(paint))
    {
    }
    // init paint for solid
    else impl->paint->setColor(gb_color_pixel(gb_paint_color(paint)));

    // init paint for stroking
    if (mode & GB_PAINT_MODE_STOK)
    {
        // init stroke width
        impl->paint->setStrokeWidth(gb_float_to_sk(gb_paint_width(paint)));

        // init cap
        switch (gb_paint_cap(paint))
        {
        case GB_PAINT_CAP_BUTT:
            impl->paint->setStrokeCap(SkPaint::kButt_Cap);
            break;
        case GB_PAINT_CAP_ROUND:
            impl->paint->setStrokeCap(SkPaint::kRound_Cap);
            break;
        case GB_PAINT_CAP_SQUARE:
            impl->paint->setStrokeCap(SkPaint::kSquare_Cap);
            break;
        default:
            break;
        }

        // init join
        switch (gb_paint_join(paint))
        {
        case GB_PAINT_JOIN_MITER:
            impl->paint->setStrokeJoin(SkPaint::kMiter_Join);
            break;
        case GB_PAINT_JOIN_ROUND:
            impl->paint->setStrokeJoin(SkPaint::kRound_Join);
            break;
        case GB_PAINT_JOIN_BEVEL:
            impl->paint->setStrokeJoin(SkPaint::kBevel_Join);
            break;
        default:
            break;
        }
    }

    // init antialiasing
    tb_size_t flag = gb_paint_flag(paint);
    if (flag & GB_PAINT_FLAG_ANTIALIASING) impl->paint->setFlags(impl->paint->getFlags() | SkPaint::kAntiAlias_Flag);
    else impl->paint->setFlags(impl->paint->getFlags() & ~SkPaint::kAntiAlias_Flag);

    // init filter bitmap
    if (flag & GB_PAINT_FLAG_FILTER_BITMAP) impl->paint->setFlags(impl->paint->getFlags() | SkPaint::kFilterBitmap_Flag);
    else impl->paint->setFlags(impl->paint->getFlags() & ~SkPaint::kFilterBitmap_Flag);
}
static tb_void_t gb_device_skia_resize(gb_device_impl_t* device, tb_size_t width, tb_size_t height)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->bitmap);

    // TODO
    tb_trace_noimpl();
}
static tb_void_t gb_device_skia_draw_clear(gb_device_impl_t* device, gb_color_t color)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas);

    // clear it
	impl->canvas->drawColor((SkColor)gb_color_pixel(color));
}
static tb_void_t gb_device_skia_draw_lines(gb_device_impl_t* device, gb_point_t const* points, tb_size_t count, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && points && count);

    // apply matrix
	gb_device_skia_apply_matrix(impl, matrix);

    // apply paint
    gb_device_skia_apply_paint(impl, paint);

    // make points
    if (!impl->points) impl->points = tb_nalloc_type(count, SkPoint);
    // not enough? grow points
    else if (count > impl->points_count) impl->points = (SkPoint*)tb_ralloc(impl->points, count * sizeof(SkPoint));
    tb_assert_and_check_return(impl->points);

    // update points count
    if (count > impl->points_count) impl->points_count = count;

    // convert points
    tb_size_t i = 0;
    for (i = 0; i < count; i++) impl->points[i].set(gb_float_to_sk(points[i].x), gb_float_to_sk(points[i].y));

	// draw it
	impl->canvas->drawPoints(SkCanvas::kLines_PointMode, count, impl->points, *impl->paint);
}
static tb_void_t gb_device_skia_draw_points(gb_device_impl_t* device, gb_point_t const* points, tb_size_t count, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && points && count);

    // apply matrix
	gb_device_skia_apply_matrix(impl, matrix);

    // apply paint
    gb_device_skia_apply_paint(impl, paint);

    // make points
    if (!impl->points) impl->points = tb_nalloc_type(count, SkPoint);
    // not enough? grow points
    else if (count > impl->points_count) impl->points = (SkPoint*)tb_ralloc(impl->points, count * sizeof(SkPoint));
    tb_assert_and_check_return(impl->points);

    // update points count
    if (count > impl->points_count) impl->points_count = count;

    // convert points
    tb_size_t i = 0;
    for (i = 0; i < count; i++) impl->points[i].set(gb_float_to_sk(points[i].x), gb_float_to_sk(points[i].y));

	// draw it
	impl->canvas->drawPoints(SkCanvas::kPoints_PointMode, count, impl->points, *impl->paint);
}
static tb_void_t gb_device_skia_draw_polygon(gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && impl->path && polygon);

    // the points
    gb_point_t const*   points = polygon->points;
    tb_size_t const*    counts = polygon->counts;
    tb_assert_and_check_return(points && counts);

    // apply matrix
	gb_device_skia_apply_matrix(impl, matrix);

    // apply paint
    gb_device_skia_apply_paint(impl, paint);

    // clear path
    impl->path->reset();

    // init path
    tb_size_t count = *counts++;
    tb_size_t index = 0;
    while (index < count)
    {
        // first point?
        if (!index) impl->path->moveTo(gb_float_to_sk(points[index].x), gb_float_to_sk(points[index].y));
        else impl->path->lineTo(gb_float_to_sk(points[index].x), gb_float_to_sk(points[index].y));

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // close path
            impl->path->close();

            // next
            count = *counts++;
            index = 0;
        }
    }

	// draw it
	impl->canvas->drawPath(*impl->path, *impl->paint);
}
static gb_shader_ref_t gb_device_skia_shader_linear(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_radial(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_bitmap(gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static tb_void_t gb_device_skia_exit(gb_device_impl_t* device)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl);

    // exit path
    if (impl->path) delete impl->path;
    impl->path = tb_null;

    // exit points
    if (impl->points) tb_free(impl->points);
    impl->points = tb_null;

    // exit paint
    if (impl->paint) delete impl->paint;
    impl->paint = tb_null;

    // exit canvas
    if (impl->canvas) delete impl->canvas;
    impl->canvas = tb_null;

    // exit bitmap
    if (impl->bitmap) delete impl->bitmap;
    impl->bitmap = tb_null;

    // exit it
    tb_free(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init_skia(gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(bitmap, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_skia_device_ref_t    impl = tb_null;
    do
    {
        // the width and height
        tb_size_t width     = gb_bitmap_width(bitmap);
        tb_size_t height    = gb_bitmap_height(bitmap);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make device
        impl = tb_malloc0_type(gb_skia_device_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_BITMAP;
        impl->base.resize           = gb_device_skia_resize;
        impl->base.draw_clear       = gb_device_skia_draw_clear;
        impl->base.draw_lines       = gb_device_skia_draw_lines;
        impl->base.draw_points      = gb_device_skia_draw_points;
        impl->base.draw_polygon     = gb_device_skia_draw_polygon;
        impl->base.shader_linear    = gb_device_skia_shader_linear;
        impl->base.shader_radial    = gb_device_skia_shader_radial;
        impl->base.shader_bitmap    = gb_device_skia_shader_bitmap;
        impl->base.exit             = gb_device_skia_exit;

        // make bitmap
        impl->bitmap = new SkBitmap();
        tb_assert_and_check_break(impl->bitmap);

        // init bitmap
        impl->bitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height, gb_bitmap_row_bytes(bitmap));
	    impl->bitmap->setPixels(gb_bitmap_data(bitmap));

        // make canvas
        impl->canvas = new SkCanvas(*impl->bitmap);
        tb_assert_and_check_break(impl->canvas);

        // init canvas
	    impl->canvas->resetMatrix();

        // make paint
        impl->paint = new SkPaint();
        tb_assert_and_check_break(impl->paint);

        // make path
        impl->path = new SkPath();
        tb_assert_and_check_break(impl->path);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_device_exit((gb_device_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_device_ref_t)impl;
}

