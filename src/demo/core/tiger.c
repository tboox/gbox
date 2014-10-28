/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "tiger"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "tiger.h"
#include "tiger.g"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
// the tiger entry type
typedef struct __gb_demo_tiger_entry_t
{
    // is fill?
    tb_size_t           is_fill:    1;

    // is stroke?
    tb_size_t           is_stroke:  1;

    // the fill color
    gb_color_t          fill_color;

    // the stroke color
    gb_color_t          stroke_color;

    // the stroke width
    gb_float_t          stroke_width;

    // the path
    gb_path_ref_t       path;

}gb_demo_tiger_entry_t, *gb_demo_tiger_entry_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the tiger entries
static gb_demo_tiger_entry_ref_t    g_tiger_entries = tb_null;

// the tiger entries count
static tb_size_t                    g_tiger_entries_count = 0;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tb_inline__ tb_char_t const* gb_demo_tiger_entry_skip_separator(tb_char_t const* p)
{
	while (*p && (tb_isspace(*p) || *p == ',')) p++;
	return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_float(tb_char_t const* p, gb_float_t* value)
{
    // skip space
    while (*p && tb_isspace(*p)) p++;

    // has sign?
    tb_long_t sign = 0;
    if (*p == '-') 
    {
        sign = 1;
        p++;
    }

    // skip '0'
    while (*p == '0') p++;

    // compute double: lhs.rhs
    tb_long_t   dec = 0;
    tb_uint32_t lhs = 0;
    gb_float_t  rhs = 0.;
    tb_long_t   zeros = 0;
    tb_int8_t   decimals[256];
    tb_int8_t*  d = decimals;
    tb_int8_t*  e = decimals + 256;
    while (*p)
    {
        tb_char_t ch = *p;

        // is the part of decimal?
        if (ch == '.')
        {
            if (!dec) 
            {
                dec = 1;
                p++;
                continue ;
            }
            else break;
        }

        // parse integer and decimal
        if (tb_isdigit10(ch))
        {
            // save decimals
            if (dec) 
            {
                if (d < e)
                {
                    if (ch != '0')
                    {
                        // fill '0'
                        while (zeros--) *d++ = 0;
                        zeros = 0;

                        // save decimal
                        *d++ = ch - '0';
                    }
                    else zeros++;
                }
            }
            else lhs = lhs * 10 + (ch - '0');
        }
        else break;
    
        // next
        p++;
    }

    // check
    tb_assert(d <= decimals + 256);

    // compute decimal
    while (d-- > decimals) rhs = gb_idiv(rhs + gb_long_to_float(*d), 10);

    // done 
    *value = (sign? -(gb_long_to_float(lhs) + rhs) : (gb_long_to_float(lhs) + rhs));

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_style_fill(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p)
{
    // seek to the color
    while (*p && *p != '#') p++;
    p++;

    // get pixel: argb
    gb_pixel_t pixel = tb_s16tou32(p);

    // skip pixel
    tb_size_t n = 0;
    for (; tb_isdigit16(*p); p++, n++) ;

    // only three digits? expand it. e.g. #123 => #112233
    if (n == 3) pixel = (((pixel >> 8) & 0x0f) << 20) | (((pixel >> 8) & 0x0f) << 16) | (((pixel >> 4) & 0x0f) << 12) | (((pixel >> 4) & 0x0f) << 8) | ((pixel & 0x0f) << 4) | (pixel & 0x0f);

    // no alpha? opaque it
    if (!(pixel & 0xff000000)) pixel |= 0xff000000;

    // init fill color
    entry->fill_color   = gb_pixel_color(pixel);
    entry->is_fill      = 1;

    // trace
    tb_trace_d("fill: %{color}", &entry->fill_color);

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_style_stroke(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p)
{
    // seek to the color
    while (*p && *p != '#') p++;
    p++;

    // get pixel: argb
    gb_pixel_t pixel = tb_s16tou32(p);

    // skip pixel
    tb_size_t n = 0;
    for (; tb_isdigit16(*p); p++, n++) ;

    // only three digits? expand it. e.g. #123 => #112233
    if (n == 3) pixel = (((pixel >> 8) & 0x0f) << 20) | (((pixel >> 8) & 0x0f) << 16) | (((pixel >> 4) & 0x0f) << 12) | (((pixel >> 4) & 0x0f) << 8) | ((pixel & 0x0f) << 4) | (pixel & 0x0f);

    // no alpha? opaque it
    if (!(pixel & 0xff000000)) pixel |= 0xff000000;

    // init stroke color
    entry->stroke_color     = gb_pixel_color(pixel);
    entry->is_stroke        = 1;

    // trace
    tb_trace_d("stroke: %{color}", &entry->stroke_color);

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_style_stroke_width(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p)
{
    // seek to the digits
    while (*p && !tb_isdigit(*p)) p++;

    // init the stroke width
    p = gb_demo_tiger_entry_init_float(p, &entry->stroke_width);

    // trace
    tb_trace_d("stroke_width: %{float}", &entry->stroke_width);

    // ok
    return p;
}
static tb_void_t gb_demo_tiger_entry_init_style(gb_demo_tiger_entry_ref_t entry, tb_char_t const* style)
{
    // check
    tb_assert_abort(entry && style);

    // done
    tb_char_t const* p = style;
    while (*p)
    {
        // done
        if (!tb_strnicmp(p, "fill", 4))
            p = gb_demo_tiger_entry_init_style_fill(entry, p + 4);
        else if (!tb_strnicmp(p, "stroke-width", 12))
            p = gb_demo_tiger_entry_init_style_stroke_width(entry, p + 12);
        else if (!tb_strnicmp(p, "stroke", 6))
            p = gb_demo_tiger_entry_init_style_stroke(entry, p + 6);
        else p++;
    }
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_xoy(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p, tb_char_t mode)
{
    // xoy
    gb_float_t xoy = 0; p = gb_demo_tiger_entry_init_float(p, &xoy); p = gb_demo_tiger_entry_skip_separator(p);

    // trace
    tb_trace_d("path: d: %c: %{float}", mode, &(xoy));

    // done path
    if (entry->path)
    {
        // last point
        gb_point_t pt = {0};
        gb_path_last(entry->path, &pt);

        // done
        switch (mode)
        {
            case 'H':
                gb_path_line2_to(entry->path, xoy, pt.y);
                break;
            case 'h':
                gb_path_line2_to(entry->path, pt.x + xoy, pt.y);
                break;
            case 'V':
                gb_path_line2_to(entry->path, pt.x, xoy);
                break;
            case 'v':
                gb_path_line2_to(entry->path, pt.x, pt.y + xoy);
                break;
            default:
                tb_trace_noimpl();
                break;
        }
    }

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_xy1(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p, tb_char_t mode)
{
    // x1
    gb_float_t x1 = 0; p = gb_demo_tiger_entry_init_float(p, &x1); p = gb_demo_tiger_entry_skip_separator(p);

    // y1
    gb_float_t y1 = 0; p = gb_demo_tiger_entry_init_float(p, &y1); p = gb_demo_tiger_entry_skip_separator(p);

    // trace
    tb_trace_d("path: d: %c: %{float}, %{float}", mode, &(x1), &(y1));

    // init path
    if (!entry->path) entry->path = gb_path_init();

    // done path
    if (entry->path)
    {
        // last point
        gb_point_t pt = {0};
        gb_path_last(entry->path, &pt);

        // done
        switch (mode)
        {
            case 'M':
                gb_path_move2_to(entry->path, x1, y1);
                break;
            case 'm':
                gb_path_move2_to(entry->path, pt.x + x1, pt.y + y1);
                break;
            case 'L':
                gb_path_line2_to(entry->path, x1, y1);
                break;
            case 'l':
                gb_path_line2_to(entry->path, pt.x + x1, pt.y + y1);
                break;
            default:
                tb_trace_noimpl();
                break;
        }
    }

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_xy2(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p, tb_char_t mode)
{
    // x1
    gb_float_t x1 = 0; p = gb_demo_tiger_entry_init_float(p, &x1); p = gb_demo_tiger_entry_skip_separator(p);

    // y1
    gb_float_t y1 = 0; p = gb_demo_tiger_entry_init_float(p, &y1); p = gb_demo_tiger_entry_skip_separator(p);

    // x2
    gb_float_t x2 = 0; p = gb_demo_tiger_entry_init_float(p, &x2); p = gb_demo_tiger_entry_skip_separator(p);

    // y2
    gb_float_t y2 = 0; p = gb_demo_tiger_entry_init_float(p, &y2); p = gb_demo_tiger_entry_skip_separator(p);

    // trace
    tb_trace_d("path: d: %c: %{float}, %{float}, %{float}, %{float}", mode, &(x1), &(y1), &(x2), &(y2));

    // init path
    if (!entry->path) entry->path = gb_path_init();

    // done path
    if (entry->path)
    {
        // done
        switch (mode)
        {
            case 'Q':
                gb_path_quad2_to(entry->path, x1, y1, x2, y2);
                break;
            case 'q':
                {
                    gb_point_t pt = {0};
                    gb_path_last(entry->path, &pt);
                    gb_path_quad2_to(entry->path, pt.x + x1, pt.y + y1, pt.x + x2, pt.y + y2);
                }
                break;
            default:
                tb_trace_noimpl();
                break;
        }
    }

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_xy3(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p, tb_char_t mode)
{
    // x1
    gb_float_t x1 = 0; p = gb_demo_tiger_entry_init_float(p, &x1); p = gb_demo_tiger_entry_skip_separator(p);

    // y1
    gb_float_t y1 = 0; p = gb_demo_tiger_entry_init_float(p, &y1); p = gb_demo_tiger_entry_skip_separator(p);

    // x2
    gb_float_t x2 = 0; p = gb_demo_tiger_entry_init_float(p, &x2); p = gb_demo_tiger_entry_skip_separator(p);

    // y2
    gb_float_t y2 = 0; p = gb_demo_tiger_entry_init_float(p, &y2); p = gb_demo_tiger_entry_skip_separator(p);

    // x3
    gb_float_t x3 = 0; p = gb_demo_tiger_entry_init_float(p, &x3); p = gb_demo_tiger_entry_skip_separator(p);

    // y3
    gb_float_t y3 = 0; p = gb_demo_tiger_entry_init_float(p, &y3); p = gb_demo_tiger_entry_skip_separator(p);

    // trace
    tb_trace_d("path: d: %c: %{float}, %{float}, %{float}, %{float}, %{float}, %{float}", mode, &(x1), &(y1), &(x2), &(y2), &(x3), &(y3));

    // init path
    if (!entry->path) entry->path = gb_path_init();

    // done path
    if (entry->path)
    {
        // done
        switch (mode)
        {
            case 'C':
                gb_path_cubic2_to(entry->path, x1, y1, x2, y2, x3, y3);
                break;
            case 'c':
                {
                    gb_point_t pt = {0};
                    gb_path_last(entry->path, &pt);
                    gb_path_cubic2_to(entry->path, pt.x + x1, pt.y + y1, pt.x + x2, pt.y + y2, pt.x + x3, pt.y + y3);
                }
                break;
            default:
                tb_trace_noimpl();
                break;
        }
    }

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_a(gb_demo_tiger_entry_ref_t entry, tb_char_t const* p, tb_char_t mode)
{
    // rx
    gb_float_t rx = 0; p = gb_demo_tiger_entry_init_float(p, &rx); p = gb_demo_tiger_entry_skip_separator(p);

    // ry
    gb_float_t ry = 0; p = gb_demo_tiger_entry_init_float(p, &ry); p = gb_demo_tiger_entry_skip_separator(p);

    // x-axis-rotation
    gb_float_t xr = 0; p = gb_demo_tiger_entry_init_float(p, &xr); p = gb_demo_tiger_entry_skip_separator(p);

    // large-arc-flag
    gb_float_t af = 0; p = gb_demo_tiger_entry_init_float(p, &af); p = gb_demo_tiger_entry_skip_separator(p);

    // sweep-flag
    gb_float_t sf = 0; p = gb_demo_tiger_entry_init_float(p, &sf); p = gb_demo_tiger_entry_skip_separator(p);

    // x
    gb_float_t x = 0; p = gb_demo_tiger_entry_init_float(p, &x); p = gb_demo_tiger_entry_skip_separator(p);

    // y
    gb_float_t y = 0; p = gb_demo_tiger_entry_init_float(p, &y); p = gb_demo_tiger_entry_skip_separator(p);

    // trace
    tb_trace_d("path: a: %c: %{float}, %{float}, %{float}, %{float}, %{float}, %{float}, %{float}", mode, &(rx), &(ry), &(xr), &(af), &(sf), &(x), &(y));

    // init path
    if (!entry->path) entry->path = gb_path_init();

    // done path
    if (entry->path)
    {
        // last point
        gb_point_t pt = {0};
        gb_path_last(entry->path, &pt);

        // absolute x & y
        if (mode == 'a') 
        {
            x += pt.x;
            y += pt.y;
        }

        // arc-to
//        gb_path_arc2_to(entry->path, x0, y0, rx, ry, ab, an);

        // noimpl
        tb_trace_noimpl();
    }

    // ok
    return p;
}
static tb_char_t const* gb_demo_tiger_entry_init_path_d_z(gb_demo_tiger_entry_ref_t entry, tb_char_t const* data, tb_char_t mode)
{
    // trace
    tb_trace_d("path: d: z");

    // close path
    if (entry->path) gb_path_clos(entry->path);

    // ok
    return data;
}
static tb_void_t gb_demo_tiger_entry_init_path(gb_demo_tiger_entry_ref_t entry, tb_char_t const* path)
{
    // check
    tb_assert_abort(entry && path);

    // trace
    tb_trace_d("path: d");

    // done
    tb_char_t const*    p = path;
    tb_char_t           l = '\0';
    tb_char_t           m = *p++;
    while (m)
    {
        tb_size_t d = 0;
        switch (m)
        {
        case 'M':
        case 'm':
        case 'L':
        case 'l':
        case 'T':
        case 't':
            p = gb_demo_tiger_entry_init_path_d_xy1(entry, p, m); l = m;
            break;
        case 'H':
        case 'h':
        case 'V':
        case 'v':
            p = gb_demo_tiger_entry_init_path_d_xoy(entry, p, m); l = m;
            break;
        case 'S':
        case 's':
        case 'Q':
        case 'q':
            p = gb_demo_tiger_entry_init_path_d_xy2(entry, p, m); l = m;
            break;
        case 'C':
        case 'c':
            p = gb_demo_tiger_entry_init_path_d_xy3(entry, p, m); l = m;
            break;
        case 'A':
        case 'a':
            p = gb_demo_tiger_entry_init_path_d_a(entry, p, m); l = m;
            break;
        case 'Z':
        case 'z':
            p = gb_demo_tiger_entry_init_path_d_z(entry, p, m); l = m;
            break;
        default:
            d = 1;
            break;
        }

        // no mode? use the last mode
        if (d && (tb_isdigit(m) || m == '.' || m == '-')) 
        {
            m = l;
            p--;
        }
        else m = *p++;
    }
}
static tb_void_t gb_demo_tiger_entry_init(gb_demo_tiger_entry_ref_t entry, tb_char_t const* style, tb_char_t const* path)
{
    // init style
    gb_demo_tiger_entry_init_style(entry, style);

    // init path
    gb_demo_tiger_entry_init_path(entry, path);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_tiger_init(gb_window_ref_t window)
{
    // check
    tb_assert_static(!(tb_arrayn(g_demo_tiger) & 0x1));

    // the width and height
    gb_float_t w = gb_long_to_float(gb_window_width(window));
    gb_float_t h = gb_long_to_float(gb_window_height(window));

    // init entries
    g_tiger_entries = tb_nalloc0_type(tb_arrayn(g_demo_tiger) >> 1, gb_demo_tiger_entry_t);
    tb_assert_abort(g_tiger_entries);

    // done
    tb_size_t index = 0;
    tb_size_t count = tb_arrayn(g_demo_tiger);
    for (index = 0; index < count; index += 2)
    {
        // the style and path
        tb_char_t const* style  = g_demo_tiger[index];
        tb_char_t const* path   = g_demo_tiger[index + 1];

        // make entry
        gb_demo_tiger_entry_ref_t entry = &g_tiger_entries[g_tiger_entries_count++];

        // init entry
        gb_demo_tiger_entry_init(entry, style, path);

        // apply matrix to the path
        if (entry->path) 
        {
            gb_matrix_t matrix;
            gb_matrix_init_translate(&matrix, -gb_long_to_float(320), -gb_long_to_float(320));
            gb_matrix_scale_lhs(&matrix, gb_idiv(w, 640), -gb_idiv(h, 640));
            gb_path_apply(entry->path, &matrix);
        }
    }
}
tb_void_t gb_demo_tiger_exit(gb_window_ref_t window)
{
    // exit entries
    if (g_tiger_entries)
    {
        // done
        tb_size_t i = 0;
        for (i = 0; i < g_tiger_entries_count; i++)
        {
            // exit path
            if (g_tiger_entries[i].path) gb_path_exit(g_tiger_entries[i].path);
            g_tiger_entries[i].path = tb_null;
        }

        // exit it
        tb_free(g_tiger_entries);
    }
}
tb_void_t gb_demo_tiger_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // done
    tb_size_t i = 0;
    for (i = 0; i < g_tiger_entries_count; i++)
    {
        // the entry
        gb_demo_tiger_entry_ref_t entry = &g_tiger_entries[i];

        // draw path
        if (entry->path)
        {
            // fill it
            if (entry->is_fill)
            {
                gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
                gb_canvas_color_set(canvas, entry->fill_color);
                gb_canvas_draw_path(canvas, entry->path);
            }

            // stroke it
            if (entry->is_stroke)
            {
                gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
                gb_canvas_color_set(canvas, entry->stroke_color);
                gb_canvas_stroke_width_set(canvas, entry->stroke_width);
                gb_canvas_draw_path(canvas, entry->path);
            }
        }
    }
}
tb_void_t gb_demo_tiger_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

