/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        application.h
 * @ingroup     platform
 *
 */
#ifndef GB_PLATFORM_APPLICATION_H
#define GB_PLATFORM_APPLICATION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the application ref type
typedef struct{}*       gb_application_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * delegate
 */

/*! the application init
 *
 * @param application   the application
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               gb_application_init(gb_application_ref_t application);

/*! the application exit
 *
 * @param application   the application
 */
tb_void_t               gb_application_exit(gb_application_ref_t application);

/*! the application finish loading
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
 * interfaces
 */

/*! the application main entry
 *
 * @param argc          the arguments count
 * @param argv          the arguments list
 *
 * @return              the error code, ok: 0
 */
tb_int_t                gb_application_main(tb_int_t argc, tb_char_t** argv);

/*! the application arguments count
 *
 * @param application   the application
 *
 * @return              the arguments count
 */
tb_size_t               gb_application_argc(gb_application_ref_t application);

/*! the application arguments list
 *
 * @param application   the application
 *
 * @return              the arguments list
 */
tb_char_t**             gb_application_argv(gb_application_ref_t application);

/*! get the application private data
 *
 * @param application   the application
 *
 * @return              the private data
 */
tb_cpointer_t           gb_application_getp(gb_application_ref_t application);

/*! set the application private data
 *
 * @param application   the application
 * @param priv          the private data
 */
tb_void_t               gb_application_setp(gb_application_ref_t application, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
