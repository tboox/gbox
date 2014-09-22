/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "circle"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "circle.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_circle_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_circle_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_circle_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // circle
    gb_circle_t circle = gb_circle_imake(0, 0, 200);

    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_circle(canvas, &circle);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_circle(canvas, &circle);
}
tb_void_t gb_demo_circle_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

