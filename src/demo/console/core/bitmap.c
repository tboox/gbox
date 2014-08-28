/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_core_bitmap_main(tb_int_t argc, tb_char_t** argv)
{
    // init bitmap
//    gb_bitmap_ref_t bitmap = gb_bitmap_init_from_url(GB_PIXFMT_ARGB8888, argv[1]);
    gb_bitmap_ref_t bitmap = gb_bitmap_init_from_url(GB_PIXFMT_RGB565, argv[1]);
    if (bitmap)
    {
        // trace
        tb_trace_i("%{bitmap}", bitmap);

        // exit bitmap
        gb_bitmap_exit(bitmap);
    }
    return 0;
}
