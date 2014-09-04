#ifndef GB_CORE_DEMO_ELLIPSE_H
#define GB_CORE_DEMO_ELLIPSE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init window
 *
 * @param window    the window
 */
tb_void_t           gb_demo_ellipse_init(gb_window_ref_t window);

/* exit window
 *
 * @param window    the window
 */
tb_void_t           gb_demo_ellipse_exit(gb_window_ref_t window);

/* draw window
 *
 * @param window    the window
 * @param canvas    the canvas
 */
tb_void_t           gb_demo_ellipse_draw(gb_window_ref_t window, gb_canvas_ref_t canvas);

/*! the window event
 *
 * @param window    the window
 * @param event     the event
 */
tb_void_t           gb_demo_ellipse_event(gb_window_ref_t window, gb_event_ref_t event);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
