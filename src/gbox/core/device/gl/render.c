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
 * @file        render.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_render"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "render.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the vertex type
#if defined(GB_CONFIG_FLOAT_FIXED) && defined(GB_GL_FIXED)
#   define GB_GL_VERTEX_TYPE            GB_GL_FIXED
#elif defined(GB_CONFIG_FLOAT_FIXED)
#   define GB_GL_VERTEX_TYPE            GL_GL_INT
#else
#   define GB_GL_VERTEX_TYPE            GB_GL_FLOAT
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_gl_render_apply_vertices(gb_gl_device_ref_t device, gb_point_ref_t points)
{
    // check
    tb_assert_abort(device && points);
 
    // apply vertices
    if (device->version >= 0x20) 
    {
        // check
        tb_assert_abort(device->render.program);

        // apply it
        gb_glVertexAttribPointer(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_VERTICES), 2, GB_GL_VERTEX_TYPE, GB_GL_FALSE, 0, points);
    }
    else 
    {
        // apply it
        gb_glVertexPointer(2, GB_GL_VERTEX_TYPE, 0, points);
    }
}
static tb_void_t gb_gl_render_enter_solid(gb_gl_device_ref_t device)
{
    // check
    tb_assert_abort(device);
 
    // the paint
    gb_paint_ref_t paint = device->base.paint;
    tb_assert_abort(paint);

    // the color 
    gb_color_t color = gb_paint_color(paint);

    // disable texture
    gb_glDisable(GB_GL_TEXTURE_2D);

    // enable blend?
    if (color.a != 0xff)
    {
        gb_glEnable(GB_GL_BLEND);
        gb_glBlendFunc(GB_GL_SRC_ALPHA, GB_GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        // disable blend
        gb_glDisable(GB_GL_BLEND);
    }

    // apply color
    if (device->version >= 0x20)
    {
        // check
        tb_assert_abort(device->render.program);

        // apply it
        gb_glVertexAttrib4f(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_COLORS), (gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
    }
    else
    {
        // apply it
        gb_glColor4f((gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
    }
}
static tb_void_t gb_gl_render_leave_solid(gb_gl_device_ref_t device)
{    
    // check
    tb_assert_abort(device);
 
    // disable blend
    gb_glDisable(GB_GL_BLEND);
}
static tb_void_t gb_gl_render_enter_shader(gb_gl_device_ref_t device)
{   
    // check
    tb_assert_abort(device);
 
    // the paint
    gb_paint_ref_t paint = device->base.paint;
    tb_assert_abort(paint);

    // disable blend
    gb_glDisable(GB_GL_BLEND);

    // enable texture
    gb_glEnable(GB_GL_TEXTURE_2D);
}
static tb_void_t gb_gl_render_leave_shader(gb_gl_device_ref_t device)
{   
    // check
    tb_assert_abort(device);
 
    // disable texture
    gb_glDisable(GB_GL_TEXTURE_2D);
}
static tb_void_t gb_gl_render_enter_paint(gb_gl_device_ref_t device)
{
    // check
    tb_assert_abort(device);

    // enter shader
    if (device->render.shader) gb_gl_render_enter_shader(device);
    // enter solid
    else gb_gl_render_enter_solid(device);
}
static tb_void_t gb_gl_render_leave_paint(gb_gl_device_ref_t device)
{
    // check
    tb_assert_abort(device);

    // leave shader
    if (device->render.shader) gb_gl_render_leave_shader(device);
    // leave solid
    else gb_gl_render_leave_solid(device);
}
static tb_void_t gb_gl_render_fill_polygon(gb_gl_device_ref_t device, gb_point_ref_t points, tb_uint16_t const* counts)
{
    // check
    tb_assert_abort(device && points && counts);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    tb_uint16_t count;
    tb_size_t   index = 0;
    while ((count = *counts++))
    {
        gb_glDrawArrays(GB_GL_TRIANGLE_FAN, (gb_GLint_t)index, (gb_GLint_t)count);
        index += count;
    }
}
static tb_void_t gb_gl_render_stroke_lines(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && points && count);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    gb_glDrawArrays(GB_GL_LINES, 0, (gb_GLint_t)count);
}
static tb_void_t gb_gl_render_stroke_points(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_abort(device && points && count);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    gb_glDrawArrays(GB_GL_POINTS, 0, (gb_GLint_t)count);
}
static tb_void_t gb_gl_render_stroke_polygon(gb_gl_device_ref_t device, gb_point_ref_t points, tb_uint16_t const* counts)
{
    // check
    tb_assert_abort(device && points && counts);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    tb_uint16_t count;
    tb_size_t   index = 0;
    while ((count = *counts++))
    {
        gb_glDrawArrays(GB_GL_LINE_STRIP, (gb_GLint_t)index, (gb_GLint_t)count);
        index += count;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_gl_render_init(gb_gl_device_ref_t device)
{
    // check
    tb_assert_and_check_return_val(device && device->base.matrix && device->base.paint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // init render
        tb_memset(&device->render, 0, sizeof(device->render));

        // init shader
        device->render.shader = gb_paint_shader(device->base.paint);

        // init vertex matrix
        gb_gl_matrix_convert(device->render.matrix_vertex, device->base.matrix);

        // apply matrix for the fixed vertex if no GB_GL_FIXED macro
#if defined(GB_CONFIG_FLOAT_FIXED) && !defined(GB_GL_FIXED)
        device->render.matrix_vertex[0] /= 65536.0f;
        device->render.matrix_vertex[1] /= 65536.0f;
        device->render.matrix_vertex[4] /= 65536.0f;
        device->render.matrix_vertex[5] /= 65536.0f;
#endif

        // init antialiasing
        if (gb_paint_flag(device->base.paint) & GB_PAINT_FLAG_ANTIALIASING) 
        {
            gb_glEnable(GB_GL_MULTISAMPLE);
#if 0
            gb_glEnable(GB_GL_LINE_SMOOTH);
            gb_glHint(GB_GL_LINE_SMOOTH_HINT, GB_GL_NICEST);
#endif
        }
        else gb_glDisable(GB_GL_MULTISAMPLE);

        // init vertex and matrix
        if (device->version >= 0x20)
        {   
            // the program type
            tb_size_t program_type = device->render.shader? GB_GL_PROGRAM_TYPE_BITMAP : GB_GL_PROGRAM_TYPE_COLOR;

            // program
            device->render.program = device->programs[program_type];
            tb_assert_and_check_break(device->render.program);

            // bind this program to the current gl context
            gb_gl_program_bind(device->render.program);

            // enable vertex
            gb_glEnableVertexAttribArray(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_VERTICES));

            // apply projection matrix
            gb_glUniformMatrix4fv(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_MATRIX_PROJECT), 1, GB_GL_FALSE, device->matrix_project);

            // apply vertex matrix
            gb_glUniformMatrix4fv(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_MATRIX_MODEL), 1, GB_GL_FALSE, device->render.matrix_vertex);
        }
        else
        {
            // enable vertex
            gb_glEnableClientState(GB_GL_VERTEX_ARRAY);
    
            // apply vertex matrix
            gb_glMatrixMode(GB_GL_MODELVIEW);
            gb_glPushMatrix();
            gb_glLoadIdentity();
            gb_glMultMatrixf(device->render.matrix_vertex);
        }

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_void_t gb_gl_render_exit(gb_gl_device_ref_t device)
{
    // check
    tb_assert_and_check_return(device);

    // exit vertex and matrix
    if (device->version >= 0x20)
    {   
        // check
        tb_assert_and_check_return(device->render.program);

        // disable vertex
        gb_glDisableVertexAttribArray(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_VERTICES));
 
        // disable texcoord
        gb_glDisableVertexAttribArray(gb_gl_program_location(device->render.program, GB_GL_PROGRAM_LOCATION_TEXCOORDS));
    }
    else
    {
        // restore vertex matrix
        gb_glMatrixMode(GB_GL_MODELVIEW);
        gb_glPopMatrix();

        // disable vertex
        gb_glDisableClientState(GB_GL_VERTEX_ARRAY);
 
        // disable texcoord
        gb_glDisableClientState(GB_GL_TEXTURE_COORD_ARRAY);
    }
 
    // disable antialiasing
    gb_glDisable(GB_GL_MULTISAMPLE);
}
tb_void_t gb_gl_render_draw_lines(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint && points && count);
    tb_assert_abort(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // enter paint
    gb_gl_render_enter_paint(device);

    // the width
    gb_float_t width = gb_paint_width(device->base.paint);

    // width == 1? stroke lines
    if (gb_e1(width) && gb_gl_matrix_identify_for_scale(device->render.matrix_vertex))
        gb_gl_render_stroke_lines(device, points, count);
    else
    {
        // TODO
        gb_gl_render_stroke_lines(device, points, count);
    }

    // leave paint
    gb_gl_render_leave_paint(device);
}
tb_void_t gb_gl_render_draw_points(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint && points && count);
    tb_assert_abort(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // enter paint
    gb_gl_render_enter_paint(device);

    // the width
    gb_float_t width = gb_paint_width(device->base.paint);

    // width == 1? stroke points
    if (gb_e1(width) && gb_gl_matrix_identify_for_scale(device->render.matrix_vertex))
        gb_gl_render_stroke_points(device, points, count);
    else
    {
        // TODO
        gb_gl_render_stroke_points(device, points, count);
    }

    // leave paint
    gb_gl_render_leave_paint(device);
}
tb_void_t gb_gl_render_draw_polygon(gb_gl_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(device && device->base.paint && polygon && polygon->points && polygon->counts);

    // enter paint
    gb_gl_render_enter_paint(device);

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        // fill polygon
        gb_gl_render_fill_polygon(device, polygon->points, polygon->counts);
    }

    // stroke it
    if (mode & GB_PAINT_MODE_STROKE)
    {
        // the width
        gb_float_t width = gb_paint_width(device->base.paint);

        // width == 1? stroke polygon
        if (gb_e1(width) && gb_gl_matrix_identify_for_scale(device->render.matrix_vertex))
            gb_gl_render_stroke_polygon(device, polygon->points, polygon->counts);
        else
        {
            // TODO
            gb_gl_render_stroke_polygon(device, polygon->points, polygon->counts);
        }
    }

    // leave paint
    gb_gl_render_leave_paint(device);
}

