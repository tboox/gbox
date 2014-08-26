/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the width
#define GB_DEMO_WIDTH           (640)

// the height
#define GB_DEMO_HEIGHT          (480)

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // init tbox
#if 0
    if (!tb_init(tb_null, (tb_byte_t*)malloc(300 * 1024 * 1024), 300 * 1024 * 1024)) return 0;
#else
    if (!tb_init(tb_null, tb_null, 0)) return 0;
#endif

    // init gbox
    if (!gb_init()) return 0;

    // init window info
    gb_window_info_t info   = {0};
    info.title              = "demo";
    info.framerate          = 60;
    info.clos               = gb_demo_clos;
    info.draw               = gb_demo_draw;
    info.resize             = gb_demo_resize;
    info.touch_begin        = gb_demo_touch_begin;
    info.touch_move         = gb_demo_touch_move;
    info.touch_end          = gb_demo_touch_end;
    info.touch_cancel       = gb_demo_touch_cancel;

    // init window
    gb_window_ref_t window = gb_window_init_glut(&info, GB_DEMO_WIDTH, GB_DEMO_HEIGHT, tb_false);
    if (window)
    {
        // loop window
        gb_window_loop(window);

        // exit window
        gb_window_exit(window);
    }

    // exit gbox
    gb_exit();

    // exit tbox
    tb_exit();

    // ok?
    return 0;
}
