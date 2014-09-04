#ifndef GB_CORE_DEMO_RECT_H
#define GB_CORE_DEMO_RECT_H

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

/* clos window
 *
 * @param window    the window
 */
tb_void_t           gb_demo_rect_clos(gb_window_ref_t window);

/* draw window
 *
 * @param window    the window
 * @param canvas    the canvas
 */
tb_void_t           gb_demo_rect_draw(gb_window_ref_t window, gb_canvas_ref_t canvas);

/*! the window event
 *
 * @param window    the window
 * @param event     the event
 */
tb_void_t           gb_demo_rect_event(gb_window_ref_t window, gb_event_ref_t event);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
