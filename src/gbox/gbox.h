/*!The Graphic Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
 * @file        gbox.h
 *
 */
#ifndef GB_GBOX_H
#define GB_GBOX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "svg/svg.h"
#include "core/core.h"
#include "utils/utils.h"
#include "platform/platform.h"

/*! architecture
 * 
 * <pre>
 *
 *  ----------------------------------------------------------------------------------------------------------------------------------------------
 * |                                                                      system                                                                  |
 * |----------------------------------------------------------------------------------------------------------------------------------------------|
 * |                                  tbox(platform, io, utils, container, algorithm, libc, libm, string, network...)                             |
 * |----------------------------------------------------------------------------------------------------------------------------------------------|
 * |   mesh    |      svg             |       path        |                          | glut, x11, sdl, ios, android, mac, windows, fbdev, ...     |
 * |           |      fonts           |       paint       |                          |------------------------------------------------------------|
 * |           |      effects         |       shader      | skia, opengl, bitmap, ...|               window                 |                     |
 * |           |      animation       |       bitmap      |                          |          ----------------------------|                     |
 * |           |                      |       clipper     |                          |  window  |   mouse, keyboard, touch  |         audio       |
 * |           |                      |                   |-------------------------------------|---------------------------|                     |
 * |           |                      |                   |               device                |           events          |                     |
 * |           |                      |                   |-------------------------------------|-------------------------------------------------|
 * |   ...     |       ...            |       ...         |               canvas                |                     application                 |
 * |-----------|----------------------|---------------------------------------------------------|-------------------------------------------------|
 * |   utils   |       others ...     |                        core                             |                      platform                   |
 * |----------------------------------------------------------------------------------------------------------------------------------------------|
 * |                                                                -> gbox(graphic)                                                              |
 * |----------------------------------------------------------------------------------------------------------------------------------------------|
 * |                                                                    ubox(ui)                                                                  |
 * |----------------------------------------------------------------------------------------------------------------------------------------------|
 * |                                                                    abox(game)                                                                |
 *  ----------------------------------------------------------------------------------------------------------------------------------------------
 *
 * </pre>
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

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

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
