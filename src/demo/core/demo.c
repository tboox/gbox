/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "demo"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"
#include "rect.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the demo entry type
typedef struct __gb_demo_entry_t
{
    // the clos func
    tb_void_t           (*clos)(gb_window_ref_t window, tb_cpointer_t priv);

    // the draw func
    tb_void_t           (*draw)(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);
     
    // the event func
    tb_void_t           (*event)(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv);

}gb_demo_entry_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the demo index
static tb_size_t        g_index = 0;

// the demo entries
static gb_demo_entry_t  g_entries[] =
{
    {gb_demo_rect_clos, gb_demo_rect_draw, gb_demo_rect_event}
};

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_demo_info(tb_bool_t killed, tb_cpointer_t priv)
{
    // check
    gb_window_ref_t window = (gb_window_ref_t)priv;
    tb_assert_and_check_return(window);

    // trace
    tb_trace_i("framerate: %{float}", gb_window_framerate(window));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_clos(gb_window_ref_t window, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(window);

    // trace
    tb_trace_d("clos");

    // clos entries
    tb_size_t index = 0;
    tb_size_t count = tb_arrayn(g_entries);
    for (index = 0; index < count; index++)
    {
        // done clos
        if (g_entries[index].clos) g_entries[index].clos(window, priv);
    }
}
tb_void_t gb_demo_draw(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(window && canvas);
    tb_assert_abort(g_index < tb_arrayn(g_entries));

    // clear it
    gb_canvas_draw_clear(canvas, GB_COLOR_DEFAULT);

    // the entry
    gb_demo_entry_t const* entry = &g_entries[g_index];
    tb_assert_abort(entry->draw);

    // done draw
    entry->draw(window, canvas, priv);
}
tb_void_t gb_demo_resize(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(window && canvas);

    // trace
    tb_trace_d("resize: %lux%lu", gb_window_width(window), gb_window_height(window));
}
tb_void_t gb_demo_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv)
{
    // check
    tb_assert_abort(window && event);
    tb_assert_abort(g_index < tb_arrayn(g_entries));

    // keyboard
    if (event->type == GB_EVENT_TYPE_KEYBOARD && event->u.keyboard.pressed)
    {
        switch (event->u.keyboard.code)
        {
        case GB_KEY_BACK:
            gb_window_fullscreen(window, tb_false);
            break;
        case GB_KEY_LEFT:
            if (g_index) g_index--;
            break;
        case GB_KEY_RIGHT:
            if (g_index < tb_arrayn(g_entries)) g_index++;
            break;
        case 'f':
            gb_window_fullscreen(window, tb_true);
            break;
        case 'i':
            tb_timer_task_post(gb_window_timer(window), 1000, tb_true, gb_demo_info, (tb_cpointer_t)window);
            break;
        default:
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

    // the entry
    gb_demo_entry_t const* entry = &g_entries[g_index];
    tb_assert_abort(entry->event);

    // done event
    entry->event(window, event, priv);
}

