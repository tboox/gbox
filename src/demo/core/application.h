#ifndef GB_DEMO_CORE_APPLICATION_H
#define GB_DEMO_CORE_APPLICATION_H

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

/*! the application init func type
 *
 * @param application   the application
 * @param info          the application info
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               gb_application_init(gb_application_ref_t application, gb_window_info_ref_t info);

/*! the application exit
 *
 * @param application   the application
 */
tb_void_t               gb_application_exit(gb_application_ref_t application);

/*! the application have been loaded
 *
 * @param application   the application
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               gb_application_loaded(gb_application_ref_t application);

/*! the application will enter background
 *
 * @param application   the application
 */
tb_void_t               gb_application_background(gb_application_ref_t application);

/*! the application will enter foreground
 *
 * @param application   the application
 */
tb_void_t               gb_application_foreground(gb_application_ref_t application);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
