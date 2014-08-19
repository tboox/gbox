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
 * @file        version.h
 *
 */
#ifndef GB_PREFIX_VERSION_H
#define GB_PREFIX_VERSION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "config.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// the major version
#define GB_VERSION_MAJOR            GB_CONFIG_VERSION_MAJOR

/// the minor version
#define GB_VERSION_MINOR            GB_CONFIG_VERSION_MINOR

/// the alter version
#define GB_VERSION_ALTER            GB_CONFIG_VERSION_ALTER

/// the build version
#define GB_VERSION_BUILD            GB_CONFIG_VERSION_BUILD

/// the build version string
#define GB_VERSION_BUILD_STRING     __tb_mstring_ex__(GB_CONFIG_VERSION_BUILD)

/// the version string
#define GB_VERSION_STRING           __tb_mstrcat6__("gbox_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, GB_VERSION_MAJOR, _, GB_VERSION_MINOR, _, GB_VERSION_ALTER, _, GB_CONFIG_VERSION_BUILD)), "_", TB_ARCH_VERSION_STRING, " by ", TB_COMPILER_VERSION_STRING)

/// the short version string
#define GB_VERSION_SHORT_STRING     __tb_mstrcat__("gbox_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, GB_VERSION_MAJOR, _, GB_VERSION_MINOR, _, GB_VERSION_ALTER, _, GB_CONFIG_VERSION_BUILD)))

#endif


