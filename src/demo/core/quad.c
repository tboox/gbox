/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "quad"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "quad.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the path
static gb_path_ref_t    g_path = tb_null;

// the ctrl x and y
static tb_long_t        g_ctrl_x = 0;
static tb_long_t        g_ctrl_y = 200;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_quad_init(gb_window_ref_t window)
{
    // init path
    g_path = gb_path_init();
}
tb_void_t gb_demo_quad_exit(gb_window_ref_t window)
{
    // exit path
    if (g_path) gb_path_exit(g_path);
    g_path = tb_null;
}
tb_void_t gb_demo_quad_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // make path
    gb_path_clear(g_path);
    gb_path_move2i_to(g_path, -200, 0);
    gb_path_quad2i_to(g_path, g_ctrl_x, g_ctrl_y, 200, 0);
    gb_path_move2i_to(g_path, -100, 0);
    gb_path_quad2i_to(g_path, g_ctrl_x + (g_ctrl_x >> 2), g_ctrl_y + (g_ctrl_y >> 2), 100, 0);
    gb_path_move2i_to(g_path, -50, 0);
    gb_path_quad2i_to(g_path, g_ctrl_x + (g_ctrl_x >> 1), g_ctrl_y + (g_ctrl_y >> 1), 50, 0);
    gb_path_move2i_to(g_path, -10, 0);
    gb_path_quad2i_to(g_path, g_ctrl_x << 1, g_ctrl_y << 1, 10, 0);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_path(canvas, g_path);
}
tb_void_t gb_demo_quad_event(gb_window_ref_t window, gb_event_ref_t event)
{
    if (    event->type == GB_EVENT_TYPE_MOUSE
        &&  event->u.mouse.code == GB_MOUSE_MOVE
       )//        &&  event->u.mouse.button == GB_MOUSE_BUTTON_RIGHT)
    {
        // the x0 and y0
        tb_long_t x0 = gb_window_width(window) >> 1;
        tb_long_t y0 = gb_window_height(window) >> 1;

        // the ctrl x and y
        g_ctrl_x = gb_float_to_long(event->u.mouse.cursor.x) - x0;
        g_ctrl_y = y0 - gb_float_to_long(event->u.mouse.cursor.y);
    }
}

