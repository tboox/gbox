/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "path"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "path.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the path index
static tb_size_t        g_index = 3;

// the path count
static tb_size_t        g_count = 0;

// the pathes
static gb_path_ref_t    g_pathes[16] = {tb_null};

// the path maker
static gb_path_ref_t    g_maker = tb_null;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_path_init(gb_window_ref_t window)
{
    // make path0
    gb_path_ref_t path0 = gb_path_init();
    if (path0)
    {
        // init path
        gb_path_move2i_to(path0, -100, -100);
        gb_path_line2i_to(path0, -50, -100);
        gb_path_line2i_to(path0, 0, -150);
        gb_path_line2i_to(path0, 50, -100);
        gb_path_line2i_to(path0, 100, -100);
        gb_path_line2i_to(path0, 100, -50);
        gb_path_line2i_to(path0, 20, 0);
        gb_path_line2i_to(path0, 100, 50);
        gb_path_line2i_to(path0, 100, 100);
        gb_path_line2i_to(path0, 50, 100);
        gb_path_line2i_to(path0, 0, 150);
        gb_path_line2i_to(path0, -50, 100);
        gb_path_line2i_to(path0, -100, 100);
        gb_path_line2i_to(path0, -100, 50);
        gb_path_line2i_to(path0, -200, 0);
        gb_path_line2i_to(path0, -100, -50);
        gb_path_clos(path0);

        // save path
        g_pathes[g_count++] = path0;
    }

    // make path1
    gb_path_ref_t path1 = gb_path_init();
    if (path1)
    {
        // init path
        gb_path_move2i_to(path1, 0, 0);
        gb_path_line2i_to(path1, -50, -100);
        gb_path_line2i_to(path1, 0, -200);
        gb_path_line2i_to(path1, 50, -100);
        gb_path_clos(path1);

        gb_path_move2i_to(path1, 0, 0);
        gb_path_line2i_to(path1, 100, -50);
        gb_path_line2i_to(path1, 200, 0);
        gb_path_line2i_to(path1, 100, 50);
        gb_path_clos(path1);

        gb_path_move2i_to(path1, 0, 0);
        gb_path_line2i_to(path1, 50, 100);
        gb_path_line2i_to(path1, 0, 200);
        gb_path_line2i_to(path1, -50, 100);
        gb_path_clos(path1);

        gb_path_move2i_to(path1, 0, 0);
        gb_path_line2i_to(path1, -100, 50);
        gb_path_line2i_to(path1, -200, 0);
        gb_path_line2i_to(path1, -100, -50);
        gb_path_clos(path1);

        // save path
        g_pathes[g_count++] = path1;
    }

    // make path2
    gb_path_ref_t path2 = gb_path_init();
    if (path2)
    {
        // init path
        gb_path_add_rect2i(path2, -200, -200, 400, 400, GB_ROTATE_DIRECTION_CW);
        gb_path_add_rect2i(path2, -100, -100, 200, 200, GB_ROTATE_DIRECTION_CW);
        gb_path_add_rect2i(path2, -50, -50, 100, 100, GB_ROTATE_DIRECTION_CW);

        // save path
        g_pathes[g_count++] = path2;
    }

    // make path3
    gb_path_ref_t path3 = gb_path_init();
    if (path3)
    {
        // init path
        gb_path_move2i_to(path3, -100, -100);
        gb_path_quad2i_to(path3, -50, -100, 0, -150);
        gb_path_quad2i_to(path3, 50, -100, 100, -100);
        gb_path_quad2i_to(path3, 100, -50, 20, 0);
        gb_path_quad2i_to(path3, 100, 50, 100, 100);
        gb_path_quad2i_to(path3, 50, 100, 0, 150);
        gb_path_quad2i_to(path3, -50, 100, -100, 100);
        gb_path_quad2i_to(path3, -100, 50, -200, 0);
        gb_path_quad2i_to(path3, -100, -50, -100, -100);
        gb_path_clos(path3);

        // save path
        g_pathes[g_count++] = path3;
    }

    // make path4
    gb_path_ref_t path4 = gb_path_init();
    if (path4)
    {
        // init path
        gb_path_move2i_to(path4, 0, 0);
        gb_path_quad2i_to(path4, -50, -100, 0, -200);
        gb_path_quad2i_to(path4, 50, -100, 0, 0);
        gb_path_clos(path4);

        gb_path_move2i_to(path4, 0, 0);
        gb_path_quad2i_to(path4, 100, -50, 200, 0);
        gb_path_quad2i_to(path4, 100, 50, 0, 0);
        gb_path_clos(path4);

        gb_path_move2i_to(path4, 0, 0);
        gb_path_quad2i_to(path4, 50, 100, 0, 200);
        gb_path_quad2i_to(path4, -50, 100, 0, 0);
        gb_path_clos(path4);

        gb_path_move2i_to(path4, 0, 0);
        gb_path_quad2i_to(path4, -100, 50, -200, 0);
        gb_path_quad2i_to(path4, -100, -50, 0, 0);
        gb_path_clos(path4);

        // save path
        g_pathes[g_count++] = path4;
    }
}
tb_void_t gb_demo_path_exit(gb_window_ref_t window)
{
    // done
    tb_size_t i = 0;
    for (i = 0; i < g_count; i++)
    {
        // exit it
        if (g_pathes[i]) gb_path_exit(g_pathes[i]);
        g_pathes[i] = tb_null;
    }

    // exit maker
    if (g_maker) gb_path_exit(g_maker);
    g_maker = tb_null;
}
tb_void_t gb_demo_path_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // fill
    gb_canvas_color_set(canvas, GB_COLOR_RED);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_FILL);
    gb_canvas_draw_path(canvas, g_pathes[g_index]);

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_path(canvas, g_pathes[g_index]);
}
tb_void_t gb_demo_path_event(gb_window_ref_t window, gb_event_ref_t event)
{
    // switch path
    if (event->type == GB_EVENT_TYPE_KEYBOARD && event->u.keyboard.pressed)
    {
        if (event->u.keyboard.code == 'p')
            g_index = (g_index + 1) % g_count;
        else if (event->u.keyboard.code == 'q')
        {
            // finished
            if (g_maker)
            {
                // close it
                gb_path_clos(g_maker);

                // save the path
                g_pathes[g_count++] = g_maker;
                g_maker = tb_null;

                // switch the new path
                g_index = g_count - 1;
            }
        }
    }
    // make path
    else if (event->type == GB_EVENT_TYPE_MOUSE)
    {
        if (    event->u.mouse.code == GB_MOUSE_DOWN
            &&  event->u.mouse.button == GB_MOUSE_BUTTON_LEFT)
        {
            // the x0 and y0
            tb_long_t x0 = gb_window_width(window) >> 1;
            tb_long_t y0 = gb_window_height(window) >> 1;

            // the cursor x and y
            tb_long_t cx = gb_float_to_long(event->u.mouse.cursor.x);
            tb_long_t cy = gb_float_to_long(event->u.mouse.cursor.y);

            // append points to the maker
            if (!g_maker)
            {
                // make it
                g_maker = gb_path_init();
                tb_assert_abort(g_maker);

                // move-to
                gb_path_move2i_to(g_maker, cx - x0, y0 - cy);
            }
            else
            {
                // line-to
                gb_path_line2i_to(g_maker, cx - x0, y0 - cy);
            }
        }
    }
}

