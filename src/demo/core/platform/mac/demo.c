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

// the pixfmt
#define GB_DEMO_PIXFMT          (GB_PIXFMT_RGB565)
//#define GB_DEMO_PIXFMT            (GB_PIXFMT_RGB888 | GB_PIXFMT_BENDIAN)
//#define GB_DEMO_PIXFMT            (GB_PIXFMT_RGBA5551)
//#define GB_DEMO_PIXFMT            (GB_PIXFMT_RGBA4444)
//#define GB_DEMO_PIXFMT            (GB_PIXFMT_RGBA8888 | GB_PIXFMT_BENDIAN)

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

    // init view
    gb_gl_view_ref_t view = gb_gl_view_init("demo", GB_DEMO_PIXFMT, GB_DEMO_WIDTH, GB_DEMO_HEIGHT);
    if (view)
    {
        // wait 
        getchar();

        // exit view
        gb_gl_view_exit(view);
    }

    // exit gbox
    gb_exit();

    // exit tbox
    tb_exit();

    // ok?
    return 0;
}
