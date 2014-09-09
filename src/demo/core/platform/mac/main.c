/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../../application.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // init application func
    gb_application_func_t func =
    {
        gb_application_init
    ,   gb_application_exit
    ,   gb_application_loaded
    ,   gb_application_background
    ,   gb_application_foreground
    };

    // ok?
    return gb_application_main(argc, argv, &func);
}
