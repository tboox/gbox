/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "demo"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_demo_info(tb_bool_t killed, tb_cpointer_t priv)
{
    // check
    gb_window_ref_t window = (gb_window_ref_t)priv;
    tb_assert_and_check_return(window);

    // trace
    tb_trace_i("framerate: " GB_FMT, gb_window_framerate(window));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_clos(gb_window_ref_t window, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window);

    // trace
    tb_trace_d("clos");
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
    tb_trace_d("resize: %lux%lu", gb_window_width(window), gb_window_height(window));
}
tb_void_t gb_demo_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv)
{
    // check
    tb_assert_and_check_return(window && event);

    // keyboard
    if (event->type == GB_EVENT_TYPE_KEYBOARD && event->u.keyboard.pressed)
    {
        switch (event->u.keyboard.code)
        {
        case GB_KEY_BACK:
            gb_window_fullscreen(window, tb_false);
            break;
        case 'f':
            gb_window_fullscreen(window, tb_true);
            break;
        case 'i':
            tb_timer_task_post(gb_window_timer(window), 1000, tb_true, gb_demo_info, (tb_cpointer_t)window);
            break;
        }
    }
    // mouse
    else if (event->type == GB_EVENT_TYPE_MOUSE)
    {
    }

#if defined(__gb_debug__) && TB_TRACE_MODULE_DEBUG
    // dump event
    gb_event_dump(event);
#endif
}

