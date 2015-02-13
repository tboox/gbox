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

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_utils_geometry_main(tb_int_t argc, tb_char_t** argv)
{
    // test is ccw
    gb_demo_utils_geometry_is_ccw();
    return 0;
}
