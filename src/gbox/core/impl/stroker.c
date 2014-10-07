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
 * @file        stroker.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "stroker.h"
#include "geometry.h"
#include "../path.h"
#include "../paint.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the stroker capper type
 *
 * @param path                  the path
 * @param center                the center point
 * @param end                   the end point
 * @param normal                the normal vector of the outer contour
 * @param is_line_to            is line-to?
 */
typedef tb_void_t               (*gb_stroker_capper_t)(gb_path_ref_t path, gb_point_ref_t center, gb_point_ref_t end, gb_vector_ref_t normal, tb_bool_t is_line_to);

/* the stroker joiner type
 *
 * @param inner                 the inner path
 * @param outer                 the outer path
 * @param center                the center point
 * @param radius                the radius
 * @param normal_unit_before    the before unit normal of the outer contour
 * @param normal_unit_after     the after unit normal of the outer contour
 * @param miter_invert          the invert miter limit
 * @param is_line_to_prev       is previous line-to?
 * @param is_line_to            is line-to?
 */
typedef tb_void_t               (*gb_stroker_joiner_t)(gb_path_ref_t inner, gb_path_ref_t outer, gb_point_ref_t center, gb_float_t radius, gb_vector_ref_t normal_unit_before, gb_vector_ref_t normal_unit_after, gb_float_t miter_invert, tb_bool_t is_line_to_prev, tb_bool_t is_line_to);

// the stroker joiner angle type enum
typedef enum __gb_stroker_joiner_angle_type_e 
{
    GB_STROKER_JOINER_ANGLE_NEAR0   = 0
,   GB_STROKER_JOINER_ANGLE_NEAR180 = 1
,   GB_STROKER_JOINER_ANGLE_OBTUSE  = 2
,   GB_STROKER_JOINER_ANGLE_SHARP   = 3

}gb_stroker_joiner_angle_type_e;

