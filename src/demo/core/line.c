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
    // stok
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STOK);
    gb_canvas_draw_line2i(canvas, -100, -100, 100, 100);
}
tb_void_t gb_demo_line_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

