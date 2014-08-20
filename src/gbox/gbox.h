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
 * @file        gbox.h
 *
 */
#ifndef GB_GBOX_H
#define GB_GBOX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "ui/ui.h"
#include "svg/svg.h"
#include "core/core.h"
#include "game/game.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

#ifdef __gb_debug__
#   define __gb_mode_debug__    TB_MODE_DEBUG
#else
#   define __gb_mode_debug__    (0)
#endif

#ifdef __gb_small__
#   define __gb_mode_small__    TB_MODE_SMALL
#else
#   define __gb_mode_small__    (0)
#endif

/*! init gbox
 *
 * @return          tb_true or tb_false
 *
 * @code
    #include "gbox/gbox.h"

    int main(int argc, char** argv)
    {
        // init gbox
        if (!gb_init()) return 0;


        // exit gbox
        gb_exit();
        return 0;
    }
 * @endcode
 */
#define gb_init()     gb_init_((tb_size_t)(__gb_mode_debug__ | __gb_mode_small__), GB_VERSION_BUILD)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

/*! init the gbox library
 *
 * @param mode      the compile mode for check __tb_small__ and __tb_debug__
 * @param build     the build version
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_init_(tb_size_t mode, tb_hize_t build);

/// exit the gbox library
tb_void_t           gb_exit(tb_noarg_t);

/*! the gbox version
 *
 * @return          the gbox version
 */
tb_version_t const* gb_version(tb_noarg_t);

__tb_extern_c_leave__
#endif