// the stroker impl type 
typedef struct __gb_stroker_impl_t
{
    // the cap
    tb_size_t               cap;

    // the join
    tb_size_t               join;

    // the radius
    gb_float_t              radius;

    // the miter limit
    gb_float_t              miter;

    // the invert miter limit: 1 / miter
    gb_float_t              miter_invert;

    // the outer path and is the output path
    gb_path_ref_t           path_outer;

    // the inner path and is the temporary path, need reuse it
    gb_path_ref_t           path_inner;

    // the other complete contours path 
    gb_path_ref_t           path_other;

    // the previous point of the contour
    gb_point_t              point_prev;

    // the first point of the contour
    gb_point_t              point_first;

    // the first point of the outer contour
    gb_point_t              outer_first;

    // the previous normal for the outer
    gb_vector_t             normal_prev;

    // the first normal_first for the outer
    gb_vector_t             normal_first;

    // the previous unit normal of the outer contour
    gb_vector_t             normal_unit_prev;

    // the first unit normal of the outer contour
    gb_vector_t             normal_unit_first;

    // the segment count
    tb_long_t               segment_count;

    // the previous operation of the contour is line-to?
    tb_bool_t               is_line_to_prev;

    // the first operation of the contour is line-to?
    tb_bool_t               is_line_to_first;

    // the capper
    gb_stroker_capper_t     capper;

    // the joiner
    gb_stroker_joiner_t     joiner;

}gb_stroker_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_stroker_add_hint(gb_stroker_ref_t stroker, gb_shape_ref_t hint)
{
    // check
    tb_check_return_val(hint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    switch (hint->type)
    {
    case GB_SHAPE_TYPE_RECT:
        {
            gb_stroker_add_rect(stroker, &hint->u.rect);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_LINE:
        {
            gb_point_t points[2];
            points[0] = hint->u.line.p0;
            points[1] = hint->u.line.p1;
            gb_stroker_add_lines(stroker, points, tb_arrayn(points));
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_CIRCLE:
        {
            gb_stroker_add_circle(stroker, &hint->u.circle);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_ELLIPSE:
        {
            gb_stroker_add_ellipse(stroker, &hint->u.ellipse);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_POINT:
        {
            gb_stroker_add_lines(stroker, &hint->u.point, 1);
            ok = tb_true;
        }
        break;
    default:
        break;
    }

    // ok?
    return ok;
}
static tb_void_t gb_stroker_capper_butt(gb_path_ref_t path, gb_point_ref_t center, gb_point_ref_t end, gb_vector_ref_t normal, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(path && end);

    /* cap th butt
     *  
     *                       normal
     *              ----------------------> first outer
     *             |  radius   |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     * reverse add |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     * last inner \|/         \|/         \|/
     *          inner        center       outer
     *             <------------------------
     *                        cap
     */
    gb_path_line_to(path, end);
}
static tb_void_t gb_stroker_capper_round(gb_path_ref_t path, gb_point_ref_t center, gb_point_ref_t end, gb_vector_ref_t normal, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(path && center && end && normal);

    /* cap th round
     *                        
     *                        .
     *                     .   . L
     *                  .       .
     *               .           c1
     *            .
     *         .                   c2
     *      .                      .
     *   .   a                     . L
     * . . . . . . . . . . . . . . .
     *              1         
     *
     * L = 4 * tan(a / 4) / 3
     *
     *      L
     * . . . . . . c1
     * .
     * .
     * .
     * .
     * .
     * .
     * .
     * .                           c2
     * .                           .
     * .                           .
     * .                           . L
     * . a = 90                    .
     * . . . . . . . . . . . . . . .
     *
     * L = 4 * tan(pi/8) / 3 if a == 90 degree
     *
     *
     *                       normal
     *              ----------------------> first outer
     *             |  radius   |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     * reverse add |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *         p3 \|/         \|/         \|/ p1
     *          inner        center       outer
     *             .           .           .
     *          L4 . .         .         . . L1
     *             .   .       .  arc  .   .
     *            c4       .   .   .       c1
     *                 c3. . . . . . .c2
     *                    L3   p2   L2
     *                      
     *                        cap
     *
     * factor = 4 * tan(pi/8) / 3
     *
     * L1 = (normal * factor).rotate(90, cw)
     *    = (lx, ly).rotate(90, cw)
     *    = (-ly, lx)
     *
     * L2 = (normal * factor)
     *    = (lx, ly)
     *
     * L3 = -L2
     *    = (-lx, -ly)
     *
     * L4 = L1
     *    = (-ly, lx)
     *
     * p1 = center + normal
     *    = (x0 + nx, y0 + ny)
     *
     * p2 = center + normal.rotate(90, cw)
     *    = (x0 - ny, y0 + nx)
     *
     * p3 = center - normal
     *    = (x0 - nx, y0 - ny)
     *    = end
     *
     * c1 = p1 + L1
     *    = (x0 + nx - ly, y0 + ny + lx)
     *
     * c2 = p2 + L2
     *    = (x0 - ny + lx, y0 + nx + ly)
     *
     * c3 = p2 + L3
     *    = (x0 - ny - lx, y0 + nx - ly)
     *
     * c4 = p3 + L4
     *    = (x0 - nx - ly, y0 - ny + lx)
     *
     * cap:
     * cube_to(c1, c2, p2) = cube_to(x0 + nx - ly, y0 + ny + lx, x0 - ny + lx, y0 + nx + ly, x0 - ny, y0 + nx)
     * cube_to(c3, c4, p3) = cube_to(x0 - ny - lx, y0 + nx - ly, x0 - nx - ly, y0 - ny + lx, x0 - nx, y0 - ny)
     */

    // the factors
    gb_float_t    x0 = center->x;
    gb_float_t    y0 = center->y;
    gb_float_t    nx = normal->x;
    gb_float_t    ny = normal->y;
    gb_float_t    lx = gb_mul(nx, GB_ARC2CUBE_FACTOR);
    gb_float_t    ly = gb_mul(ny, GB_ARC2CUBE_FACTOR);

    // cap the round
    gb_path_cube2_to(path, x0 + nx - ly, y0 + ny + lx, x0 - ny + lx, y0 + nx + ly, x0 - ny, y0 + nx);
    gb_path_cube2_to(path, x0 - ny - lx, y0 + nx - ly, x0 - nx - ly, y0 - ny + lx, end->x, end->y);
}
static tb_void_t gb_stroker_capper_square(gb_path_ref_t path, gb_point_ref_t center, gb_point_ref_t end, gb_vector_ref_t normal, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(path && center && end && normal);

    // make the patched vector
    gb_vector_t patched;
    gb_vector_rotate2(normal, &patched, GB_ROTATE_DIRECTION_CW);

    /* cap the square
     * 
     *                       normal
     *              ----------------------> first outer
     *             |  radius   |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     * reverse add |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     *             |           |           |
     * last inner \|/         \|/         \|/
     *    end   inner        center       outer
     *            /|\                      |
     *             |                       | patched
     *             |                       |
     *             |                      \|/
     *             <------------------------
     *                        cap
     *
     */
    if (is_line_to)
    {
        // ignore the two points for optimization if cap the line-to contour
        gb_point_t last;
        gb_point_make(&last, center->x + normal->x + patched.x, center->y + normal->y + patched.y);
        gb_path_last_set(path, &last);
        gb_path_line2_to(path, center->x - normal->x + patched.x, center->y - normal->y + patched.y);
    }
    else
    {
        gb_path_line2_to(path, center->x + normal->x + patched.x, center->y + normal->y + patched.y);
        gb_path_line2_to(path, center->x - normal->x + patched.x, center->y - normal->y + patched.y);
        gb_path_line_to(path, end);
    }
}
static gb_float_t gb_stroker_joiner_angle(gb_vector_ref_t normal_unit_before, gb_vector_ref_t normal_unit_after, tb_size_t* ptype)
{
    // check
    tb_assert_abort(normal_unit_before && normal_unit_after);

    /* the cos(angle) value
     *
     *                   normal_before
     *                         | 
     * . . . . . . . . . . . . .   
     *                         .       
     *                       R .    
     *                         .   
     *                         . angle   
     *               . . . . . c . . . . . --> normal_after
     *               .         .    R    .
     *               .         .         .
     *               .         .         .
     *               .         .         .
     * . . . . . . . . . . . . .         .
     *               .                   .
     *               .                   .
     *               .                   .
     *               .                   .
     *               .                   . 
     *               .                   .
     *                            
     */
    gb_float_t angle = gb_vector_dot(normal_unit_before, normal_unit_after);

    // compute the angle type
    if (ptype)
    {
        if (gb_lz(angle)) *ptype = (GB_ONE + angle) <= GB_NEAR0? GB_STROKER_JOINER_ANGLE_NEAR180 : GB_STROKER_JOINER_ANGLE_OBTUSE;
        else *ptype = (GB_ONE - angle) <= GB_NEAR0? GB_STROKER_JOINER_ANGLE_NEAR0 : GB_STROKER_JOINER_ANGLE_SHARP;
    }

    // the angle
    return angle;
}
static tb_void_t gb_stroker_joiner_outer(gb_point_ref_t ctrl, gb_point_ref_t point, tb_cpointer_t priv)
{
    // check
    gb_path_ref_t outer = (gb_path_ref_t)priv;
    tb_assert_abort(outer && point);

    // add quadratic curve for the outer contour
    if (ctrl) gb_path_quad_to(outer, ctrl, point);
}
static tb_void_t gb_stroker_joiner_inner(gb_path_ref_t inner, gb_point_ref_t center, gb_vector_ref_t normal_after)
{
    /* join the inner contour
     *               
     *               <-
     *               . . . . center
     *               .     .
     *               .     .
     * . . . . . . . . . . . 
     * before        .    ->
     *               .
     *               .
     *               .
     *               .
     *             after
     *
     * @note need patch a center first for the radius is larger than segments
     * 
     *          i2 . .
     *           .   .
     *           .   .
     * . . . . . . . . . . . . . 
     * .         .   .   \|/   .
     *i1 . . . . . c . . . . . . - normal_before
     *           .   .      .   
     *           .<- .    .  
     *           .   .  .  outer
     *           .   . 
     *           . . .
     *           |
     *      normal_after
     *
     * inner: i1 => c = > i2
     */
    gb_path_line2_to(inner, center->x, center->y);
    gb_path_line2_to(inner, center->x - normal_after->x, center->y - normal_after->y);
}
static tb_void_t gb_stroker_joiner_miter(gb_path_ref_t inner, gb_path_ref_t outer, gb_point_ref_t center, gb_float_t radius, gb_vector_ref_t normal_unit_before, gb_vector_ref_t normal_unit_after, gb_float_t miter_invert, tb_bool_t is_line_to_prev, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(inner && outer && center && normal_unit_before && normal_unit_after);

    /*                   normal_before
     *                         | 
     * . . . . . . . . . . . . . . . . . . miter 
     *                         .    L  . .
     *                         .     .   .
     *                         .   .     .
     *                         . a       .
     *               . . . . . c . . . . . --> normal_after
     *               .         .    R    .
     *               .         .         .
     *               .         .         .
     *               .         .         .
     * . . . . . . . . . . . . .         .
     *               .                   .
     *               .                   .
     *               .                   .
     *               .                   .
     *               .                   . 
     *               .                   .
     *                            
     * R: radius
     * M: miter limit
     * L: miter length
     * a: degree
     *
     * M = L / R
     * L = R / cos(a/2)
     */

    // compute the cos(a) value of the angle
    tb_size_t   type;
    gb_float_t  cos_a = gb_stroker_joiner_angle(normal_unit_before, normal_unit_after, &type);

    // the join is nearly line? ignore this join directly
    if (type == GB_STROKER_JOINER_ANGLE_NEAR0) return ;

    // the unit normal vectors and direction
    gb_vector_t before       = *normal_unit_before;
    gb_vector_t after        = *normal_unit_after;

    // done miter
    gb_vector_t     miter;
    tb_bool_t       miter_join = tb_true;
    do
    {
        // the join is nearly 180 degrees? join the bevel
        if (type == GB_STROKER_JOINER_ANGLE_NEAR180)
        {
            // disable the line-to optimization
            is_line_to = tb_false;
 
            // switch to the bevel join
            miter_join = tb_false;
            break;
        }

        // counter-clockwise? reverse it
        tb_bool_t clockwise;
        if (!(clockwise = gb_vector_is_clockwise(normal_unit_before, normal_unit_after)))
        {
            // swap the inner and outer path
            tb_swap(gb_path_ref_t, inner, outer);

            // reverse the before normal
            gb_vector_negate(&before);

            // reverse the after normal
            gb_vector_negate(&after);
        }

        /* right angle? done the more faster and accuracy miter
         *
         * .       .
         * .     .
         * .   . L
         * . .
         * . . . . .
         *     R
         *
         * if (M = L / R >= sqrt(2)) miter
         * if (1 / m <= 1 / sqrt(2)) miter
         */
        if (gb_ez(cos_a) && miter_invert <= GB_ONEOVER_SQRT2)
        {
            gb_vector_make(&miter, gb_mul(before.x + after.x, radius), gb_mul(before.y + after.y, radius));
            break;
        }

        /* compute the cos(a/2)
         *
         * cos(a/2) = sqrt((1 + cos(a)) / 2)
         */
        gb_float_t cos_half_a = gb_sqrt(gb_rsh(GB_ONE + cos_a, 1));

        /* limit the miter length
         *
         * if (L / R > M) strip
         * if (L > M * R) strip
         * if (R / cos(a/2) > M * R) strip
         * if (1 / cos(a/2) > M) strip
         * if (1 / M > cos(a/2)) strip
         */
        if (miter_invert > cos_half_a) 
        {
            // disable the line-to optimization
            is_line_to = tb_false;
 
            // switch to the bevel join
            miter_join = tb_false;
            break;
        }

        /* compute the miter length
         *
         * L = R / cos(a/2)
         */
        gb_float_t length = gb_div(radius, cos_half_a);

        // compute the miter vector
        if (type == GB_STROKER_JOINER_ANGLE_OBTUSE)
        {
            /* compute the more accurate miter vector for the obtuse angle
             *
             *                              miter
             *                               .
             *            after.rotate(ccw) .
             *                          .  .  . before.rotate(cw)
             *                          . . .
             *                          . .
             *                          .
             *                        . .
             *                      . . .
             *          before    .  .  .
             *              \   .   .   .
             *                .    .    .
             *              .   .angle  .
             *            . . . . c . . . -> after
             *          .   .       .   .
             *        .     .         . .
             *      .       .       .   .
             *    .         .     .     .
             *  .           .   .       .
             *              . .         .
             *              .           .
             *            . .           .
             *          .   .           .
             *              .           .
             *
             * miter = before.rotate(cw) + after.rotate(ccw)
             */
            gb_vector_make(&miter, after.y - before.y, before.x - after.x);
            if (!clockwise) gb_vector_negate(&miter);
        }
        else gb_vector_make(&miter, before.x + after.x, before.y + after.y);
        gb_vector_length_set(&miter, length);

    } while (0);

    // scale the after normal
    gb_vector_scale(&after, radius);

    // join the outer contour
    if (miter_join) 
    {
        if (is_line_to_prev)
        {
            // ignore one point for optimization if join the previous line-to contour
            gb_point_t last;
            gb_point_make(&last, center->x + miter.x, center->y + miter.y);
            gb_path_last_set(outer, &last);
        }
        else gb_path_line2_to(outer, center->x + miter.x, center->y + miter.y);
    }

    // ignore one point for optimization if join the line-to contour
    if (!is_line_to) gb_path_line2_to(outer, center->x + after.x, center->y + after.y);

    // join the inner contour
    gb_stroker_joiner_inner(inner, center, &after);
}
static tb_void_t gb_stroker_joiner_round(gb_path_ref_t inner, gb_path_ref_t outer, gb_point_ref_t center, gb_float_t radius, gb_vector_ref_t normal_unit_before, gb_vector_ref_t normal_unit_after, gb_float_t miter_invert, tb_bool_t is_line_to_prev, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(inner && outer && center && normal_unit_before && normal_unit_after);

    // compute the angle type
    tb_size_t type;
    gb_stroker_joiner_angle(normal_unit_before, normal_unit_after, &type);

    // the join is nearly line? ignore this join directly
    if (type == GB_STROKER_JOINER_ANGLE_NEAR0) return ;

    // the unit normal vectors and direction
    gb_vector_t start       = *normal_unit_before;
    gb_vector_t stop        = *normal_unit_after;
    tb_size_t   direction   = GB_ROTATE_DIRECTION_CW;

    // counter-clockwise? reverse it
    if (!gb_vector_is_clockwise(normal_unit_before, normal_unit_after))
    {
        // swap the inner and outer path
        tb_swap(gb_path_ref_t, inner, outer);

        // reverse the start normal
        gb_vector_negate(&start);

        // reverse the stop normal
        gb_vector_negate(&stop);

        // reverse direction
        direction = GB_ROTATE_DIRECTION_CCW;
    }
 
    // init matrix
    gb_matrix_t matrix;
    gb_matrix_init_scale(&matrix, radius, radius);
    gb_matrix_translate_lhs(&matrix, center->x, center->y);

    // join the outer contour
    gb_geometry_make_arc2(&start, &stop, &matrix, direction, gb_stroker_joiner_outer, outer);

    // join the inner contour
    gb_vector_scale(&stop, radius);
    gb_stroker_joiner_inner(inner, center, &stop);
}
static tb_void_t gb_stroker_joiner_bevel(gb_path_ref_t inner, gb_path_ref_t outer, gb_point_ref_t center, gb_float_t radius, gb_vector_ref_t normal_unit_before, gb_vector_ref_t normal_unit_after, gb_float_t miter_invert, tb_bool_t is_line_to_prev, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(inner && outer && center && normal_unit_before && normal_unit_after);

    /* the after normal
     *
     *                      normal_before
     *            outer          |
     * . . . . . . . . . . . . . o1
     * .                         . .
     * .           -->      i2 . c . o2 -> normal_after
     * .                     .   .   .
     * . . . . . . . . . . . . . i1  .
     *            inner      .       .
     *                       .       .
     *                       .       .
     *                       .       .
     *                 inner .       . outer
     *                       .       .
     *                       .       .
     *                       .       .
     *                       .       .
     *
     *
     * outer: o1 => o2
     * inner: i1 => c = > i2
     */
    gb_vector_t normal_after;
    gb_vector_scale2(normal_unit_after, &normal_after, radius);

    // counter-clockwise? reverse it
    if (!gb_vector_is_clockwise(normal_unit_before, normal_unit_after))
    {
        // swap the inner and outer path
        tb_swap(gb_path_ref_t, inner, outer);

        // reverse the after normal
        gb_vector_negate(&normal_after);
    }

    // join the outer contour
    gb_path_line2_to(outer, center->x + normal_after.x, center->y + normal_after.y);

    // join the inner contour
    gb_stroker_joiner_inner(inner, center, &normal_after);
}
static tb_bool_t gb_stroker_enter_to(gb_stroker_impl_t* impl, gb_point_ref_t point, gb_vector_ref_t normal, gb_vector_ref_t normal_unit, tb_bool_t is_line_to)
{
    // check
    tb_assert_abort(impl && point && normal && normal_unit);
    tb_assert_abort(impl->segment_count >= 0);

    // the radius
    gb_float_t radius = impl->radius;
    tb_assert_and_check_return_val(gb_bz(radius), tb_false);

    /* compute the unit normal vector
     *                              
     *        ---------------------->  normal
     *       |  radius   |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *       |           |           |
     *      \|/         \|/         \|/
     *    inner         line        outer
     *
     */
    gb_vector_make(normal_unit, point->x - impl->point_prev.x, point->y - impl->point_prev.y);
    if (!gb_vector_normalize(normal_unit)) 
    {
        // failed
        tb_assert_abort(0);
        return tb_false;
    }
    gb_vector_rotate(normal_unit, GB_ROTATE_DIRECTION_CCW);

    // compute the normal vector
    gb_vector_scale2(normal_unit, normal, radius);

    // body?
    if (impl->segment_count > 0)
    {
        // check
        tb_assert_abort(impl->joiner);

        // join it
        impl->joiner(impl->path_inner, impl->path_outer, &impl->point_prev, radius, &impl->normal_unit_prev, normal_unit, impl->miter_invert, impl->is_line_to_prev, is_line_to);
    }
    // start?
    else
    {
        // save the first point of the outer contour
        gb_point_make(&impl->outer_first, impl->point_prev.x + normal->x, impl->point_prev.y + normal->y);

        // save the first normal
        impl->normal_first = *normal;

        // save the first unit normal
        impl->normal_unit_first = *normal_unit;

        // save the first line-to state?
        impl->is_line_to_first = is_line_to;

        // move to the start point for the inner and outer path
        gb_path_move_to(impl->path_outer, &impl->outer_first);
        gb_path_move2_to(impl->path_inner, impl->point_prev.x - normal->x, impl->point_prev.y - normal->y);
    }

    // update the previous line-to state?
    impl->is_line_to_prev = is_line_to;

    // ok
    return tb_true;
}
static tb_void_t gb_stroker_leave_to(gb_stroker_impl_t* impl, gb_point_ref_t point, gb_vector_ref_t normal, gb_vector_ref_t normal_unit)
{
    // check
    tb_assert_abort(impl && point && normal && normal_unit);

    // update the previous point
    impl->point_prev = *point;

    // update the previous normal
    impl->normal_prev = *normal;

    // update the previous unit normal
    impl->normal_unit_prev = *normal_unit;

    // update the segment count
    impl->segment_count++;
}
static tb_void_t gb_stroker_finish(gb_stroker_impl_t* impl, tb_bool_t closed)
{
    // check
    tb_assert_abort(impl && impl->path_inner && impl->path_outer);
    tb_assert_abort(impl->capper && impl->joiner);

    // exists contour now?
    if (impl->segment_count > 0)
    {
        // closed?
        if (closed)
        {
            // join it
            impl->joiner(impl->path_inner, impl->path_outer, &impl->point_prev, impl->radius, &impl->normal_unit_prev, &impl->normal_unit_first, impl->miter_invert, impl->is_line_to_prev, impl->is_line_to_first);

            // close the outer contour
            gb_path_clos(impl->path_outer);

            /* add the inner contour in reverse order to the outer path
             *
             *              -->
             * . . . . . . . . .
             * .               .
             * .   . . . . .   .
             * .   .       .   .
             * .   .       .   .
             * .   . inner .   .
             * .   . . . . x   .
             * .               .
             * . . . . . . . . x outer 
             * <--
             */
            gb_point_t inner_last;
            gb_path_last(impl->path_inner, &inner_last);
            gb_path_move_to(impl->path_outer, &inner_last);
            gb_path_rpath_to(impl->path_outer, impl->path_inner);
            gb_path_clos(impl->path_outer);

        }
        /* add caps to the start and end point
         *
         *                    start cap
         *             ------------------------>
         *                                    
         *                       normal
         *              ----------------------> first outer
         *             |  radius   |           |
         *             |           |           |
         *             |           |           |
         *             |           |           |
         *             |           |           |
         * reverse add |           |           |
         *             |           |           |
         *             |           |           |
         *             |           |           |
         *             |           |           |
         *             |           |           |
         * last inner \|/         \|/         \|/
         *          inner        center       outer
         *
         *             <------------------------
         *                     end cap
         */
        else
        {
            // cap the end point
            gb_point_t inner_last;
            gb_path_last(impl->path_inner, &inner_last);
            impl->capper(impl->path_outer, &impl->point_prev, &inner_last, &impl->normal_prev, impl->is_line_to_prev);

            // add the inner contour in reverse order to the outer path
            gb_path_rpath_to(impl->path_outer, impl->path_inner);

            // cap the start point
            gb_vector_t normal_first;
            gb_vector_negate2(&impl->normal_first, &normal_first);
            impl->capper(impl->path_outer, &impl->point_first, &impl->outer_first, &normal_first, impl->is_line_to_first);

            // close the outer contour
            gb_path_clos(impl->path_outer);
        }
    }

    // finish it
    impl->segment_count = -1;

    // clear the inner path for reusing it
    gb_path_clear(impl->path_inner);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_stroker_ref_t gb_stroker_init()
{
    // done
    tb_bool_t           ok = tb_false;
    gb_stroker_impl_t*  impl = tb_null;
    do
    {
        // make stroker
        impl = tb_malloc0_type(gb_stroker_impl_t);
        tb_assert_and_check_break(impl);

        // init stroker
        impl->cap               = GB_PAINT_STROKE_CAP_BUTT;
        impl->join              = GB_PAINT_STROKE_JOIN_MITER;
        impl->miter             = GB_STROKER_DEFAULT_MITER;
        impl->radius            = 0;
        impl->segment_count     = -1;
        impl->capper            = gb_stroker_capper_butt;
        impl->joiner            = gb_stroker_joiner_miter;
        impl->miter_invert      = gb_invert(GB_STROKER_DEFAULT_MITER);
        impl->is_line_to_prev   = tb_false;
        impl->is_line_to_first  = tb_false;

        // init the outer path
        impl->path_outer = gb_path_init();
        tb_assert_and_check_break(impl->path_outer);
    
        // init the inner path
        impl->path_inner = gb_path_init();
        tb_assert_and_check_break(impl->path_inner);
   
        // init the other path
        impl->path_other = gb_path_init();
        tb_assert_and_check_break(impl->path_other);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_stroker_exit((gb_stroker_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_stroker_ref_t)impl;
}
tb_void_t gb_stroker_exit(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl);

    // exit the other path
    if (impl->path_other) gb_path_exit(impl->path_other);
    impl->path_other = tb_null;

    // exit the inner path
    if (impl->path_inner) gb_path_exit(impl->path_inner);
    impl->path_inner = tb_null;

    // exit the outer path
    if (impl->path_outer) gb_path_exit(impl->path_outer);
    impl->path_outer = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_stroker_clear(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl);

    // clear it
    impl->cap               = GB_PAINT_STROKE_CAP_BUTT;
    impl->join              = GB_PAINT_STROKE_JOIN_MITER;
    impl->miter             = GB_STROKER_DEFAULT_MITER;
    impl->radius            = 0;
    impl->segment_count     = -1;
    impl->capper            = gb_stroker_capper_butt;
    impl->joiner            = gb_stroker_joiner_miter;
    impl->miter_invert      = gb_invert(GB_STROKER_DEFAULT_MITER);
    impl->is_line_to_prev   = tb_false;
    impl->is_line_to_first  = tb_false;

    // clear the other path
    if (impl->path_other) gb_path_clear(impl->path_other);

    // clear the inner path
    if (impl->path_inner) gb_path_clear(impl->path_inner);

    // clear the outer path
    if (impl->path_outer) gb_path_clear(impl->path_outer);
}
tb_void_t gb_stroker_apply_paint(gb_stroker_ref_t stroker, gb_paint_ref_t paint)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && paint);

    // the width
    gb_float_t width = gb_paint_stroke_width(paint);
    tb_assert_abort(!gb_lz(width));

    // the miter
    gb_float_t miter = gb_paint_stroke_miter(paint);

    // set the cap
    impl->cap = gb_paint_stroke_cap(paint);

    // set the join
    impl->join = gb_paint_stroke_join(paint);

    // set the radius
    impl->radius = gb_rsh(width, 1);

    // set the invert miter limit
    if (impl->miter != miter)
    {
        impl->miter_invert = 0;
        if (impl->join == GB_PAINT_STROKE_JOIN_MITER)
        {
            if (!gb_l1(miter)) impl->join = GB_PAINT_STROKE_JOIN_BEVEL;
            else impl->miter_invert = gb_invert(miter);
        }
    }

    // set the miter limit
    impl->miter = miter;

    // the cappers
    static gb_stroker_capper_t s_cappers[] = 
    {
        gb_stroker_capper_butt
    ,   gb_stroker_capper_round
    ,   gb_stroker_capper_square
    };
    tb_assert_abort(impl->cap < tb_arrayn(s_cappers));

    // the joiners
    static gb_stroker_joiner_t s_joiners[] = 
    {
        gb_stroker_joiner_miter
    ,   gb_stroker_joiner_round
    ,   gb_stroker_joiner_bevel
    };
    tb_assert_abort(impl->join < tb_arrayn(s_joiners));

    // set capper
    impl->capper = s_cappers[impl->cap];

    // set joiner
    impl->joiner = s_joiners[impl->join];
}
tb_void_t gb_stroker_clos(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl);

    // close this contour
    gb_stroker_finish(impl, tb_true);
}
tb_void_t gb_stroker_move_to(gb_stroker_ref_t stroker, gb_point_ref_t point)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_inner && impl->path_outer && point);

    // finish the current contour first
    if (impl->segment_count > 0) gb_stroker_finish(impl, tb_false);

    // start a new contour
    impl->segment_count = 0;

    // save the first point
    impl->point_first = *point;

    // save the previous point
    impl->point_prev = *point;
}
tb_void_t gb_stroker_line_to(gb_stroker_ref_t stroker, gb_point_ref_t point)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && point);

    // only be a point?
    if (gb_point_equal(&impl->point_prev, point)) return ;

    // enter-to 
    gb_vector_t normal;
    gb_vector_t normal_unit;
    if (!gb_stroker_enter_to(impl, point, &normal, &normal_unit, tb_true)) return ;

    // line to the point for the inner and outer path
    gb_path_line2_to(impl->path_outer, point->x + normal.x, point->y + normal.y);
    gb_path_line2_to(impl->path_inner, point->x - normal.x, point->y - normal.y);

    // leave-to
    gb_stroker_leave_to(impl, point, &normal, &normal_unit);
}
tb_void_t gb_stroker_quad_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl, gb_point_ref_t point)
{
    // TODO
    tb_trace_noimpl();
}
tb_void_t gb_stroker_cube_to(gb_stroker_ref_t stroker, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point)
{
    // TODO
    tb_trace_noimpl();
}
tb_void_t gb_stroker_add_path(gb_stroker_ref_t stroker, gb_path_ref_t path)
{
    // done
    tb_for_all_if (gb_path_item_ref_t, item, path, item)
    {
        switch (item->code)
        {
        case GB_PATH_CODE_MOVE:
            gb_stroker_move_to(stroker, &item->points[0]);
            break;
        case GB_PATH_CODE_LINE:
            gb_stroker_line_to(stroker, &item->points[1]);
            break;
        case GB_PATH_CODE_QUAD:
            gb_stroker_quad_to(stroker, &item->points[1], &item->points[2]);
            break;
        case GB_PATH_CODE_CUBE:
            gb_stroker_cube_to(stroker, &item->points[1], &item->points[2], &item->points[3]);
            break;
        case GB_PATH_CODE_CLOS:
            gb_stroker_clos(stroker);
            break;
        default:
            // trace
            tb_trace_e("invalid code: %lu", item->code);
            break;
        }
    }
}
tb_void_t gb_stroker_add_rect(gb_stroker_ref_t stroker, gb_rect_ref_t rect)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_other && rect);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // the width
    gb_float_t width = gb_lsh(radius, 1);

    // init the inner rect
    gb_rect_t rect_inner = *rect;
    if (rect_inner.w > width && rect_inner.h > width)
    {
        // make the inner rect
        gb_rect_deflate(&rect_inner, radius, radius);

        // add the inner rect to the other path
        gb_path_add_rect(impl->path_other, &rect_inner, GB_ROTATE_DIRECTION_CW);
    }

    // init the outer rect
    gb_rect_t rect_outer = *rect;

    // make the outer rect
    gb_rect_inflate(&rect_outer, radius, radius);

    // the join
    tb_size_t join = impl->join;
    if (join == GB_PAINT_STROKE_JOIN_MITER && impl->miter < GB_SQRT2)
        join = GB_PAINT_STROKE_JOIN_BEVEL;

    /* add the outer rect to the other path
     *
     * ------------------------------ miter join
     *                    .        . |
     *                      .    L   |
     *            bevel join  .      |
     *                        | .    |  
     *                        |   .  |
     * -----------------|     |      |
     *                  |        R   |
     *                  |            |
     *                  |            |
     *                  |            |
     *                  |            |
     *                  | W = R * 2  |
     * 
     * W: width
     * R: radius
     * miter_limit = L / R > 1
     */
    switch (join)
    {
    case GB_PAINT_STROKE_JOIN_MITER:
        {
            // add miter rect
            gb_path_add_rect(impl->path_other, &rect_outer, GB_ROTATE_DIRECTION_CCW);
        }
        break;
    case GB_PAINT_STROKE_JOIN_BEVEL:
        {
            // the bounds
            gb_float_t x = rect_outer.x;
            gb_float_t y = rect_outer.y;
            gb_float_t w = rect_outer.w;
            gb_float_t h = rect_outer.h;

            // add bevel rect by counter-clockwise
            gb_path_move2_to(impl->path_other, x,               y + radius);
            gb_path_line2_to(impl->path_other, x,               y + h - radius);
            gb_path_line2_to(impl->path_other, x + radius,      y + h);
            gb_path_line2_to(impl->path_other, x + w - radius,  y + h);
            gb_path_line2_to(impl->path_other, x + w,           y + h - radius);
            gb_path_line2_to(impl->path_other, x + w,           y + radius);
            gb_path_line2_to(impl->path_other, x + w - radius,  y);
            gb_path_line2_to(impl->path_other, x + radius,      y);
            gb_path_clos(impl->path_other);
        }
        break;
    case GB_PAINT_STROKE_JOIN_ROUND:
        {
            // add round rect
            gb_path_add_round_rect2(impl->path_other, &rect_outer, radius, radius, GB_ROTATE_DIRECTION_CCW);
        }
        break;
    default:
        tb_trace_e("unknown join: %lu", impl->join);
        break;
    }
}
tb_void_t gb_stroker_add_circle(gb_stroker_ref_t stroker, gb_circle_ref_t circle)
{
    // check
    tb_assert_and_check_return(circle);

    // make ellipse
    gb_ellipse_t ellipse;
    gb_ellipse_make(&ellipse, circle->c.x, circle->c.y, circle->r, circle->r);

    // add ellipse
    gb_stroker_add_ellipse(stroker, &ellipse);
}
tb_void_t gb_stroker_add_ellipse(gb_stroker_ref_t stroker, gb_ellipse_ref_t ellipse)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_other && ellipse);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // init the inner ellipse
    gb_ellipse_t ellipse_inner = *ellipse;
    if (ellipse_inner.rx > radius && ellipse_inner.ry > radius)
    {
        // make the inner ellipse
        ellipse_inner.rx -= radius;
        ellipse_inner.ry -= radius;

        // add the inner ellipse to the other path
        gb_path_add_ellipse(impl->path_other, &ellipse_inner, GB_ROTATE_DIRECTION_CW);
    }

    // init the outer ellipse
    gb_ellipse_t ellipse_outer = *ellipse;

    // make the outer ellipse
    ellipse_outer.rx += radius;
    ellipse_outer.ry += radius;

    // add the inner and outer ellipse to the other path
    gb_path_add_ellipse(impl->path_other, &ellipse_outer, GB_ROTATE_DIRECTION_CCW);
}
tb_void_t gb_stroker_add_lines(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(points && count && !(count & 0x1));

    // done
    tb_size_t index;
    for (index = 0; index < count; index += 2)
    {
        gb_stroker_move_to(stroker, points + index);
        gb_stroker_line_to(stroker, points + index + 1);
    }
}
tb_void_t gb_stroker_add_points(gb_stroker_ref_t stroker, gb_point_ref_t points, tb_size_t count)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return(impl && impl->path_other && points && count);

    // the radius
    gb_float_t radius = impl->radius;
    tb_check_return(gb_bz(radius));

    // make the stroked path
    switch (impl->cap)
    {
    case GB_PAINT_STROKE_CAP_ROUND:
        {
            // done
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_circle_t     circle;
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make circle
                gb_circle_make(&circle, point->x, point->y, radius);

                // add circle to the other path
                gb_path_add_circle(impl->path_other, &circle, GB_ROTATE_DIRECTION_CW);
            }
        }
        break;
    case GB_PAINT_STROKE_CAP_BUTT:
    case GB_PAINT_STROKE_CAP_SQUARE:
    default:
        {
            // done
            gb_rect_t       rect;
            tb_size_t       index;
            gb_point_ref_t  point;
            gb_float_t      width = gb_lsh(radius, 1);
            for (index = 0; index < count; index++)
            {
                // the point
                point = points + index;

                // make rect
                gb_rect_make(&rect, point->x - radius, point->y - radius, width, width);

                // add rect to the other path
                gb_path_add_rect(impl->path_other, &rect, GB_ROTATE_DIRECTION_CW);
            }
        }
        break;
    }
}
tb_void_t gb_stroker_add_polygon(gb_stroker_ref_t stroker, gb_polygon_ref_t polygon)
{
    // check
    tb_assert_and_check_return(polygon && polygon->points && polygon->counts);

    // done
    gb_point_ref_t  first = tb_null;
    gb_point_ref_t  point = tb_null;
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    tb_size_t       index = 0;
    while (index < count)
    {
        // the point
        point = points++;
        
        // first point?
        if (!index) 
        {
            gb_stroker_move_to(stroker, point);
            first = point;
        }
        else gb_stroker_line_to(stroker, point);

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // close path
            if (first && first->x == point->x && first->y == point->y) gb_stroker_clos(stroker);

            // next
            count = *counts++;
            index = 0;
        }
    }
}
gb_path_ref_t gb_stroker_done(gb_stroker_ref_t stroker)
{
    // check
    gb_stroker_impl_t* impl = (gb_stroker_impl_t*)stroker;
    tb_assert_and_check_return_val(impl, tb_null);

    // finish the current contour first
    if (impl->segment_count > 0) gb_stroker_finish(impl, tb_false);

    // exists the other path? merge it
    if (impl->path_other && !gb_path_null(impl->path_other))
    {
        // add the other path
        gb_path_add_path(impl->path_outer, impl->path_other);

        // clear the other path
        gb_path_clear(impl->path_other);
    }

    // the stroked path
    return impl->path_outer;
}
gb_path_ref_t gb_stroker_done_path(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_path_ref_t path)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // attempt to add hint first
    if (!gb_stroker_add_hint(stroker, gb_path_hint(path)))
    {
        // add path to the stroker
        gb_stroker_add_path(stroker, path);
    }

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_lines(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add lines to the stroker
    gb_stroker_add_lines(stroker, points, count);

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_points(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_point_ref_t points, tb_size_t count)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // add points to the stroker
    gb_stroker_add_points(stroker, points, count);

    // done the stroker
    return gb_stroker_done(stroker);
}
gb_path_ref_t gb_stroker_done_polygon(gb_stroker_ref_t stroker, gb_paint_ref_t paint, gb_polygon_ref_t polygon, gb_shape_ref_t hint)
{
    // clear the stroker
    gb_stroker_clear(stroker);

    // apply paint to the stroker
    gb_stroker_apply_paint(stroker, paint);

    // attempt to add hint first
    if (!gb_stroker_add_hint(stroker, hint))
    {
        // add polygon to the stroker
        gb_stroker_add_polygon(stroker, polygon);
    }

    // done the stroker
    return gb_stroker_done(stroker);
}
