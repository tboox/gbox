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
 * @file        vector.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "vector"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "vector.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_vector_make(gb_vector_ref_t vector, gb_float_t x, gb_float_t y)
{
    // check
    tb_assert_abort(vector);

    // make it
    vector->x = x;
    vector->y = y;
}
tb_void_t gb_vector_imake(gb_vector_ref_t vector, tb_long_t x, tb_long_t y)
{
    gb_vector_make(vector, gb_long_to_float(x), gb_long_to_float(y));
}
tb_void_t gb_vector_make_from_point(gb_vector_ref_t vector, gb_point_ref_t point)
{
    // check
    tb_assert_abort(vector && point);

    // make it
    *vector = *((gb_vector_ref_t)point);
}
tb_void_t gb_vector_negate(gb_vector_ref_t vector)
{
    // check
    tb_assert_abort(vector);

    // negate it
    vector->x = -vector->x;
    vector->y = -vector->y;
}
tb_void_t gb_vector_rotate(gb_vector_ref_t vector, tb_size_t direction)
{
    // rotate it
    gb_vector_rotate2(vector, vector, direction);
}
tb_void_t gb_vector_rotate2(gb_vector_ref_t vector, gb_vector_ref_t rotated, tb_size_t direction)
{
    // check
    tb_assert_abort(vector && rotated);

    /* rotate it
     *
     * @note using t for avoiding the case of vector == rotated
     */
    if (direction == GB_ROTATE_DIRECTION_CW)
    {
        gb_float_t t = vector->x;
        rotated->x = -vector->y;
        rotated->y = t;
    }
    else
    {
        gb_float_t t = vector->x;
        rotated->x = vector->y;
        rotated->y = -t;
    }
}
tb_void_t gb_vector_scale(gb_vector_ref_t vector, gb_float_t scale)
{
    // scale it
    gb_vector_scale2(vector, vector, scale);
}
tb_void_t gb_vector_scale2(gb_vector_ref_t vector, gb_vector_ref_t scaled, gb_float_t scale)
{
    // check
    tb_assert_abort(vector && scaled);

    // scale it
    scaled->x = gb_mul(vector->x, scale);
    scaled->y = gb_mul(vector->y, scale);
}
gb_float_t gb_vector_length(gb_vector_ref_t vector)
{
    // check
    tb_assert_abort(vector);

    // the dx and dy
    gb_float_t dx = vector->x;
    gb_float_t dy = vector->y;

    // the length
    gb_float_t length = 0;

#ifdef GB_CONFIG_FLOAT_FIXED
    // attempt to compute the length directly
    tb_hong_t dd = ((tb_hong_t)dx * dx + (tb_hong_t)dy * dy) >> 16;
    if (!(dd >> 32)) length = gb_sqrt((tb_long_t)dd);
    else
    {
#   ifdef TB_CONFIG_TYPE_FLOAT
        // compute the length using the double value
        tb_double_t xx      = gb_float_to_tb(dx);
        tb_double_t yy      = gb_float_to_tb(dy);
        tb_float_t  lengthf = (tb_float_t)tb_sqrt(xx * xx + yy * yy);
        length = tb_float_to_gb(lengthf);
#   else
        // trace
        tb_trace_e("cannot compute: %{vector}.length", vector);
        
        // abort it for the debug mode
        tb_assert_abort(0);
#   endif
    }
#else
    // attempt to compute the length directly
    tb_float_t dd = dx * dx + dy * dy;
    if (gb_isfinite(dd)) length = dd;
    else
    {
        // compute the length using the double value
        tb_double_t xx = dx;
        tb_double_t yy = dy;
        length = (tb_float_t)tb_sqrt(xx * xx + yy * yy);
    }
#endif

    // ok?
    return length;
}
