/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "round_rect"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "round_rect.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_round_rect_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_round_rect_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_round_rect_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // the bounds
    gb_rect_t bounds = gb_rect_imake(-100, -100, 200, 200);

    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_round_rect2i(canvas, &bounds, 50, 50);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_round_rect2i(canvas, &bounds, 50, 50);
}
tb_void_t gb_demo_round_rect_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

