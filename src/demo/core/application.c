/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"
#include "application.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_application_init(gb_application_ref_t application, gb_window_info_ref_t info)
{
    // check
    tb_assert_and_check_return_val(application && info, tb_false);

    // init window
    info->title         = "demo";
    info->framerate     = 60;
    info->width         = 640;
    info->height        = 640;
    info->init          = gb_demo_init;
    info->exit          = gb_demo_exit;
    info->draw          = gb_demo_draw;
    info->resize        = gb_demo_resize;
    info->event         = gb_demo_event;

    // ok
    return tb_true;
}

