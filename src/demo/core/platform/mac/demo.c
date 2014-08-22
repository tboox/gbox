/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "gbox/gbox.h"
#include <stdio.h>
#include <stdlib.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the width
#define GB_DEMO_WIDTH           (640)

// the height
#define GB_DEMO_HEIGHT          (480)

/* //////////////////////////////////////////////////////////////////////////////////////
 * demo
 */
static tb_void_t gb_demo_draw_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv)
{
    tb_trace_i("draw");
}
static tb_void_t gb_demo_clos_func(gb_window_ref_t window, tb_cpointer_t priv)
{
    tb_trace_i("clos");
}

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
    gb_window_info_t info = {0};
    info.title      = "demo";
    info.framerate  = gb_long_to_float(60);
    info.draw       = gb_demo_draw_func;
    info.clos       = gb_demo_clos_func;

    // init window
    gb_window_ref_t window = gb_window_init_glfw(&info, GB_DEMO_WIDTH, GB_DEMO_HEIGHT);
    if (window)
    {
        // wait 
        getchar();

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
