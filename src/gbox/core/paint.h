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
 * @file        paint.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PAINT_H
#define GB_CORE_PAINT_H

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

/// the paint mode enum
typedef enum __gb_paint_mode_e
{
    GB_PAINT_MODE_NONE          = 0 //!< none
,   GB_PAINT_MODE_FILL          = 1 //!< fill
,   GB_PAINT_MODE_STROKE        = 2 //!< stroke
,   GB_PAINT_MODE_FILL_STROKE   = 3 //!< fill and stroke

}gb_paint_mode_e;

/// the paint flag enum
typedef enum __gb_paint_flag_e
{
    GB_PAINT_FLAG_NONE          = 0 //!< none
,   GB_PAINT_FLAG_ANTIALIASING  = 1 //!< antialiasing
,   GB_PAINT_FLAG_FILTER_BITMAP = 2 //!< filter bitmap

}gb_paint_flag_e;

/// the paint stroke cap enum
typedef enum __gb_paint_cap_e
{
    GB_PAINT_CAP_BUTT           = 0 //!< no extension
,   GB_PAINT_CAP_ROUND          = 1 //!< a semi-circle extension
,   GB_PAINT_CAP_SQUARE         = 2 //!< a half square extension

}gb_paint_cap_e;

/// the paint stroke join enum
typedef enum __gb_paint_join_e
{
    GB_PAINT_JOIN_MITER         = 0 //!< a sharp join
,   GB_PAINT_JOIN_ROUND         = 1 //!< a round join
,   GB_PAINT_JOIN_BEVEL         = 2 //!< a flat bevel join

}gb_paint_join_e;

/// the paint fill rule enum
typedef enum __gb_paint_rule_e
{
    GB_PAINT_RULE_ODD           = 0 //!< odd fill
,   GB_PAINT_RULE_NONZERO       = 1 //!< non-zero fill

}gb_paint_rule_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init paint
 *
 * @return          the paint 
 */
gb_paint_ref_t      gb_paint_init(tb_noarg_t);

/*! exit paint
 *
 * @param paint     the paint 
 */
tb_void_t           gb_paint_exit(gb_paint_ref_t paint);

/*! clear paint
 *
 * @param paint     the paint 
 */
tb_void_t           gb_paint_clear(gb_paint_ref_t paint);

/*! copy paint
 *
 * @param paint     the paint 
 * @param copied    the copied paint 
 */
tb_void_t           gb_paint_copy(gb_paint_ref_t paint, gb_paint_ref_t copied);

/*! the paint mode
 *
 * @param paint     the paint 
 *
 * @return          the paint mode
 */
tb_size_t           gb_paint_mode(gb_paint_ref_t paint);

/*! set the paint mode
 *
 * @param paint     the paint 
 * @param mode      the paint mode
 */
tb_void_t           gb_paint_mode_set(gb_paint_ref_t paint, tb_size_t mode);

/*! the paint flag
 *
 * @param paint     the paint 
 *
 * @return          the paint flag
 */
tb_size_t           gb_paint_flag(gb_paint_ref_t paint);

/*! set the paint flag
 *
 * @param paint     the paint 
 * @param flag      the paint flag
 */
tb_void_t           gb_paint_flag_set(gb_paint_ref_t paint, tb_size_t flag);

/*! the paint color
 *
 * @param paint     the paint 
 *
 * @return          the paint color
 */
gb_color_t          gb_paint_color(gb_paint_ref_t paint);

/*! set the paint color
 *
 * @param paint     the paint 
 * @param color     the paint color
 */
tb_void_t           gb_paint_color_set(gb_paint_ref_t paint, gb_color_t color);

/*! the paint alpha
 *
 * @param paint     the paint 
 *
 * @return          the paint alpha
 */
tb_byte_t           gb_paint_alpha(gb_paint_ref_t paint);

/*! set the paint alpha
 *
 * @param paint     the paint 
 * @param alpha     the paint alpha
 */
tb_void_t           gb_paint_alpha_set(gb_paint_ref_t paint, tb_byte_t alpha);

/*! the paint width
 *
 * @param paint     the paint 
 *
 * @return          the paint width
 */
gb_float_t          gb_paint_width(gb_paint_ref_t paint);

/*! set the paint width
 *
 * @param paint     the paint 
 * @param width     the paint width
 */
tb_void_t           gb_paint_width_set(gb_paint_ref_t paint, gb_float_t width);

/*! the paint cap
 *
 * @param paint     the paint 
 *
 * @return          the paint cap
 */
tb_size_t           gb_paint_cap(gb_paint_ref_t paint);

/*! set the paint cap
 *
 * @param paint     the paint 
 * @param cap       the paint cap
 */
tb_void_t           gb_paint_cap_set(gb_paint_ref_t paint, tb_size_t cap);

/*! the paint join
 *
 * @param paint     the paint 
 *
 * @return          the paint join
 */
tb_size_t           gb_paint_join(gb_paint_ref_t paint);

/*! set the paint join
 *
 * @param paint     the paint 
 * @param join      the paint join
 */
tb_void_t           gb_paint_join_set(gb_paint_ref_t paint, tb_size_t join);

/*! the paint rule
 *
 * @param paint     the paint 
 *
 * @return          the paint rule
 */
tb_size_t           gb_paint_rule(gb_paint_ref_t paint);

/*! set the paint rule
 *
 * @param paint     the paint 
 * @param rule      the paint rule
 */
tb_void_t           gb_paint_rule_set(gb_paint_ref_t paint, tb_size_t rule);

/*! the paint quality
 *
 * @param paint     the paint 
 *
 * @return          the paint quality
 */
tb_size_t           gb_paint_quality(gb_paint_ref_t paint);

/*! set the paint alpha
 *
 * @param paint     the paint 
 * @param alpha     the paint alpha
 */
tb_void_t           gb_paint_quality_set(gb_paint_ref_t paint, tb_size_t quality);

/*! the paint shader
 *
 * @param paint     the paint 
 *
 * @return          the paint shader
 */
gb_shader_ref_t     gb_paint_shader(gb_paint_ref_t paint);

/*! set the paint shader
 *
 * @param paint     the paint 
 * @param shader    the paint shader
 */
tb_void_t           gb_paint_shader_set(gb_paint_ref_t paint, gb_shader_ref_t shader);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
