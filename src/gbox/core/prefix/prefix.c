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
 * @file        prefix.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../pixmap.h"
#include "../bitmap.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_long_t gb_prefix_printf_format_float(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(cstr && maxn && object, -1);

    // the value
    gb_float_t* value = (gb_float_t*)object;

    // format
#ifdef TB_CONFIG_TYPE_HAVE_FLOAT
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "%f", gb_float_to_tb(*value));
    if (size >= 0) cstr[size] = '\0';
#else
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "%ld", gb_float_to_long(*value));
    if (size >= 0) cstr[size] = '\0';
#endif

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_point(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the point
    gb_point_ref_t point = (gb_point_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(%{float}, %{float})", &point->x, &point->y);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_line(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the line
    gb_line_ref_t line = (gb_line_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(%{point} => %{point})", &line->p0, &line->p1);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_rect(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the rect
    gb_rect_ref_t rect = (gb_rect_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(x: %{float}, y: %{float}, w: %{float}, h: %{float})", &rect->x, &rect->y, &rect->w, &rect->h);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_round_rect(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the rect
    gb_round_rect_ref_t rect = (gb_round_rect_ref_t)object;

    // format
    tb_long_t size = tb_snprintf( cstr, maxn - 1
                                , "(x: %{float}, y: %{float}, w: %{float}, h: %{float}, lt: %{vector}, rt: %{vector}, rb: %{vector}, lb: %{vector})"
                                , &rect->bounds.x, &rect->bounds.y, &rect->bounds.w, &rect->bounds.h
                                , &rect->radius[GB_RECT_CORNER_LT]
                                , &rect->radius[GB_RECT_CORNER_RT]
                                , &rect->radius[GB_RECT_CORNER_RB]
                                , &rect->radius[GB_RECT_CORNER_LB]);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_triangle(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the triangle
    gb_triangle_ref_t triangle = (gb_triangle_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(%{point}, %{point}, %{point})", &triangle->p0, &triangle->p1, &triangle->p2);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_circle(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the circle
    gb_circle_ref_t circle = (gb_circle_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c: %{point}, r: %{float})", &circle->c, &circle->r);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_ellipse(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the ellipse
    gb_ellipse_ref_t ellipse = (gb_ellipse_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c: %{point}, rx: %{float}, ry: %{float})", &ellipse->c, &ellipse->rx, &ellipse->ry);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_arc(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the arc
    gb_arc_ref_t arc = (gb_arc_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c: %{point}, rx: %{float}, ry: %{float}, ab: %{float}, an: %{float})", &arc->c, &arc->rx, &arc->ry, &arc->ab, &arc->an);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_path(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the path
    gb_path_ref_t path = (gb_path_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(path: %p)", path);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_polygon(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the polygon
    gb_polygon_ref_t polygon = (gb_polygon_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(polygon: %p)", polygon);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_shape(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the shape
    gb_shape_ref_t shape = (gb_shape_ref_t)object;

    // done
    tb_long_t ok = -1;
    switch (shape->type)
    {
    case GB_SHAPE_TYPE_ARC:         ok = gb_prefix_printf_format_arc(&shape->u.arc, cstr, maxn);            break;
    case GB_SHAPE_TYPE_PATH:        ok = gb_prefix_printf_format_path(shape->u.path, cstr, maxn);           break;
    case GB_SHAPE_TYPE_LINE:        ok = gb_prefix_printf_format_line(&shape->u.line, cstr, maxn);          break;
    case GB_SHAPE_TYPE_RECT:        ok = gb_prefix_printf_format_rect(&shape->u.rect, cstr, maxn);          break;
    case GB_SHAPE_TYPE_POINT:       ok = gb_prefix_printf_format_point(&shape->u.point, cstr, maxn);        break;
    case GB_SHAPE_TYPE_CIRCLE:      ok = gb_prefix_printf_format_circle(&shape->u.circle, cstr, maxn);      break;
    case GB_SHAPE_TYPE_ELLIPSE:     ok = gb_prefix_printf_format_ellipse(&shape->u.ellipse, cstr, maxn);    break;
    case GB_SHAPE_TYPE_POLYGON:     ok = gb_prefix_printf_format_polygon(&shape->u.polygon, cstr, maxn);    break;
    case GB_SHAPE_TYPE_TRIANGLE:    ok = gb_prefix_printf_format_triangle(&shape->u.triangle, cstr, maxn);  break;
    default:                        ok = tb_snprintf(cstr, maxn, "(unknown: %u)", shape->type);             break;
    }

    // ok?
    return ok;
}
static tb_long_t gb_prefix_printf_format_color(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the color
    gb_color_ref_t color = (gb_color_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(a: %u, r: %u, g: %u, b: %u)", color->a, color->r, color->g, color->b);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_matrix(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the matrix
    gb_matrix_ref_t matrix = (gb_matrix_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(sx: %{float}, sy: %{float}, kx: %{float}, ky: %{float}, tx: %{float}, ty: %{float})", &matrix->sx, &matrix->sy, &matrix->kx, &matrix->ky, &matrix->tx, &matrix->ty);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}
static tb_long_t gb_prefix_printf_format_bitmap(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the bitmap
    gb_bitmap_ref_t bitmap = (gb_bitmap_ref_t)object;

    // format
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(%lux%lu, %s, %u)", gb_bitmap_width(bitmap), gb_bitmap_height(bitmap), gb_pixmap(gb_bitmap_pixfmt(bitmap), 0xff)->name, gb_bitmap_has_alpha(bitmap));
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return size;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_prefix_init()
{
    // register printf("%{float}", float);
    tb_printf_object_register("float", gb_prefix_printf_format_float);

    // register printf("%{point}", &point);
    tb_printf_object_register("point", gb_prefix_printf_format_point);

    // register printf("%{vector}", &vector);
    tb_printf_object_register("vector", gb_prefix_printf_format_point);

    // register printf("%{line}", &line);
    tb_printf_object_register("line", gb_prefix_printf_format_line);

    // register printf("%{rect}", &rect);
    tb_printf_object_register("rect", gb_prefix_printf_format_rect);

    // register printf("%{round_rect}", &rect);
    tb_printf_object_register("round_rect", gb_prefix_printf_format_round_rect);

    // register printf("%{triangle}", &triangle);
    tb_printf_object_register("triangle", gb_prefix_printf_format_triangle);

    // register printf("%{circle}", &circle);
    tb_printf_object_register("circle", gb_prefix_printf_format_circle);

    // register printf("%{polygon}", &polygon);
    tb_printf_object_register("polygon", gb_prefix_printf_format_polygon);

    // register printf("%{ellipse}", &ellipse);
    tb_printf_object_register("ellipse", gb_prefix_printf_format_ellipse);

    // register printf("%{arc}", &arc);
    tb_printf_object_register("arc", gb_prefix_printf_format_arc);

    // register printf("%{path}", path);
    tb_printf_object_register("path", gb_prefix_printf_format_path);

    // register printf("%{shape}", &shape);
    tb_printf_object_register("shape", gb_prefix_printf_format_shape);

    // register printf("%{color}", &color);
    tb_printf_object_register("color", gb_prefix_printf_format_color);

    // register printf("%{matrix}", &matrix);
    tb_printf_object_register("matrix", gb_prefix_printf_format_matrix);

    // register printf("%{bitmap}", bitmap);
    tb_printf_object_register("bitmap", gb_prefix_printf_format_bitmap);

    // ok
    return tb_true;
}
tb_void_t gb_prefix_exit()
{
}

