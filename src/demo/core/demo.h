#ifndef GB_CORE_DEMO_H
#define GB_CORE_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "gbox/gbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* init window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_init_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/* exit window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_exit_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/* draw window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_draw_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);

/* resize window
 *
 * @param window    the window
 * @param canvas    the canvas
 * @param priv      the user private data
 */
tb_void_t           gb_demo_resize_func(gb_window_ref_t window, gb_canvas_ref_t canvas, tb_cpointer_t priv);



#endif
