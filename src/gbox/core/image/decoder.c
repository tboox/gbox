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
 * @file        decoder.c
 * @ingroup     core
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "decoder.h"
#include "decoder/prefix.h"
//#include "bmp.h"
//#include "jpg.h"
//#include "png.h"
//#include "gif.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_image_decoder_ref_t gb_image_decoder_init(tb_size_t pixfmt, tb_stream_ref_t stream)
{
    // the image decoder probe list
    static tb_size_t (*probe[])(tb_stream_ref_t ) =
    {
        tb_null
    ,   gb_image_decoder_bmp_probe
    };

    // the image decoder init list
    static gb_image_decoder_ref_t (*init[])(tb_size_t , tb_stream_ref_t ) =
    {
        tb_null
    ,   gb_image_decoder_bmp_init
    };
    tb_assert_static(tb_arrayn(probe) == tb_arrayn(init));

    // probe and init it
    tb_size_t i;
    tb_size_t n = tb_arrayn(init);
    tb_size_t p = n;
    tb_size_t m = 0;
    for (i = 1; i < n; i++)
    {
        // probe it
        tb_size_t s = probe[i](stream);
        if (s > m)
        {
            p = i;
            m = s;
            if (m == 100) break;
        }
    }

    // found? init it
    return (p < n)? init[p](pixfmt, stream) : tb_null;
}
tb_void_t gb_image_decoder_exit(gb_image_decoder_ref_t decoder)
{
    // check
    gb_image_decoder_impl_t* impl = (gb_image_decoder_impl_t*)decoder;
    tb_assert_and_check_return(impl);

    // exit decoder
    if (impl->exit) impl->exit(impl);

    // exit it
    tb_free(decoder);
}
gb_bitmap_ref_t gb_image_decoder_done(gb_image_decoder_ref_t decoder)
{
    // check
    gb_image_decoder_impl_t* impl = (gb_image_decoder_impl_t*)decoder;
    tb_assert_and_check_return_val(impl && impl->done, tb_null);

    // done
    return impl->done(impl);
}
