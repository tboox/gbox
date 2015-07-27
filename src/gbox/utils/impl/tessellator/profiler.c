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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        profiler.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "profiler"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "profiler.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the profiler 
static tb_stream_ref_t  g_profiler = tb_null;

// the head
static tb_char_t const* g_head = "\
<!DOCTYPE html>\n\
<html>\n\
    <head>\n\
        <meta charset=\"utf-8\">\n\
        <title>PolygonTessellationProfiler</title>\n\
        <script src=\"raphael.js\"></script>\n\
        <script>\n\
        \n\
            // size\n\
            var width = %{float};\n\
            var height = %{float};\n\
\n\
            // translate\n\
            var tx = %{float};\n\
            var ty = %{float};\n\
\n\
            function add_edge(canvas, id, vb, ve, x1, y1, x2, y2)\n\
            {\n\
                var edge = canvas.path(\"M\" + x1 + \", \" + y1 + \"L\" + x2 + \", \" + y2);\n\
                edge.translate(tx, ty);\n\
                edge.attr(\"stroke\", \"blue\");\n\
\n\
                var text = canvas.text((x1 + x2) / 2, (y1 + y2) / 2, id);\n\
                text.translate(tx, ty).scale(0.7);\n\
                text.attr(\"fill\", \"red\");\n\
\n\
                canvas.text(x1, y1, vb).translate(tx, ty).scale(0.7);\n\
                canvas.text(x2, y2, ve).translate(tx, ty).scale(0.7);\n\
\n\
                return edge;\n\
            }\n\
            function add_split(canvas, id, vb, ve, x1, y1, x2, y2)\n\
            {\n\
                var edge = canvas.path(\"M\" + x1 + \", \" + y1 + \"L\" + x2 + \", \" + y2);\n\
                edge.translate(tx, ty);\n\
                edge.attr(\"stroke\", \"pink\");\n\
\n\
                var text = canvas.text((x1 + x2) / 2, (y1 + y2) / 2, id);\n\
                text.translate(tx, ty).scale(0.7);\n\
                text.attr(\"fill\", \"green\");\n\
\n\
                canvas.text(x1, y1, vb).translate(tx, ty).scale(0.7);\n\
                canvas.text(x2, y2, ve).translate(tx, ty).scale(0.7);\n\
\n\
                return edge;\n\
            }\n\
            function add_patch(canvas, id, vb, ve, x1, y1, x2, y2)\n\
            {\n\
                var edge = canvas.path(\"M\" + x1 + \", \" + y1 + \"L\" + x2 + \", \" + y2);\n\
                edge.translate(tx, ty);\n\
                edge.attr(\"stroke\", \"lightgrey\");\n\
\n\
                var text = canvas.text((x1 + x2) / 2, (y1 + y2) / 2, id);\n\
                text.translate(tx, ty).scale(0.7);\n\
                text.attr(\"fill\", \"lightgrey\");\n\
\n\
                canvas.text(x1, y1, vb).translate(tx, ty).scale(0.7);\n\
                canvas.text(x2, y2, ve).translate(tx, ty).scale(0.7);\n\
\n\
                return edge;\n\
            }\n\
            function add_inter(canvas, vi, x, y)\n\
            {\n\
                var text = canvas.text(x, y, vi);\n\
                text.translate(tx, ty).scale(0.7);\n\
            }\n\
            function finish_face(canvas, x, y)\n\
            {\n\
                var text = canvas.text(x, y, \"*\");\n\
                text.translate(tx, ty).scale(0.7);\n\
                text.attr(\"fill\", \"red\");\n\
            }\n\
            function finish_region(canvas, x1, y1, x2, y2)\n\
            {\n\
                var text = canvas.text((x1 + x2) / 2, (y1 + y2) / 2, \"__\");\n\
                text.translate(tx, ty).scale(0.7);\n\
                text.attr(\"fill\", \"red\");\n\
            }\n\
            window.onload = function ()\n\
            {\n\
                // canvas\n\
                var canvas = Raphael(0, 0, width, height); \n\
\n\
                add_edge(canvas, \"ebl\", \"\", \"\", %{float}, %{float}, %{float}, %{float});\n\
                add_edge(canvas, \"ebr\", \"\", \"\", %{float}, %{float}, %{float}, %{float});\n\
\n\
";

