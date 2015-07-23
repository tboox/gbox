/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static tb_void_t gb_demo_utils_geometry_is_ccw()
{
    /* make points
     *
     * (-10, -50)       (500, -50)
     *    . <-------------- . p1
     *   p2                 |
     *                      |
     *                      |
     *                      . p0
     *                  (500, 100)
     */
    gb_point_t p0;
    gb_point_t p1;
    gb_point_t p2;
    gb_point_imake(&p0, 500, 100);
    gb_point_imake(&p1, 500, -50);
    gb_point_imake(&p2, -10, -50);
    
    // check
    if (!gb_points_is_ccw(&p0, &p1, &p2)) tb_abort();
    if (!gb_points_is_ccw(&p1, &p2, &p0)) tb_abort();
    if (!gb_points_is_ccw(&p2, &p0, &p1)) tb_abort();

    if (gb_points_is_ccw(&p0, &p2, &p1)) tb_abort();
    if (gb_points_is_ccw(&p1, &p0, &p2)) tb_abort();
    if (gb_points_is_ccw(&p2, &p1, &p0)) tb_abort();
}
static tb_void_t gb_demo_utils_geometry_in_point()
{
    /* make points
     *
     * (-10, -50)       (500, -50)
     *    . <-------------- . p1
     *   p2                 |
     *                      |
     *                      |
     *                      . p0
     *                  (500, 100)
     */
    gb_point_t p0;
    gb_point_t p1;
    gb_point_t p2;
    gb_point_imake(&p0, 500, 100);
    gb_point_imake(&p1, 500, -50);
    gb_point_imake(&p2, -10, -50);
    
    // check
    if (!gb_point_in_top_or_horizontal(&p1, &p0)) tb_abort();
    if (!gb_point_in_top_or_horizontal(&p2, &p0)) tb_abort();
    if (!gb_point_in_top_or_horizontal(&p2, &p1)) tb_abort();

    if (!gb_point_in_left_or_vertical(&p2, &p1)) tb_abort();
    if (!gb_point_in_left_or_vertical(&p2, &p0)) tb_abort();
    if (!gb_point_in_left_or_vertical(&p1, &p0)) tb_abort();
}
static tb_void_t gb_demo_utils_geometry_in_segment()
{
    /* make points
     *
     * (-10, -50)       (500, -50)
     *    . <-------------- . p1
     *   p2                 |
     *                      |
     *                      |
     *                      . p0
     *                  (500, 100)
     */
    gb_point_t p0;
    gb_point_t p1;
    gb_point_t p2;
    gb_point_imake(&p0, 500, 100);
    gb_point_imake(&p1, 500, -50);
    gb_point_imake(&p2, -10, -50);
    
    // check
    if (!gb_point_in_segment_left(&p2, &p1, &p0)) tb_abort();
    if (!gb_point_in_segment_right(&p1, &p2, &p0)) tb_abort();
    if (!gb_point_in_segment_top(&p1, &p2, &p0)) tb_abort();
    if (!gb_point_in_segment_bottom(&p0, &p2, &p1)) tb_abort();
}
static tb_void_t gb_demo_utils_geometry_intersection()
{
    // the points
    gb_point_t org1;
    gb_point_t dst1;
    gb_point_t org2;
    gb_point_t dst2;
    gb_point_t intersection;

    // clear random
    tb_random_clear(tb_null);

    // test results
    __tb_volatile__ tb_size_t count = 256;
    while (count--)
    {
        // make range
        tb_long_t x_range = ((count + 1) * GB_WIDTH_MAXN >> 8);
        tb_long_t y_range = ((count + 1) * GB_HEIGHT_MAXN >> 8);

        // make line segments
        gb_point_imake(&org1, tb_random_range(tb_null, -x_range, x_range), tb_random_range(tb_null, -y_range, y_range));
        gb_point_imake(&dst1, tb_random_range(tb_null, -x_range, x_range), tb_random_range(tb_null, -y_range, y_range));
        gb_point_imake(&org2, tb_random_range(tb_null, -x_range, x_range), tb_random_range(tb_null, -y_range, y_range));
        gb_point_imake(&dst2, tb_random_range(tb_null, -x_range, x_range), tb_random_range(tb_null, -y_range, y_range));

        // calculate intersection
        if (gb_segment_intersection(&org1, &dst1, &org2, &dst2, &intersection))
        {
            // trace
            tb_trace_i("(%{point} => %{point}) x (%{point} => %{point})): %{point}", &org1, &dst1, &org2, &dst2, &intersection);
        }
    }

    // clear random
    tb_random_clear(tb_null);

    // test performance
    count = 1000000;
    tb_size_t in = 0;
    tb_hong_t dt = tb_mclock();
    while (count--)
    {
        // make line segments
        gb_point_imake(&org1, tb_random_range(tb_null, -GB_WIDTH_MAXN, GB_WIDTH_MAXN), tb_random_range(tb_null, -GB_HEIGHT_MAXN, GB_HEIGHT_MAXN));
        gb_point_imake(&dst1, tb_random_range(tb_null, -GB_WIDTH_MAXN, GB_WIDTH_MAXN), tb_random_range(tb_null, -GB_HEIGHT_MAXN, GB_HEIGHT_MAXN));
        gb_point_imake(&org2, tb_random_range(tb_null, -GB_WIDTH_MAXN, GB_WIDTH_MAXN), tb_random_range(tb_null, -GB_HEIGHT_MAXN, GB_HEIGHT_MAXN));
        gb_point_imake(&dst2, tb_random_range(tb_null, -GB_WIDTH_MAXN, GB_WIDTH_MAXN), tb_random_range(tb_null, -GB_HEIGHT_MAXN, GB_HEIGHT_MAXN));

        // calculate intersection
        if (gb_segment_intersection(&org1, &dst1, &org2, &dst2, &intersection))
        {
            // update the intersection number
            in++;
        }
    }
    dt = tb_mclock() - dt;

    // trace
    tb_trace_i("intersection: count: %lu, time: %lld ms", in, dt);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_utils_geometry_main(tb_int_t argc, tb_char_t** argv)
{
    // test is ccw
    gb_demo_utils_geometry_is_ccw();

    // test in point
    gb_demo_utils_geometry_in_point();

    // test in segment
    gb_demo_utils_geometry_in_segment();

    // test intersection
    gb_demo_utils_geometry_intersection();

    gb_point_t org1;
    gb_point_t dst1;
    gb_point_t org2;
    gb_point_t dst2;
    gb_point_t intersection;
    gb_point_make(&org1, -33.541019, 0.000000);
    gb_point_make(&dst1, 100.000000, 66.770507);
    gb_point_make(&org2, -33.541019, -0.000000);
    gb_point_make(&dst2, -6.708203, 13.416407);
    if (gb_segment_intersection(&org1, &dst1, &org2, &dst2, &intersection))
    {
        tb_trace_i("%{point}", &intersection);
    }

    return 0;
}
