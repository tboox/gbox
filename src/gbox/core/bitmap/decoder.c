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
gb_bitmap_decoder_ref_t gb_bitmap_decoder_init(tb_size_t pixfmt, tb_stream_ref_t stream)
{
    // the bitmap decoder probe list
    static tb_size_t (*probe[])(tb_stream_ref_t ) =
    {
        tb_null
    ,   gb_bitmap_decoder_bmp_probe
    };

    // the bitmap decoder init list
    static gb_bitmap_decoder_ref_t (*init[])(tb_size_t , tb_stream_ref_t ) =
    {
        tb_null
    ,   gb_bitmap_decoder_bmp_init
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
tb_void_t gb_bitmap_decoder_exit(gb_bitmap_decoder_ref_t decoder)
{
    // check
    gb_bitmap_decoder_impl_t* impl = (gb_bitmap_decoder_impl_t*)decoder;
    tb_assert_and_check_return(impl);

    // exit decoder
    if (impl->exit) impl->exit(impl);

    // exit it
    tb_free(decoder);
}
gb_bitmap_ref_t gb_bitmap_decoder_done(gb_bitmap_decoder_ref_t decoder)
{
    // check
    gb_bitmap_decoder_impl_t* impl = (gb_bitmap_decoder_impl_t*)decoder;
    tb_assert_and_check_return_val(impl && impl->done, tb_null);

    // done
    return impl->done(impl);
}
