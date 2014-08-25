#ifndef GB_CORE_DEMO_H
#define GB_CORE_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "gbox/gbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */
/* clos window
 *
 * @param window    the window
 * @param priv      the user private data
 */
tb_void_t           gb_demo_clos(gb_window_ref_t window, tb_cpointer_t priv);

/* draw window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_draw(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/* resize window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_resize(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! begin touch
 *
 * @param window    the window
 * @param points    the points
 * @param count     the count
 */
tb_void_t           gb_demo_touch_begin(gb_window_ref_t window, gb_point_t const* points, tb_size_t count);

/*! move touch
 *
 * @param window    the window
 * @param points    the points
 * @param count     the count
 */
tb_void_t           gb_demo_touch_move(gb_window_ref_t window, gb_point_t const* points, tb_size_t count);

/*! end touch
 *
 * @param window    the window
 * @param points    the points
 * @param count     the count
 */
tb_void_t           gb_demo_touch_end(gb_window_ref_t window, gb_point_t const* points, tb_size_t count);

/*! cancel touch
 *
 * @param window    the window
 * @param points    the points
 * @param count     the count
 */
tb_void_t           gb_demo_touch_cancel(gb_window_ref_t window, gb_point_t const* points, tb_size_t count);



#endif
