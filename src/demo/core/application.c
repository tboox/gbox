/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "demo"
#define TB_TRACE_MODULE_DEBUG           (0)

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

    // trace
    tb_trace_d("init");

    // init window
    info->title         = TB_TRACE_MODULE_NAME;
    info->framerate     = 60;
    info->width         = 640;
    info->height        = 480;
    info->init          = gb_demo_init;
    info->exit          = gb_demo_exit;
    info->draw          = gb_demo_draw;
    info->resize        = gb_demo_resize;
    info->event         = gb_demo_event;

    // ok
    return tb_true;
}
tb_void_t gb_application_exit(gb_application_ref_t application)
{
    // trace
    tb_trace_d("exit");
}
tb_bool_t gb_application_loaded(gb_application_ref_t application)
{
    // trace
    tb_trace_d("loaded");

    // ok
    return tb_true;
}
tb_void_t gb_application_background(gb_application_ref_t application)
{
    // trace
    tb_trace_d("background");
}
tb_void_t gb_application_foreground(gb_application_ref_t application)
{
    // trace
    tb_trace_d("foreground");
}
