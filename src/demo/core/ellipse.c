/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "ellipse"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "ellipse.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_ellipse_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_ellipse_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_ellipse_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // ellipse
    gb_ellipse_t ellipse = gb_ellipse_imake(0, 0, 200, 100);

    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_ellipse(canvas, &ellipse);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_ellipse(canvas, &ellipse);
}
tb_void_t gb_demo_ellipse_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

