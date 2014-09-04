/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "point"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "point.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_point_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_point_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_point_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // stok
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STOK);
    gb_canvas_draw_point2i(canvas, 0, 0);
}
tb_void_t gb_demo_point_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

