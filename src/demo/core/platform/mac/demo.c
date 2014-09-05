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

// main
#ifdef GB_CONFIG_APP_WINDOW_SDL
#   undef main
#   define main                 SDL_main
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // init tbox
    if (!tb_init(tb_null, tb_null, 0)) return 0;

    // init gbox
    if (!gb_init()) return 0;

    // init window info
    gb_window_info_t info   = {0};
    info.title              = "demo";
    info.framerate          = 60;
    info.init               = gb_demo_init;
    info.exit               = gb_demo_exit;
    info.draw               = gb_demo_draw;
    info.resize             = gb_demo_resize;
    info.event              = gb_demo_event;

    // init window
#ifdef GB_CONFIG_APP_WINDOW_SDL
    gb_window_ref_t window = gb_window_init_sdl(&info, GB_DEMO_WIDTH, GB_DEMO_HEIGHT, GB_WINDOW_FLAG_NONE);
#else
    gb_window_ref_t window = gb_window_init(&info, GB_DEMO_HEIGHT, GB_DEMO_HEIGHT, GB_WINDOW_FLAG_NONE);
#endif
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
