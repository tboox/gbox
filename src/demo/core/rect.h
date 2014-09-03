#ifndef GB_CORE_DEMO_RECT_H
#define GB_CORE_DEMO_RECT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* clos window
 *
 * @param window    the window
 * @param priv      the user private data
 */
tb_void_t           gb_demo_rect_clos(gb_window_ref_t window, tb_cpointer_t priv);

/* draw window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_rect_draw(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/*! the window event
 *
 * @param window    the window
 * @param event     the event
 * @param priv      the user private data
 */
tb_void_t           gb_demo_rect_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv);

#endif
