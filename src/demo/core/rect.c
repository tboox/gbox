/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "rect"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "rect.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_rect_clos(gb_window_ref_t window)
{
}
tb_void_t gb_demo_rect_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // rect
    gb_rect_t rect = gb_rect_imake(-100, -100, 200, 200);

    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_rect(canvas, &rect);

    // stok
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STOK);
    gb_canvas_draw_rect(canvas, &rect);
}
tb_void_t gb_demo_rect_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

