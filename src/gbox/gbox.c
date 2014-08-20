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
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
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
static __tb_inline__ tb_bool_t tb_check_mode(tb_size_t mode)
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
static __tb_inline__ tb_bool_t tb_version_check(tb_hize_t build)
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
        tb_trace_w("version: %s != %s", GB_VERSION_STRING, GB_VERSION_BUILD_STRING);
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
    if (!tb_check_mode(mode)) return tb_false;

    // check version
    tb_version_check(build);

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

