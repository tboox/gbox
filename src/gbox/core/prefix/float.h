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
 * @file        float.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_FLOAT_H
#define GB_CORE_PREFIX_FLOAT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

#ifdef GB_CONFIG_FLOAT_FIXED

// constant
#define GB_ONE                  TB_FIXED_ONE
#define GB_TWO                  (TB_FIXED_ONE + TB_FIXED_ONE)
#define GB_HALF                 TB_FIXED_HALF
#define GB_MAF                  TB_FIXED_MAX
#define GB_MIF                  TB_FIXED_MIN
#define GB_NAN                  TB_FIXED_NAN
#define GB_INF                  TB_FIXED_INF
#define GB_PI                   TB_FIXED_PI
#define GB_NEAR0                TB_FIXED_NEAR0
#define GB_SQRT2                TB_FIXED_SQRT2      //!< sqrt(2)
#define GB_TAN_PIOVER8          (27146)             //!< tan(pi/8)
#define GB_SQRT2_OVER2          (46341)             //!< sqrt(2)/2
#define GB_PIOVER180            (1143)              //<! pi/180
#define GB_180OVERPI            (3754936)           //!< 180/pi
#define GB_DEGREE_45            (2949120)
#define GB_DEGREE_90            (5898240)
#define GB_DEGREE_180           (11796480)
#define GB_DEGREE_360           (23592960)

// conversion
#ifdef TB_CONFIG_TYPE_FLOAT
#   define gb_float_to_tb(x)    tb_fixed_to_float(x)
#   define tb_float_to_gb(x)    tb_float_to_fixed(x)
#endif

#define gb_long_to_float(x)     tb_long_to_fixed(x)
#define gb_float_to_long(x)     tb_fixed_to_long(x)

#define gb_fixed_to_float(x)    (x)
#define gb_float_to_fixed(x)    (x)

#define gb_fixed6_to_float(x)   tb_fixed6_to_fixed(x)
#define gb_float_to_fixed6(x)   tb_fixed_to_fixed6(x)

#define gb_fixed30_to_float(x)  tb_fixed30_to_fixed(x)
#define gb_float_to_fixed30(x)  tb_fixed_to_fixed30(x)

#define gb_degree_to_radian(x)  (tb_fixed_mul((x), GB_PIOVER180))
#define gb_radian_to_degree(x)  (tb_fixed_mul((x), GB_180OVERPI))

// round
#define gb_round(x)             tb_fixed_round(x)
#define gb_ceil(x)              tb_fixed_ceil(x)
#define gb_floor(x)             tb_fixed_floor(x)

// is finite?
#define gb_isfinite(x)          ((x) != TB_FIXED_NAN)

// functions
#define gb_fabs(x)              tb_fixed_abs(x)
#define gb_avg(x, y)            tb_fixed_avg(x, y)
#define gb_mul(x, y)            tb_fixed_mul(x, y)
#define gb_div(x, y)            tb_fixed_div(x, y)
#define gb_lsh(x, y)            tb_fixed_lsh(x, y)
#define gb_rsh(x, y)            tb_fixed_rsh(x, y)
#define gb_imul(x, y)           tb_fixed_imul(x, y)
#define gb_idiv(x, y)           tb_fixed_idiv(x, y)
#define gb_imuldiv(x, y, z)     tb_fixed_imuldiv(x, y, z)
#define gb_imulsub(x, y, z)     tb_fixed_imulsub(x, y, z)
#define gb_invert(x)            tb_fixed_invert(x)
#define gb_sqre(x)              tb_fixed_sqre(x)
#define gb_sqrt(x)              tb_fixed_sqrt(x)
#define gb_sin(x)               tb_fixed_sin(x)
#define gb_cos(x)               tb_fixed_cos(x)
#define gb_sincos(x, s, c)      tb_fixed_sincos(x, s, c)
#define gb_tan(x)               tb_fixed_tan(x)
#define gb_asin(x)              tb_fixed_asin(x)
#define gb_acos(x)              tb_fixed_acos(x)
#define gb_atan(x)              tb_fixed_atan(x)
#define gb_atan2(y, x)          tb_fixed_atan2(y, x)
#define gb_exp(x)               tb_fixed_exp(x)
#define gb_exp1(x)              tb_fixed_exp1(x)
#define gb_expi(x)              tb_fixed_expi(x)

// comparison
#define gb_nz(x)                (x)
#define gb_ez(x)                (!(x))
#define gb_lz(x)                ((x) < 0)
#define gb_bz(x)                ((x) > 0)

#define gb_n1(x)                (GB_ONE != (x))
#define gb_e1(x)                (GB_ONE == (x))
#define gb_l1(x)                ((x) < GB_ONE)
#define gb_b1(x)                ((x) > GB_ONE)

