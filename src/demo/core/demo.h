#ifndef GB_CORE_DEMO_H
#define GB_CORE_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "gbox/gbox.h"

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
 * @param canvas    the canvas
 * @param priv      the user private data
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_demo_init(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/* exit window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_exit(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

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

/*! the window event
 *
 * @param window    the window
 * @param event     the event
 * @param priv      the user private data
 */
tb_void_t           gb_demo_event(gb_window_ref_t window, gb_event_ref_t event, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
