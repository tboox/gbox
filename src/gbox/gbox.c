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
 * @file        gbox.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "gbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the initial count
static tb_atomic_t  g_init = 0;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tb_inline__ tb_bool_t gb_check_mode(tb_size_t mode)
{
#ifdef __gb_debug__
    if (!(mode & TB_MODE_DEBUG))
    {
        tb_trace_e("libgbox.a has __tb_debug__ but gbox/gbox.h not");
        return tb_false;
    }
#else
    if (mode & TB_MODE_DEBUG)
    {
        tb_trace_e("gbox/gbox.h has __tb_debug__ but libgbox.a not");
        return tb_false;
    }
#endif

#ifdef __gb_small__
    if (!(mode & TB_MODE_SMALL))
    {
        tb_trace_e("libgbox.a has __tb_small__ but gbox/gbox.h not");
        return tb_false;
    }
#else
    if (mode & TB_MODE_SMALL)
    {
        tb_trace_e("gbox/gbox.h has __tb_small__ but libgbox.a not");
        return tb_false;
    }
#endif

    // ok
    return tb_true;
}
static __tb_inline__ tb_bool_t gb_version_check(tb_hize_t build)
{
    // the version oly for link the static vtag string
    tb_version_t const* version = tb_version(); tb_used(version);

    // ok
    if ((build / 100) == (GB_VERSION_BUILD / 100))
    {
        tb_trace_d("version: %s", GB_VERSION_STRING);
        return tb_true;
    }
    else
    {
        tb_trace_w("version: %s != %llu", GB_VERSION_STRING, build);
    }

    // no
    return tb_false;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_init_(tb_size_t mode, tb_hize_t build)
{
    // is inited?
    if (tb_atomic_fetch_and_inc(&g_init)) return tb_true;

    // trace
    tb_trace_d("init: ..");

    // check mode
    if (!gb_check_mode(mode)) return tb_false;

    // check version
    gb_version_check(build);

    // init platform
    if (!gb_platform_init()) return tb_false;

    // init core
    if (!gb_core_init()) return tb_false;

    // trace
    tb_trace_d("init: ok");

    // ok
    return tb_true;
}
tb_void_t gb_exit()
{
    // need exit?
    tb_long_t init = 0;
    if ((init = tb_atomic_dec_and_fetch(&g_init)) > 0) return ;

    // check
    tb_assert_and_check_return(!init);

    // exit core
    gb_core_exit();

    // exit platform
    gb_platform_exit();
}
tb_version_t const* gb_version()
{
    // init version tag for binary search
    static __tb_volatile__ tb_char_t const* s_vtag = "[gbox]: [vtag]: " GB_VERSION_STRING; tb_used(s_vtag);

    // init version
    static tb_version_t s_version = {0};
    if (!s_version.major)
    {
        s_version.major = GB_VERSION_MAJOR;
        s_version.minor = GB_VERSION_MINOR;
        s_version.alter = GB_VERSION_ALTER;
        s_version.build = (tb_hize_t)tb_atoll(GB_VERSION_BUILD_STRING);
    }

    return &s_version;
}

