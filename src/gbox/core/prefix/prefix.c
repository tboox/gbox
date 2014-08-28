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
 * @file        prefix.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_long_t gb_prefix_printf_format_float(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(cstr && maxn, -1);

    // the value
    gb_float_t value = (gb_float_t)object;

    // format
#ifdef TB_CONFIG_TYPE_FLOAT
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "%0.3f", gb_float_to_tb(value));
    if (size >= 0) cstr[size] = '\0';
#else
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "%ld", gb_float_to_long(value));
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(%{float}, %{float})", point->x, point->y);
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(x: %{float}, y: %{float}, w: %{float}, h: %{float})", rect->x, rect->y, rect->w, rect->h);
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c: %{point}, r: %{float})", &circle->c, circle->r);
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c0: %{point}, rx: %{float}, ry: %{float})", &ellipse->c0, ellipse->rx, ellipse->ry);
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(c0: %{point}, rx: %{float}, ry: %{float}, ab: %{float}, an: %{float})", &arc->c0, arc->rx, arc->ry, arc->ab, arc->an);
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
    tb_long_t size = tb_snprintf(cstr, maxn - 1, "(sx: %{float}, sy: %{float}, kx: %{float}, ky: %{float}, tx: %{float}, ty: %{float})", matrix->sx, matrix->sy, matrix->kx, matrix->ky, matrix->tx, matrix->ty);
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

    // register printf("%{line}", &line);
    tb_printf_object_register("line", gb_prefix_printf_format_line);

    // register printf("%{rect}", &rect);
    tb_printf_object_register("rect", gb_prefix_printf_format_rect);

    // register printf("%{triangle}", &triangle);
    tb_printf_object_register("triangle", gb_prefix_printf_format_triangle);

    // register printf("%{circle}", &circle);
    tb_printf_object_register("circle", gb_prefix_printf_format_circle);

    // register printf("%{ellipse}", &ellipse);
    tb_printf_object_register("ellipse", gb_prefix_printf_format_ellipse);

    // register printf("%{arc}", &arc);
    tb_printf_object_register("arc", gb_prefix_printf_format_arc);

    // register printf("%{matrix}", &matrix);
    tb_printf_object_register("matrix", gb_prefix_printf_format_matrix);

    // ok
    return tb_true;
}
tb_void_t gb_prefix_exit()
{
}

