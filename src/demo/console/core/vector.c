/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t gb_demo_core_vector_test_length_near0()
{
    // the vector
    gb_vector_t vector;
    gb_vector_make(&vector, GB_NEAR0, GB_NEAR0);

    // the length
    gb_float_t length = gb_vector_length(&vector);

    // trace
    tb_trace_i("%{vector}.length: %{float}", &vector, &length);
}
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
static tb_void_t gb_demo_core_vector_test_normalize_near0()
{
    // the vector
    gb_vector_t vector;
    gb_vector_make(&vector, GB_NEAR0, GB_NEAR0);

    // normalize it
    tb_bool_t ok = gb_vector_normalize(&vector);

    // the length
    gb_float_t length = gb_vector_length(&vector);

    // trace
    tb_trace_i("%{vector}.length: %{float}: %s", &vector, &length, ok? "ok" : "no");
}
static tb_void_t gb_demo_core_vector_test_normalize(tb_long_t x, tb_long_t y)
{
    // the vector
    gb_vector_t vector;
    gb_vector_imake(&vector, x, y);

    // normalize it
    tb_bool_t ok = gb_vector_normalize(&vector);

    // the length
    gb_float_t length = gb_vector_length(&vector);

    // trace
    tb_trace_i("normalize: %{vector}.length: %{float}: %s", &vector, &length, ok? "ok" : "no");
}
static tb_void_t gb_demo_core_vector_test_length_set(tb_long_t x, tb_long_t y, tb_long_t n)
{
    // the vector
    gb_vector_t vector;
    gb_vector_imake(&vector, x, y);

    // set length
    tb_bool_t ok = gb_vector_length_set(&vector, gb_long_to_float(n));

    // the length
    gb_float_t length = gb_vector_length(&vector);

    // trace
    tb_trace_i("set %{vector}.length: %ld => %{float}: %s", &vector, n, &length, ok? "ok" : "no");
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_core_vector_main(tb_int_t argc, tb_char_t** argv)
{
    gb_demo_core_vector_test_length_near0();
    gb_demo_core_vector_test_length(0, 1);
    gb_demo_core_vector_test_length(0, 10);
    gb_demo_core_vector_test_length(1, 10);
    gb_demo_core_vector_test_length(10, 10);
    gb_demo_core_vector_test_length(200, 200);
    gb_demo_core_vector_test_length(512, 512);
    gb_demo_core_vector_test_length(1024, 1024);
    gb_demo_core_vector_test_length(4096, 4096);

    gb_demo_core_vector_test_normalize_near0();
    gb_demo_core_vector_test_normalize(0, 1);
    gb_demo_core_vector_test_normalize(0, 10);
    gb_demo_core_vector_test_normalize(1, 10);
    gb_demo_core_vector_test_normalize(10, 10);
    gb_demo_core_vector_test_normalize(512, 512);
    gb_demo_core_vector_test_normalize(1024, 1024);
    gb_demo_core_vector_test_normalize(4096, 4096);

    gb_demo_core_vector_test_length_set(0, 1, 10);
    gb_demo_core_vector_test_length_set(0, 10, 50);
    gb_demo_core_vector_test_length_set(1, 10, 100);
    gb_demo_core_vector_test_length_set(10, 10, 200);
    gb_demo_core_vector_test_length_set(512, 512, 300);
    gb_demo_core_vector_test_length_set(1024, 1024, 400);
    gb_demo_core_vector_test_length_set(4096, 4096, 500);
    return 0;
}
