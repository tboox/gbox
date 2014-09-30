/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t gb_demo_core_vector_test_length(tb_long_t x, tb_long_t y)
{
    // the vector
    gb_vector_t vector;
    gb_vector_imake(&vector, x, y);

    // the length
    gb_float_t length = gb_vector_length(&vector);

    // trace
    tb_trace_i("%{vector}.length: %{float}", &vector, &length);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_core_vector_main(tb_int_t argc, tb_char_t** argv)
{
    gb_demo_core_vector_test_length(0, 10);
    gb_demo_core_vector_test_length(1, 10);
    gb_demo_core_vector_test_length(10, 10);
    gb_demo_core_vector_test_length(512, 512);
    gb_demo_core_vector_test_length(1024, 1024);
    gb_demo_core_vector_test_length(4096, 4096);
    return 0;
}
