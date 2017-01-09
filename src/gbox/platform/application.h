/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
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
