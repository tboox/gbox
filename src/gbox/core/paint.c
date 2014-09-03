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
 * @file        paint.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "paint.h"
#include "shader.h"

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

    // the stok cap
    tb_uint32_t         cap     : 4;

    // the stok join
    tb_uint32_t         join    : 4;

    // the fill rule
    tb_uint32_t         rule    : 1;

    // the quality
    tb_uint32_t         quality : 2;

    // the paint color
    gb_color_t          color;

    // the pen width
    gb_float_t          width;

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
    impl->mode    = GB_PAINT_MODE_NONE;
    impl->flag    = GB_PAINT_FLAG_ANTI_ALIAS | GB_PAINT_FLAG_BITMAP_FILTER;
    impl->cap     = GB_PAINT_CAP_BUTT;
    impl->join    = GB_PAINT_JOIN_MITER;
    impl->rule    = GB_PAINT_RULE_EVENODD;
    impl->width   = GB_ONE;
    impl->color   = GB_COLOR_DEFAULT;
    impl->quality = GB_QUALITY_TOP;

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
    return impl->color.a;
}
tb_void_t gb_paint_alpha_set(gb_paint_ref_t paint, tb_byte_t alpha)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->color.a = alpha;
}
gb_float_t gb_paint_width(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_ONE);

    // the width
    return impl->width;
}
tb_void_t gb_paint_width_set(gb_paint_ref_t paint, gb_float_t width)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->width = width;
}
tb_size_t gb_paint_cap(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_CAP_BUTT);

    // the cap
    return impl->cap;
}
tb_void_t gb_paint_cap_set(gb_paint_ref_t paint, tb_size_t cap)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->cap = (tb_uint32_t)cap;
}
tb_size_t gb_paint_join(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_JOIN_MITER);

    // the join
    return impl->join;
}
tb_void_t gb_paint_join_set(gb_paint_ref_t paint, tb_size_t join)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->join = (tb_uint32_t)join;
}
tb_size_t gb_paint_rule(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_PAINT_RULE_EVENODD);

    // the rule
    return impl->rule;
}
tb_void_t gb_paint_rule_set(gb_paint_ref_t paint, tb_size_t rule)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl);

    // done
    impl->rule = (tb_uint32_t)rule;
}
tb_size_t gb_paint_quality(gb_paint_ref_t paint)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return_val(impl, GB_QUALITY_TOP);

    // the quality
    return impl->quality;
}
tb_void_t gb_paint_quality_set(gb_paint_ref_t paint, tb_size_t quality)
{
    // check
    gb_paint_impl_t* impl = (gb_paint_impl_t*)paint;
    tb_assert_and_check_return(impl && quality <= GB_QUALITY_TOP);

    // done
    impl->quality = (tb_uint32_t)quality;
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
