/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "cubic"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "cubic.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the path
static gb_path_ref_t    g_path = tb_null;

// the ctrl x0 and y0
static tb_long_t        g_ctrl_x0 = -100 >> 4;
static tb_long_t        g_ctrl_y0 = 200 >> 1;

// the ctrl x1 and y1
static tb_long_t        g_ctrl_x1 = 100 >> 4;
static tb_long_t        g_ctrl_y1 = 200 >> 1;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_cubic_init(gb_window_ref_t window)
{
    // init path
    g_path = gb_path_init();
}
tb_void_t gb_demo_cubic_exit(gb_window_ref_t window)
{
    // exit path
    if (g_path) gb_path_exit(g_path);
    g_path = tb_null;
}
tb_void_t gb_demo_cubic_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // make path
    gb_path_clear(g_path);
    gb_path_move2i_to(g_path, -200, 0);
    gb_path_cubic2i_to(g_path, g_ctrl_x0, g_ctrl_y0, g_ctrl_x1, g_ctrl_y1, 200, 0);
    gb_path_move2i_to(g_path, -100, 0);
    gb_path_cubic2i_to(g_path, g_ctrl_x0 + (g_ctrl_x0 >> 2), g_ctrl_y0 + (g_ctrl_y0 >> 2), g_ctrl_x1 + (g_ctrl_x1 >> 2), g_ctrl_y1 + (g_ctrl_y1 >> 2), 100, 0);
    gb_path_move2i_to(g_path, -50, 0);
    gb_path_cubic2i_to(g_path, g_ctrl_x0 + (g_ctrl_x0 >> 1), g_ctrl_y0 + (g_ctrl_y0 >> 1), g_ctrl_x1 + (g_ctrl_x1 >> 1), g_ctrl_y1 + (g_ctrl_y1 >> 1), 50, 0);
    gb_path_move2i_to(g_path, -10, 0);
    gb_path_cubic2i_to(g_path, g_ctrl_x0 << 1, g_ctrl_y0 << 1, g_ctrl_x1 << 1, g_ctrl_y1 << 1, 10, 0);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_path(canvas, g_path);
}
tb_void_t gb_demo_cubic_event(gb_window_ref_t window, gb_event_ref_t event)
{
    if (    event->type == GB_EVENT_TYPE_MOUSE
        &&  event->u.mouse.code == GB_MOUSE_MOVE
       )//        &&  event->u.mouse.button == GB_MOUSE_BUTTON_RIGHT)
    {
        // the x0 and y0
        tb_long_t x0 = gb_window_width(window) >> 1;
        tb_long_t y0 = gb_window_height(window) >> 1;

        // the cursor x and y
        tb_long_t cx = gb_float_to_long(event->u.mouse.cursor.x);
        tb_long_t cy = gb_float_to_long(event->u.mouse.cursor.y);

        // the ctrl x0 and y0
        if (cx < x0)
        {
            g_ctrl_x0 = (cx - x0) >> 1;
            g_ctrl_y0 = (y0 - cy) >> 1;
        }

        // the ctrl x1 and y1
        if (cx > x0)
        {
            g_ctrl_x1 = (cx - x0) >> 1;
            g_ctrl_y1 = (y0 - cy) >> 1;
        }
    }
}

