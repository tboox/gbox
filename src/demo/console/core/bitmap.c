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
        tb_trace_i("bitmap: %lux%lu, pixfmt: %s, size: %lu, row_bytes: %lu, has_alpha: %s", gb_bitmap_width(bitmap), gb_bitmap_height(bitmap), gb_pixmap(gb_bitmap_pixfmt(bitmap), 0xff)->name, gb_bitmap_size(bitmap), gb_bitmap_row_bytes(bitmap), gb_bitmap_has_alpha(bitmap)? "ok" : "no");

        // exit bitmap
        gb_bitmap_exit(bitmap);
    }
    return 0;
}
