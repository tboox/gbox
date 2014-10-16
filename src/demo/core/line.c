/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "line"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "line.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the x and y
static tb_long_t g_x = 100;
static tb_long_t g_y = 100;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_line_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_line_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_line_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_line2i(canvas, -g_x, -g_y, g_x, g_y);
}
tb_void_t gb_demo_line_event(gb_window_ref_t window, gb_event_ref_t event)
{
    if (    event->type == GB_EVENT_TYPE_MOUSE
        &&  event->u.mouse.code == GB_MOUSE_MOVE
        &&  event->u.mouse.button == GB_MOUSE_BUTTON_LEFT)
    {
        // the x0 and y0
        tb_long_t x0 = gb_window_width(window) >> 1;
        tb_long_t y0 = gb_window_height(window) >> 1;

        // the cursor x and y
        tb_long_t cx = gb_float_to_long(event->u.mouse.cursor.x);
        tb_long_t cy = gb_float_to_long(event->u.mouse.cursor.y);

        // the x and y
        g_x = cx - x0;
        g_y = y0 - cy;
    }
}

