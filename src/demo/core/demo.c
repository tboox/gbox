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
    gb_canvas_draw_clear(canvas, GB_COLOR_DEFAULT);
}
tb_void_t gb_demo_resize(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && canvas);

    // trace
    tb_trace_i("resize: %lux%lu", gb_window_width(window), gb_window_height(window));
}
tb_void_t gb_demo_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && event);

#ifdef __tb_debug__
    // dump event
    gb_event_dump(event);
#endif
}

