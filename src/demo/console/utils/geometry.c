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

    return 0;
}
