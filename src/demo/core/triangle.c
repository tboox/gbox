/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "triangle"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "triangle.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_triangle_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_triangle_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_triangle_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // triangle
    gb_triangle_t triangle = gb_triangle_imake(-100, 100, 0, -100, 100, 100);

    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_triangle(canvas, &triangle);

    // stok
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STOK);
    gb_canvas_draw_triangle(canvas, &triangle);
}
tb_void_t gb_demo_triangle_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

