/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "demo"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"
#include "arc.h"
#include "rect.h"
#include "path.h"
#include "quad.h"
#include "cubic.h"
#include "line.h"
#include "lines.h"
#include "tiger.h"
#include "point.h"
#include "points.h"
#include "circle.h"
#include "ellipse.h"
#include "triangle.h"
#include "round_rect.h"
 
/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the demo entry type
typedef struct __gb_demo_entry_t
{
    // the init func
    tb_void_t           (*init)(gb_window_ref_t window);

    // the exit func
    tb_void_t           (*exit)(gb_window_ref_t window);

    // the draw func
    tb_void_t           (*draw)(gb_window_ref_t window, gb_canvas_ref_t canvas);
     
    // the event func
    tb_void_t           (*event)(gb_window_ref_t window, gb_event_ref_t event);

}gb_demo_entry_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the demo index
static tb_size_t        g_index = 0;

// the demo entries
static gb_demo_entry_t  g_entries[] =
{
    {gb_demo_rect_init,         gb_demo_rect_exit,          gb_demo_rect_draw,          gb_demo_rect_event          }
,   {gb_demo_round_rect_init,   gb_demo_round_rect_exit,    gb_demo_round_rect_draw,    gb_demo_round_rect_event    }
,   {gb_demo_path_init,         gb_demo_path_exit,          gb_demo_path_draw,          gb_demo_path_event          }
,   {gb_demo_line_init,         gb_demo_line_exit,          gb_demo_line_draw,          gb_demo_line_event          }
,   {gb_demo_quad_init,         gb_demo_quad_exit,          gb_demo_quad_draw,          gb_demo_quad_event          }
,   {gb_demo_cubic_init,        gb_demo_cubic_exit,         gb_demo_cubic_draw,         gb_demo_cubic_event         }
,   {gb_demo_lines_init,        gb_demo_lines_exit,         gb_demo_lines_draw,         gb_demo_lines_event         }
,   {gb_demo_point_init,        gb_demo_point_exit,         gb_demo_point_draw,         gb_demo_point_event         }
,   {gb_demo_points_init,       gb_demo_points_exit,        gb_demo_points_draw,        gb_demo_points_event        }
,   {gb_demo_circle_init,       gb_demo_circle_exit,        gb_demo_circle_draw,        gb_demo_circle_event        }
,   {gb_demo_ellipse_init,      gb_demo_ellipse_exit,       gb_demo_ellipse_draw,       gb_demo_ellipse_event       }
,   {gb_demo_triangle_init,     gb_demo_triangle_exit,      gb_demo_triangle_draw,      gb_demo_triangle_event      }
,   {gb_demo_arc_init,          gb_demo_arc_exit,           gb_demo_arc_draw,           gb_demo_arc_event           }
,   {gb_demo_tiger_init,        gb_demo_tiger_exit,         gb_demo_tiger_draw,         gb_demo_tiger_event         }
};

// the matrix
static gb_matrix_t      g_matrix;

// the quality
static tb_size_t        g_quality = GB_QUALITY_LOW;

// the cap
static tb_size_t        g_cap = GB_PAINT_STROKE_CAP_BUTT;

// the join
static tb_size_t        g_join = GB_PAINT_STROKE_JOIN_MITER;

// the width
static gb_float_t       g_width = GB_ONE;

// the alpha
static tb_byte_t        g_alpha = 255;

