/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        paint.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "paint.h"
#include "shader.h"
#include "impl/stroker.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the default cap
#define GB_PAINT_DEFAULT_CAP                GB_PAINT_STROKE_CAP_BUTT

// the default join
#define GB_PAINT_DEFAULT_JOIN               GB_PAINT_STROKE_JOIN_MITER

// the default rule
#define GB_PAINT_DEFAULT_RULE               GB_PAINT_FILL_RULE_ODD

// the default width
#define GB_PAINT_DEFAULT_WIDTH              GB_ONE

// the default alpha
#define GB_PAINT_DEFAULT_ALPHA              (255)

// the default miter limit
#define GB_PAINT_DEFAULT_MITER              GB_STROKER_DEFAULT_MITER

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the paint impl type
typedef struct __gb_paint_impl_t
{
    // the paint mode
    tb_uint32_t         mode    : 4;

    // the paint flag
    tb_uint32_t         flag    : 4;

    // the stroke cap
    tb_uint32_t         cap     : 4;

    // the stroke join
    tb_uint32_t         join    : 4;

    // the fill rule
    tb_uint32_t         rule    : 1;

    // the paint color
    gb_color_t          color;

    // the paint alpha
    tb_byte_t           alpha;

    // the stroking width
    gb_float_t          width;

    // the miter miter
    gb_float_t          miter;

    // the shader
    gb_shader_ref_t     shader;

}gb_paint_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_paint_ref_t gb_paint_init()
{
    // done
    tb_bool_t           ok = tb_false;
    gb_paint_impl_t*    impl = tb_null;
    do
    {
        // make paint
        impl = tb_malloc0_type(gb_paint_impl_t);
        tb_assert_and_check_break(impl);

        // init paint
        gb_paint_clear((gb_paint_ref_t)impl);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_paint_exit((gb_paint_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_paint_ref_t)impl;
}
tb_void_t gb_paint_exit(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // exit shader
    if (impl->shader) gb_shader_exit(impl->shader);
    impl->shader = tb_null;

    // exit paint
    tb_free(impl);
}
tb_void_t gb_paint_clear(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // clear
    impl->mode          = GB_PAINT_MODE_NONE;
    impl->flag          = GB_PAINT_FLAG_NONE;
    impl->cap           = GB_PAINT_DEFAULT_CAP;
    impl->join          = GB_PAINT_DEFAULT_JOIN;
    impl->rule          = GB_PAINT_DEFAULT_RULE;
    impl->width         = GB_PAINT_DEFAULT_WIDTH;
    impl->color         = GB_COLOR_DEFAULT;
    impl->alpha         = GB_PAINT_DEFAULT_ALPHA;
    impl->miter         = GB_PAINT_DEFAULT_MITER;

    // clear shader
    if (impl->shader) gb_shader_exit(impl->shader);
    impl->shader = tb_null;
}
tb_void_t gb_paint_copy(gb_paint_ref_t paint, gb_paint_ref_t copied)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    gb_paint_impl_t* impl_copied = (gb_paint_impl_t*)copied;
    tb_assert_and_check_return(impl && impl_copied);

    // refn++
    if (impl_copied->shader) gb_shader_inc(impl_copied->shader);

    // refn--
    if (impl->shader) gb_shader_dec(impl->shader);

    // copy
    tb_memcpy(impl, impl_copied, sizeof(gb_paint_impl_t));
}
tb_size_t gb_paint_mode(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_MODE_NONE);

    // the mode
    return impl->mode;
}
tb_void_t gb_paint_mode_set(gb_paint_ref_t paint, tb_size_t mode)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->mode = (tb_uint32_t)mode;
}
tb_size_t gb_paint_flag(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_FLAG_NONE);

    // modify flag for quality
    if (gb_quality() > GB_QUALITY_LOW) impl->flag |= (GB_PAINT_FLAG_ANTIALIASING | GB_PAINT_FLAG_FILTER_BITMAP);
    else impl->flag &= ~(GB_PAINT_FLAG_ANTIALIASING | GB_PAINT_FLAG_FILTER_BITMAP);

    // the flag
    return impl->flag;
}
tb_void_t gb_paint_flag_set(gb_paint_ref_t paint, tb_size_t flag)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->flag = flag;
}
gb_color_t gb_paint_color(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_COLOR_DEFAULT);

    // the color
    return impl->color;
}
tb_void_t gb_paint_color_set(gb_paint_ref_t paint, gb_color_t color)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->color = color;
}
tb_byte_t gb_paint_alpha(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, 0xff);

    // the alpha
    return impl->alpha;
}
tb_void_t gb_paint_alpha_set(gb_paint_ref_t paint, tb_byte_t alpha)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->alpha = alpha;
}
gb_float_t gb_paint_stroke_width(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_DEFAULT_WIDTH);

    // the width
    return impl->width;
}
tb_void_t gb_paint_stroke_width_set(gb_paint_ref_t paint, gb_float_t width)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl && width >= 0);

    // done
    impl->width = width;
}
tb_size_t gb_paint_stroke_cap(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_DEFAULT_CAP);

    // the cap
    return impl->cap;
}
tb_void_t gb_paint_stroke_cap_set(gb_paint_ref_t paint, tb_size_t cap)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->cap = (tb_uint32_t)cap;
}
tb_size_t gb_paint_stroke_join(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_DEFAULT_JOIN);

    // the join
    return impl->join;
}
tb_void_t gb_paint_stroke_join_set(gb_paint_ref_t paint, tb_size_t join)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->join = (tb_uint32_t)join;
}
gb_float_t gb_paint_stroke_miter(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_DEFAULT_MITER);

    // the miter 
    return impl->miter;
}
tb_void_t gb_paint_stroke_miter_set(gb_paint_ref_t paint, gb_float_t miter)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->miter = miter;
}
tb_size_t gb_paint_fill_rule(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_DEFAULT_RULE);

    // the rule
    return impl->rule;
}
tb_void_t gb_paint_fill_rule_set(gb_paint_ref_t paint, tb_size_t rule)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->rule = (tb_uint32_t)rule;
}
gb_shader_ref_t gb_paint_shader(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, tb_null);

    // the shader
    return impl->shader;
}
tb_void_t gb_paint_shader_set(gb_paint_ref_t paint, gb_shader_ref_t shader)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // ref--
    if (impl->shader) gb_shader_dec(impl->shader);

    // assign
    impl->shader = shader;

    // ref++
    if (shader) gb_shader_inc(shader);
}
