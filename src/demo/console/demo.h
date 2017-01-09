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
 * @file        demo.h
 *
 */
#ifndef GB_DEMO_H
#define GB_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "gbox/gbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the main decl
#define GB_DEMO_MAIN_DECL(name)         tb_int_t gb_demo_##name##_main(tb_int_t argc, tb_char_t** argv)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

// core
GB_DEMO_MAIN_DECL(core_path);
GB_DEMO_MAIN_DECL(core_bitmap);
GB_DEMO_MAIN_DECL(core_vector);

// utils
GB_DEMO_MAIN_DECL(utils_mesh);
GB_DEMO_MAIN_DECL(utils_geometry);

// other
GB_DEMO_MAIN_DECL(other_test);

#endif