// transform it?
static tb_bool_t        g_transform = tb_false;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_demo_info(tb_bool_t killed, tb_cpointer_t priv)
{
    // check
    gb_window_ref_t window = (gb_window_ref_t)priv;
    tb_assert_and_check_return(window);

    // the framerate 
    gb_float_t framerate = gb_window_framerate(window);

    // trace
    tb_trace_i("framerate: %{float}", &framerate);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_demo_init(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert(window);

    // trace
    tb_trace_d("init");

    // init quality
    gb_quality_set(g_quality);

    // the x0 and y0
    gb_float_t x0 = gb_long_to_float(gb_window_width(window) >> 1);
    gb_float_t y0 = gb_long_to_float(gb_window_height(window) >> 1);

	// init matrix
	gb_matrix_init_translate(&g_matrix, x0, y0);

    // init entries
    tb_size_t index = 0;
    tb_size_t count = tb_arrayn(g_entries);
    for (index = 0; index < count; index++)
    {
        // done init
        if (g_entries[index].init) g_entries[index].init(window);
    }

    // ok
    return tb_true;
}
tb_void_t gb_demo_exit(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert(window);

    // trace
    tb_trace_d("exit");

    // clos entries
    tb_size_t index = 0;
    tb_size_t count = tb_arrayn(g_entries);
    for (index = 0; index < count; index++)
    {
        // done clos
        if (g_entries[index].exit) g_entries[index].exit(window);
    }
}
tb_void_t gb_demo_draw(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert(window && canvas);
    tb_assert(g_index < tb_arrayn(g_entries));

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_DEFAULT);

    // the entry
    gb_demo_entry_t const* entry = &g_entries[g_index];
    tb_assert(entry->draw);

    // enter matrix
    gb_matrix_copy(gb_canvas_save_matrix(canvas), &g_matrix);

    // apply cap
    gb_canvas_stroke_cap_set(canvas, g_cap);

    // apply join
    gb_canvas_stroke_join_set(canvas, g_join);

    // apply width
    gb_canvas_stroke_width_set(canvas, g_width);

    // apply alpha
    gb_canvas_alpha_set(canvas, g_alpha);

    // done draw
    entry->draw(window, canvas);

    // leave matrix
    gb_canvas_load_matrix(canvas);
}
tb_void_t gb_demo_resize(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert(window && canvas);

    // trace
    tb_trace_d("resize: %lux%lu", gb_window_width(window), gb_window_height(window));

    // the x0 and y0
    gb_float_t x0 = gb_long_to_float(gb_window_width(window) >> 1);
    gb_float_t y0 = gb_long_to_float(gb_window_height(window) >> 1);

	// update matrix
	gb_matrix_init_translate(&g_matrix, x0, y0);	
}
tb_void_t gb_demo_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv)
{
    // check
    tb_assert(window && event);
    tb_assert(g_index < tb_arrayn(g_entries));

    // keyboard
    if (event->type == GB_EVENT_TYPE_KEYBOARD && event->u.keyboard.pressed)
    {
        switch (event->u.keyboard.code)
        {
        case GB_KEY_BACK:
            gb_window_fullscreen(window, tb_false);
            break;
        case GB_KEY_LEFT:
            g_index = (g_index + tb_arrayn(g_entries) - 1) % tb_arrayn(g_entries);
            break;
        case GB_KEY_RIGHT:
            g_index = (g_index + 1) % tb_arrayn(g_entries);
            break;
        case GB_KEY_UP:
            g_width += GB_ONE;
            break;
        case GB_KEY_DOWN:
            if (g_width >= GB_ONE) g_width -= GB_ONE;
            break;
        case 'f':
            gb_window_fullscreen(window, tb_true);
            break;
        case 'q':
            g_quality = (g_quality + 1) % 3;
            gb_quality_set(g_quality);
            break;
        case 'c':
            g_cap = (g_cap + 1) % 3;
            break;
        case 'j':
            g_join = (g_join + 1) % 3;
            break;
        case 'a':
            g_alpha -= 15;
            break;
        case 't':
            g_transform = !g_transform;
            break;
        case 'i':
            tb_timer_task_post(gb_window_timer(window), 1000, tb_true, gb_demo_info, (tb_cpointer_t)window);
            break;
        default:
            break;
        }
    }
    // mouse
    else if (event->type == GB_EVENT_TYPE_MOUSE)
    {
        // move?
        if (event->u.mouse.code == GB_MOUSE_MOVE)
        {
            // the cursor coordinates
            gb_float_t x = event->u.mouse.cursor.x;
            gb_float_t y = event->u.mouse.cursor.y;

            // transform it
            if (g_transform)
            {
                // the dw and dh
                gb_float_t dw = gb_long_to_float(gb_window_width(window));
                gb_float_t dh = gb_long_to_float(gb_window_height(window));

                // the x0 and y0
                gb_float_t x0 = gb_half(dw);
                gb_float_t y0 = gb_half(dh);

                // the dx and dy
                gb_float_t dx = x > x0? (x - x0) : (x0 - x);
                gb_float_t dy = y > y0? (y - y0) : (y0 - y);
                dx = gb_lsh(dx, 1);
                dy = gb_lsh(dy, 1);

                // the an
                gb_float_t an = 0;
                if (y == y0) an = 0;
                else if (x == x0) an = gb_long_to_float(90);
                else an = gb_div(gb_atan(gb_div(dy, dx)) * 180, GB_PI);
                if (y < y0 && x < x0) an = gb_long_to_float(180) - an;
                if (y > y0 && x < x0) an += gb_long_to_float(180);
                if (y > y0 && x > x0) an = gb_long_to_float(360) - an;
                an = -an;

                // scale dx and dy
                dx = gb_lsh(dx, 2);
                dy = gb_lsh(dy, 2);

                // update matrix
                gb_matrix_init_translate(&g_matrix, x0, y0);
                gb_matrix_scale(&g_matrix, gb_div(dx, dw), gb_div(dy, dh));
                gb_matrix_rotate(&g_matrix, an);
            }
        }
    }

#if defined(__gb_debug__) && TB_TRACE_MODULE_DEBUG
    // dump event
    gb_event_dump(event);
#endif

    // the entry
    gb_demo_entry_t const* entry = &g_entries[g_index];
    tb_assert(entry->event);

    // done event
    entry->event(window, event);
}

