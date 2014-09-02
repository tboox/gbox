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
 * @file        device.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "device.h"
#include "device/prefix.h"
#include "path.h"
#include "paint.h"
#include "impl/cutter/cutter.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#ifdef GB_CONFIG_THIRD_HAVE_GL
__tb_extern_c__ gb_device_ref_t gb_device_init_gl(gb_window_ref_t window);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_fill_polygon(gb_device_impl_t* impl, gb_polygon_ref_t polygon, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    tb_assert_and_check_return(impl && impl->fill_polygon);

    // fill polygon
    impl->fill_polygon(impl, polygon, matrix, paint, clipper);
}
static tb_void_t gb_device_stok_segment(gb_device_impl_t* impl, gb_segment_ref_t segment, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    tb_assert_and_check_return(impl && paint);

    // the width
    gb_float_t width = gb_paint_width(paint);

    // > 1?
    if (gb_b1(width))
    {
        // TODO
        tb_trace_noimpl();
    }
    else 
    {
        // check
        tb_assert(impl->stok_segment);

        // stok segment
        impl->stok_segment(impl, segment, matrix, paint, clipper);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init(gb_window_ref_t window)
{
    // check
    tb_assert_and_check_return_val(window, tb_null);

    // the window mode
    tb_size_t mode = gb_window_mode(window);
    tb_assert_and_check_return_val(mode, tb_null);

    // done
    gb_device_ref_t device = tb_null;
    switch (mode)
    {
#ifdef GB_CONFIG_THIRD_HAVE_GL
    case GB_WINDOW_MODE_GL:
        device = gb_device_init_gl(window);
        break;
#endif
    case GB_WINDOW_MODE_BITMAP:
        device = gb_device_init_bitmap(gb_window_bitmap(window));
        break;
    default:
        // trace
        tb_trace_e("unknown window mode: %lu", mode);
        break;
    }

    // init
    tb_bool_t ok = tb_false;
    do
    {
        // check
        gb_device_impl_t* impl = (gb_device_impl_t*)device;
        tb_assert_and_check_break(impl);

        // init pixfmt
        impl->pixfmt           = (tb_uint16_t)gb_window_pixfmt(window); 

        // init width and height
        impl->width            = (tb_uint16_t)gb_window_width(window); 
        impl->height           = (tb_uint16_t)gb_window_height(window); 

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (device) gb_device_exit(device);
        device = tb_null;
    }

    // ok?
    return device;
}
tb_void_t gb_device_exit(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // exit path
    if (impl->path) gb_path_exit(impl->path);
    impl->path = tb_null;

    // exit it
    if (impl->exit) impl->exit(impl);
}
tb_size_t gb_device_type(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_DEVICE_TYPE_NONE);

    // the type
    return impl->type;
}
tb_size_t gb_device_pixfmt(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
    return impl->pixfmt;
}
tb_size_t gb_device_width(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, 0);

    // the width
    return impl->width;
}
tb_size_t gb_device_height(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, 0);

    // the height
    return impl->height;
}
tb_void_t gb_device_resize(gb_device_ref_t device, tb_size_t width, tb_size_t height)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->resize);

    // resize
    impl->resize(impl, width, height);
}
tb_void_t gb_device_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_clear);

    // clear it
    impl->draw_clear(impl, color);
}
tb_void_t gb_device_draw_path(gb_device_ref_t device, gb_path_ref_t path, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint && path);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL) 
        gb_device_fill_polygon(impl, gb_path_polygon(path), matrix, paint, clipper);

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, gb_path_segment(path), matrix, paint, clipper);
}
tb_void_t gb_device_draw_point(gb_device_ref_t device, gb_point_ref_t point, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint && point);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // check
    tb_assert(!(mode & GB_PAINT_MODE_FILL));

    // init segment
    gb_point_t      points[] = {*point, *point};
    tb_size_t       counts[] = {2, 0};
    gb_segment_t    segment = {points, counts};

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, &segment, matrix, paint, clipper);
}
tb_void_t gb_device_draw_line(gb_device_ref_t device, gb_line_ref_t line, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // check
    tb_assert(!(mode & GB_PAINT_MODE_FILL));

    // init segment
    gb_point_t      points[] = {line->p0, line->p1};
    tb_size_t       counts[] = {2, 0};
    gb_segment_t    segment = {points, counts};

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, &segment, matrix, paint, clipper);
}
tb_void_t gb_device_draw_arc(gb_device_ref_t device, gb_arc_ref_t arc, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && arc);

    // init path
    if (!impl->path) impl->path = gb_path_init();
    tb_assert_and_check_return(impl->path);

    // make arc
    gb_path_clear(impl->path);
    gb_path_add_arc(impl->path, arc);

    // draw path
    gb_device_draw_path(device, impl->path, matrix, paint, clipper);
}
tb_void_t gb_device_draw_triangle(gb_device_ref_t device, gb_triangle_ref_t triangle, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint && triangle);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // init polygon and segment
    gb_point_t      points[] = {triangle->p0, triangle->p1, triangle->p2};
    tb_size_t       counts[] = {3, 0};
    gb_polygon_t    polygon = {points, counts};
    gb_segment_t    segment = {points, counts};

    // fill it
    if (mode & GB_PAINT_MODE_FILL) 
        gb_device_fill_polygon(impl, &polygon, matrix, paint, clipper);

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, &segment, matrix, paint, clipper);
}
tb_void_t gb_device_draw_rect(gb_device_ref_t device, gb_rect_ref_t rect, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint && rect);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // init polygon and segment
    gb_point_t      points[4];
    tb_size_t       counts[] = {4, 0};
    gb_polygon_t    polygon = {points, counts};
    gb_segment_t    segment = {points, counts};

    // init points
    points[0].x = rect->x;
    points[0].y = rect->y;
    points[1].x = rect->x + rect->w;
    points[1].y = rect->y;
    points[2].x = rect->x + rect->w;
    points[2].y = rect->y + rect->h;
    points[3].x = rect->x;
    points[3].y = rect->y + rect->h;

    // fill it
    if (mode & GB_PAINT_MODE_FILL) 
        gb_device_fill_polygon(impl, &polygon, matrix, paint, clipper);

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, &segment, matrix, paint, clipper);
}
tb_void_t gb_device_draw_circle(gb_device_ref_t device, gb_circle_ref_t circle, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && circle);

    // init path
    if (!impl->path) impl->path = gb_path_init();
    tb_assert_and_check_return(impl->path);

    // make circle
    gb_path_clear(impl->path);
    gb_path_add_circle(impl->path, circle);

    // draw path
    gb_device_draw_path(device, impl->path, matrix, paint, clipper);
}
tb_void_t gb_device_draw_ellipse(gb_device_ref_t device, gb_ellipse_ref_t ellipse, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && ellipse);

    // init path
    if (!impl->path) impl->path = gb_path_init();
    tb_assert_and_check_return(impl->path);

    // make ellipse
    gb_path_clear(impl->path);
    gb_path_add_ellipse(impl->path, ellipse);

    // draw path
    gb_device_draw_path(device, impl->path, matrix, paint, clipper);
}
tb_void_t gb_device_draw_polygon(gb_device_ref_t device, gb_polygon_ref_t polygon, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint && polygon);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // init segment
    gb_segment_t segment = {polygon->points, polygon->counts};

    // fill it
    if (mode & GB_PAINT_MODE_FILL) 
        gb_device_fill_polygon(impl, polygon, matrix, paint, clipper);

    // stok it
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, &segment, matrix, paint, clipper);
}
tb_void_t gb_device_draw_segment(gb_device_ref_t device, gb_segment_ref_t segment, gb_matrix_ref_t matrix, gb_paint_ref_t paint, gb_clipper_ref_t clipper)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && paint);

    // the mode
    tb_size_t mode = gb_paint_mode(paint);

    // check
    tb_assert(!(mode & GB_PAINT_MODE_FILL));

    // stok segment
    if (mode & GB_PAINT_MODE_STOK) 
        gb_device_stok_segment(impl, segment, matrix, paint, clipper);
}