#elif defined(TB_CONFIG_TYPE_FLOAT)

// constant
#define GB_ONE                  (1.0f)
#define GB_TWO                  (2.0f)
#define GB_HALF                 (0.5f)
#define GB_MAF                  TB_MAF
#define GB_MIF                  TB_MIF
#define GB_NAN                  TB_NAN
#define GB_INF                  TB_INF
#define GB_PI                   TB_PI
#define GB_NEAR0                (1.0f / (1 << 12))
#define GB_SQRT2                (1.414213562f)      //!< sqrt(2)
#define GB_TAN_PIOVER8          (0.414213562f)      //!< tan(pi/8)
#define GB_SQRT2_OVER2          (0.707106781f)      //!< sqrt(2)/2
#define GB_PIOVER180            (0.017453293f)      //<! pi/180
#define GB_180OVERPI            (57.29577951f)      //!< 180/pi
#define GB_DEGREE_45            (45.0f)
#define GB_DEGREE_90            (90.0f)
#define GB_DEGREE_180           (180.0f)
#define GB_DEGREE_360           (360.0f)

// conversion
#define gb_float_to_tb(x)       (x)
#define tb_float_to_gb(x)       (x)

#define gb_long_to_float(x)     (tb_float_t)(x)
#define gb_float_to_long(x)     (tb_long_t)(x)

#define gb_fixed_to_float(x)    tb_fixed_to_float(x)
#define gb_float_to_fixed(x)    tb_float_to_fixed(x)

#define gb_fixed6_to_float(x)   tb_fixed6_to_float(x)
#define gb_float_to_fixed6(x)   tb_float_to_fixed6(x)

#define gb_fixed30_to_float(x)  tb_fixed30_to_float(x)
#define gb_float_to_fixed30(x)  tb_float_to_fixed30(x)

#define gb_degree_to_radian(x)  ((x) * GB_PIOVER180)
#define gb_radian_to_degree(x)  ((x) * GB_180OVERPI)

// round
#define gb_round(x)             tb_round(x)
#define gb_ceil(x)              tb_ceil(x)
#define gb_floor(x)             tb_floor(x)

// is finite?
#define gb_isfinite(x)          tb_isfinf(x)

// functions
#define gb_fabs(x)              tb_fabs(x)
#define gb_avg(x, y)            (((x) + (y)) * 0.5)
#define gb_lsh(x, y)            ((x) * (tb_float_t)(1 << (y)))
#define gb_rsh(x, y)            ((x) / (tb_float_t)(1 << (y)))
#define gb_mul(x, y)            ((x) * (y))
#define gb_div(x, y)            ((x) / (y))
#define gb_imul(x, y)           ((x) * (tb_float_t)(y))
#define gb_idiv(x, y)           ((x) / (tb_float_t)(y))
#define gb_imuldiv(x, y, z)     (((x) * (tb_float_t)(y)) / (tb_float_t)(z))
#define gb_imulsub(x, y, z)     (((x) * (tb_float_t)(y)) - (tb_float_t)(z))
#define gb_invert(x)            (1.0f / (x))
#define gb_sqre(x)              ((x) * (x))
#define gb_sqrt(x)              tb_sqrtf(x)
#define gb_sin(x)               tb_sinf(x)
#define gb_cos(x)               tb_cosf(x)
#define gb_sincos(x, s, c)      tb_sincosf(x, s, c)
#define gb_tan(x)               tb_tanf(x)
#define gb_asin(x)              tb_asinf(x)
#define gb_acos(x)              tb_acosf(x)
#define gb_atan(x)              tb_atanf(x)
#define gb_atan2(y, x)          tb_atan2f(y, x)
#define gb_exp(x)               tb_expf(x)
#define gb_exp1(x)              tb_exp1f(x)
#define gb_expi(x)              tb_expif(x)

// comparison
#define gb_nz(x)                gb_float_nz(x)
#define gb_ez(x)                (!gb_nz(x))
#define gb_lz(x)                (gb_nz(x) < 0)
#define gb_bz(x)                (gb_nz(x) > 0)

#define gb_n1(x)                ((x) != GB_ONE)
#define gb_e1(x)                ((x) == GB_ONE)
#define gb_l1(x)                ((x) < GB_ONE)
#define gb_b1(x)                ((x) > GB_ONE)

#else
#   error float is not supported.
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */
#ifdef TB_CONFIG_TYPE_FLOAT
static __tb_inline__ tb_long_t gb_float_nz(tb_float_t x)
{
    tb_int32_t y;
    union gb_float_union 
    {
        tb_float_t  f;
        tb_int32_t  i;
    }u;
    u.f = x;
    y = u.i;

    if (y < 0) 
    {
        y &= 0x7fffffff;
        y = -y;
    }
    return y;
}
#endif

#endif

