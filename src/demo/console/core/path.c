/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_core_path_main(tb_int_t argc, tb_char_t** argv)
{
    // make path
    gb_path_ref_t path = gb_path_init();
    if (path)
    {
        // init path
        gb_path_move2i_to(path, 100, 100);
        gb_path_line2i_to(path, 200, 200);
        gb_path_quad2i_to(path, 300, 300, 400, 400);
        gb_path_cube2i_to(path, 500, 500, 600, 600, 700, 700);
        gb_path_clos(path);

        // make copy
        gb_path_ref_t copy = gb_path_init();
        if (copy)
        {
            // copy path
            gb_path_copy(copy, path);

#ifdef __gb_debug__
            // dump copy
            gb_path_dump(copy);
#endif
        
            // exit copy
            gb_path_exit(copy);
        }

#ifdef __gb_debug__
        // dump path
        gb_path_dump(path);
#endif

#if 1
        // reverse dump
        tb_rfor_all_if (gb_path_item_ref_t, item, path, item)
        {
            switch (item->code)
            {
            case GB_PATH_CODE_MOVE:
                tb_trace_i("move_to: %{point}", &item->points[0]);
                break;
            case GB_PATH_CODE_LINE:
                tb_trace_i("line_to: %{point}", &item->points[1]);
                break;
            case GB_PATH_CODE_QUAD:
                tb_trace_i("quad_to: %{point}, %{point}", &item->points[1], &item->points[2]);
                break;
            case GB_PATH_CODE_CUBE:
                tb_trace_i("cube_to: %{point}, %{point}, %{point}", &item->points[1], &item->points[2], &item->points[3]);
                break;
            case GB_PATH_CODE_CLOS:
                tb_trace_i("closed");
                break;
            default:
                break;
            }
        }
#endif
    
        // exit path
        gb_path_exit(path);
    }
    return 0;
}
