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
 * @file        clipper.h
 * @ingroup     core
 */
#ifndef GB_CORE_CLIPPER_H
#define GB_CORE_CLIPPER_H

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

/// the clipper mode enum
typedef enum __gb_clipper_mode_e
{
    GB_CLIPPER_MODE_NONE        = 0x00
,   GB_CLIPPER_MODE_SUBTRACT    = 0x01
,   GB_CLIPPER_MODE_INTERSECT   = 0x02
,   GB_CLIPPER_MODE_UNION       = 0x03
,   GB_CLIPPER_MODE_REPLACE     = 0x04

}gb_clipper_mode_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init clipper
 *
 * @return                  the clipper
 */
gb_clipper_ref_t            gb_clipper_init(tb_noarg_t);

/*! exit clipper
 *
 * @param clipper           the clipper
 */
tb_void_t                   gb_clipper_exit(gb_clipper_ref_t clipper);

/*! the clipper shape count
 *
 * @param clipper           the clipper
 *
 * @return                  the item count
 */
tb_size_t                   gb_clipper_size(gb_clipper_ref_t clipper);

/*! clear the clipper
 *
 * @param clipper           the clipper
 */
tb_void_t                   gb_clipper_clear(gb_clipper_ref_t clipper);

/*! copy clipper 
 *
 * @param clipper           the clipper 
 * @param copied            the copied clipper
 */
tb_void_t                   gb_clipper_copy(gb_clipper_ref_t clipper, gb_clipper_ref_t copied);

/*! get the current clipper matrix
 *
 * @param clipper           the clipper 
 *
 * @return                  the matrix
 */
gb_matrix_ref_t             gb_clipper_matrix(gb_clipper_ref_t clipper);

/*! set the current clipper matrix
 *
 * @param clipper           the clipper 
 * @param matrix            the matrix
 */
tb_void_t                   gb_clipper_matrix_set(gb_clipper_ref_t clipper, gb_matrix_ref_t matrix);

/*! add path
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param path              the path
 */
tb_void_t                   gb_clipper_add_path(gb_clipper_ref_t clipper, tb_size_t mode, gb_path_ref_t path);

/*! add triangle
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param triangle          the triangle
 */
tb_void_t                   gb_clipper_add_triangle(gb_clipper_ref_t clipper, tb_size_t mode, gb_triangle_ref_t triangle);

/*! add rect
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param rect              the rect
 */
tb_void_t                   gb_clipper_add_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_rect_ref_t rect);

/*! add round rect
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param rect              the round rect
 */
tb_void_t                   gb_clipper_add_round_rect(gb_clipper_ref_t clipper, tb_size_t mode, gb_round_rect_ref_t rect);

/*! add circle
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param circle            the circle
 */
tb_void_t                   gb_clipper_add_circle(gb_clipper_ref_t clipper, tb_size_t mode, gb_circle_ref_t circle);

/*! add ellipse
 *
 * @param clipper           the clipper
 * @param mode              the clipper mode
 * @param ellipse           the ellipse
 */
tb_void_t                   gb_clipper_add_ellipse(gb_clipper_ref_t clipper, tb_size_t mode, gb_ellipse_ref_t ellipse);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
