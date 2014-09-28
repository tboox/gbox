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