// the tail
static tb_char_t const* g_tail = "\
            };\n\
        </script>\n\
    </head>\n\
    <body>\n\
        <div id=\"holder\"></div>\n\
    </body>\n\
</html>\n\
";

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_profiler_init(gb_rect_ref_t bounds)
{
    // check
    tb_assert_and_check_return_val(bounds, tb_false);

    // done
    tb_bool_t   ok = tb_false;
    tb_char_t*  head = tb_null;
    tb_size_t   head_maxn = tb_strlen(g_head) + 256;
    do
    {
        // check
        tb_assert_abort(!g_profiler);

        // init sequence
        static tb_size_t s_sequence = 0;

        // get the temporary directory
        tb_char_t temp[TB_PATH_MAXN];
        tb_size_t size = tb_directory_temporary(temp, sizeof(temp));
        tb_assert_and_check_break(size);

        // make the profiler file path
        tb_snprintf(temp + size, sizeof(temp) - size, "/profiler_%lu.html", s_sequence++);

        // make profiler
        g_profiler = tb_stream_init_from_file(temp, TB_FILE_MODE_RW | TB_FILE_MODE_CREAT | TB_FILE_MODE_TRUNC);
        tb_assert_and_check_break(g_profiler);

        // open profiler
        if (!tb_stream_open(g_profiler)) break;

        // make head
        head = (tb_char_t*)tb_malloc0(head_maxn);
        tb_assert_and_check_break(head);

        // the width and height
        gb_float_t width    = bounds->w + gb_long_to_float(64);
        gb_float_t height   = bounds->h + gb_long_to_float(64);

        // the tx and ty
        gb_float_t tx = -bounds->x + gb_long_to_float(32);
        gb_float_t ty = -bounds->y + gb_long_to_float(32);

        // the left vertical line
        gb_float_t lx1 = bounds->x;
        gb_float_t ly1 = bounds->y;
        gb_float_t lx2 = bounds->x;
        gb_float_t ly2 = bounds->y + bounds->h;

        // the right vertical line
        gb_float_t rx1 = bounds->x + bounds->w;
        gb_float_t ry1 = bounds->y;
        gb_float_t rx2 = bounds->x + bounds->w;
        gb_float_t ry2 = bounds->y + bounds->h;

        // format head
        tb_size_t head_size = tb_snprintf(  head, head_maxn, g_head
                                        ,   &width, &height, &tx, &ty
                                        ,   &lx1, &ly1, &lx2, &ly2
                                        ,   &rx1, &ry1, &rx2, &ry2);

        // write head 
        if (!tb_stream_bwrit(g_profiler, (tb_byte_t const*)head, head_size)) break;

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (g_profiler) tb_stream_exit(g_profiler);
        g_profiler = tb_null;
    }

    // exit head
    if (head) tb_free(head);
    head = tb_null;

    // ok?
    return ok;
}
tb_void_t gb_tessellator_profiler_exit()
{
    // check
    tb_assert_and_check_return(g_profiler);

    // write the tail
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)g_tail, tb_strlen(g_tail));

    // exit it
    tb_stream_exit(g_profiler);
    g_profiler = tb_null;
}
tb_void_t gb_tessellator_profiler_add_edge(gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return(g_profiler && edge);

    // the vertices
    gb_mesh_vertex_ref_t org = gb_mesh_edge_org(edge);
    gb_mesh_vertex_ref_t dst = gb_mesh_edge_dst(edge);

    // the points
    gb_point_ref_t point_org = gb_tessellator_vertex_point(org);
    gb_point_ref_t point_dst = gb_tessellator_vertex_point(dst);

    // make line
    tb_char_t line[256] = {0};
    tb_long_t size = tb_snprintf(line, sizeof(line), "                add_edge(canvas, \"e%lu\", \"v%lu\", \"v%lu\", %{float}, %{float}, %{float}, %{float});\n", edge->id, org->id, dst->id, &point_org->x, &point_org->y, &point_dst->x, &point_dst->y);
    tb_assert_and_check_return(size > 0);

    // write line
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)line, size);
}
tb_void_t gb_tessellator_profiler_add_split(gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return(g_profiler && edge);

    // the vertices
    gb_mesh_vertex_ref_t org = gb_mesh_edge_org(edge);
    gb_mesh_vertex_ref_t dst = gb_mesh_edge_dst(edge);

    // the points
    gb_point_ref_t point_org = gb_tessellator_vertex_point(org);
    gb_point_ref_t point_dst = gb_tessellator_vertex_point(dst);

    // make line
    tb_char_t line[256] = {0};
    tb_long_t size = tb_snprintf(line, sizeof(line), "                add_split(canvas, \"e%lu\", \"v%lu\", \"v%lu\", %{float}, %{float}, %{float}, %{float});\n", edge->id, org->id, dst->id, &point_org->x, &point_org->y, &point_dst->x, &point_dst->y);
    tb_assert_and_check_return(size > 0);

    // write line
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)line, size);
}
tb_void_t gb_tessellator_profiler_add_patch(gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert_and_check_return(g_profiler && edge);

    // the vertices
    gb_mesh_vertex_ref_t org = gb_mesh_edge_org(edge);
    gb_mesh_vertex_ref_t dst = gb_mesh_edge_dst(edge);

    // the points
    gb_point_ref_t point_org = gb_tessellator_vertex_point(org);
    gb_point_ref_t point_dst = gb_tessellator_vertex_point(dst);

    // make line
    tb_char_t line[256] = {0};
    tb_long_t size = tb_snprintf(line, sizeof(line), "                add_patch(canvas, \"e%lu\", \"v%lu\", \"v%lu\", %{float}, %{float}, %{float}, %{float});\n", edge->id, org->id, dst->id, &point_org->x, &point_org->y, &point_dst->x, &point_dst->y);
    tb_assert_and_check_return(size > 0);

    // write line
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)line, size);
}
tb_void_t gb_tessellator_profiler_add_inter(gb_mesh_vertex_ref_t inter)
{
    // check
    tb_assert_and_check_return(g_profiler && inter);

    // the point
    gb_point_ref_t point = gb_tessellator_vertex_point(inter);

    // make line
    tb_char_t line[256] = {0};
    tb_long_t size = tb_snprintf(line, sizeof(line), "                add_inter(canvas, \"v%lu\", %{float}, %{float});\n", inter->id, &point->x, &point->y);
    tb_assert_and_check_return(size > 0);

    // write line
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)line, size);
}
tb_void_t gb_tessellator_profiler_finish_region(gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert_and_check_return(g_profiler && region);

    // the region is inside?
    tb_check_return(region->inside);

    // get the edge of this region
    gb_mesh_edge_ref_t edge = region->edge;
    tb_assert_and_check_return(edge);

    // the vertices
    gb_mesh_vertex_ref_t org = gb_mesh_edge_org(edge);
    gb_mesh_vertex_ref_t dst = gb_mesh_edge_dst(edge);

    // the points
    gb_point_ref_t point_org = gb_tessellator_vertex_point(org);
    gb_point_ref_t point_dst = gb_tessellator_vertex_point(dst);

    // make line
    tb_char_t line[256] = {0};
    tb_long_t size = tb_snprintf(line, sizeof(line), "                finish_region(canvas, %{float}, %{float}, %{float}, %{float});\n", &point_org->x, &point_org->y, &point_dst->x, &point_dst->y);
    tb_assert_and_check_return(size > 0);

    // write line
    tb_stream_bwrit(g_profiler, (tb_byte_t const*)line, size);
}

