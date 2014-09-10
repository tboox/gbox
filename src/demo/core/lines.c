/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "lines"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "lines.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_lines_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_lines_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_lines_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // make points
    gb_point_t points[] = 
    {
        { gb_long_to_float(-100),   gb_long_to_float(-100)  }
    ,   { gb_long_to_float(100),    gb_long_to_float(100)   }
    ,   { gb_long_to_float(-100),   gb_long_to_float(100)   }
    ,   { gb_long_to_float(100),    gb_long_to_float(-100)  }
    ,   { gb_long_to_float(0),      gb_long_to_float(100)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(-100)  }
    ,   { gb_long_to_float(100),    gb_long_to_float(0)     }
    ,   { gb_long_to_float(-100),   gb_long_to_float(0)     }
    };

    // stok
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STOK);
    gb_canvas_draw_lines(canvas, points, tb_arrayn(points));
}
tb_void_t gb_demo_lines_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

