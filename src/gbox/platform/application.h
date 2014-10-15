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
 * macros
 */

// force using sdl window for application
#ifdef GB_CONFIG_APPLICATION_WINDOW_SDL
#   undef main
#   define main SDL_main
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the application ref type
typedef struct{}*       gb_application_ref_t;

/*! the application init func type
 *
 * @param application   the application
 * @param info          the application info
 *
 * @return              tb_true or tb_false
 */
typedef tb_bool_t       (*gb_application_init_func_t)(gb_application_ref_t application, gb_window_info_ref_t info);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the application 
 *
 * @return              the application
 */
gb_application_ref_t    gb_application(tb_noarg_t);

/*! the application main entry
 *
 * @param argc          the arguments count
 * @param argv          the arguments list
 * @param init          the application init func
 *
 * @return              the error code, ok: 0
 */
tb_int_t                gb_application_main(tb_int_t argc, tb_char_t** argv, gb_application_init_func_t init);

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

/*! get the application window
 *
 * @param application   the application
 *
 * @return              the window
 */
gb_window_ref_t         gb_application_window(gb_application_ref_t application);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__
#endif
