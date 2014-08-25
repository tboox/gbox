/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_clos(gb_window_ref_t window, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window);

    // trace
    tb_trace_i("clos");
}
tb_void_t gb_demo_draw(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_BLACK);
}
tb_void_t gb_demo_resize(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // trace
    tb_trace_i("resize: %lux%lu", gb_window_width(window), gb_window_height(window));

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_BLACK);
}
tb_void_t gb_demo_touch_begin(gb_window_ref_t window, gb_point_t const* points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count);

    // trace
    tb_trace_i("touch: begin: %ld, %ld", gb_float_to_long(points[0].x), gb_float_to_long(points[0].y));
}
tb_void_t gb_demo_touch_move(gb_window_ref_t window, gb_point_t const* points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count);

    // trace
    tb_trace_i("touch: move: %ld, %ld", gb_float_to_long(points[0].x), gb_float_to_long(points[0].y));
}
tb_void_t gb_demo_touch_end(gb_window_ref_t window, gb_point_t const* points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count);

    // trace
    tb_trace_i("touch: end: %ld, %ld", gb_float_to_long(points[0].x), gb_float_to_long(points[0].y));
}
tb_void_t gb_demo_touch_cancel(gb_window_ref_t window, gb_point_t const* points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count);

    // trace
    tb_trace_i("touch: cancel: %ld, %ld", gb_float_to_long(points[0].x), gb_float_to_long(points[0].y));
}
