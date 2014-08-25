/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_clos_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // trace
    tb_trace_i("clos");

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_BLACK);
}
tb_void_t gb_demo_draw_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_BLACK);
}
tb_void_t gb_demo_resize_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // trace
    tb_trace_i("resize: %lux%lu", gb_window_width(window), gb_window_height(window));

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_BLACK);
}

