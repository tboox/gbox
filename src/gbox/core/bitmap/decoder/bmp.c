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
 * @file        bmp.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "bmp_decoder"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the bpp offset
#define GB_BMP_OFFSET_BPP               (28)

// the bitmap size offset
#define GB_BMP_OFFSET_BITMAP_SIZE        (34)

// the palette offset
#define GB_BMP_OFFSET_PALETTE           (54)

// the bmp compression flag
#define GB_BMP_RGB                      (0)
#define GB_BMP_RLE8                     (1)
#define GB_BMP_RLE4                     (2)
#define GB_BMP_BITFIELDS                (3)

/* the bmp format
 *
 * @code
 *
 * typedef struct __gb_bmp_t
 * {
 *      gb_bmp_file_header_t    header;
 *      gb_bmp_info_t           info;
 *  
 * }gb_bitmap_t;
 *
 * typedef struct __gb_bmp_file_header_t
 * {
 *      tb_uint16_t             type;               //!< bmp_id == 0x4D42("BM")
 *      tb_uint32_t             size; 
 *      tb_uint16_t             reserved1; 
 *      tb_uint16_t             reserved2; 
 *      tb_uint32_t             offset_bits; 
 *
 * }gb_bmp_file_header_t;
 *  
 * typedef struct __gb_bmp_info_t
 * {
 *      gb_bmp_info_header_t    header;
 *      gb_bmp_rgb_t            colors[1];
 *
 * }gb_bmp_info_t;
 *
 * typedef struct __gb_bmp_info_header_t
 * {
 *      tb_uint32_t             size;               // sizeof(gb_bmp_info_header_t) 
 *      tb_int32_t              w;                  // width 
 *      tb_int32_t              h;                  // height
 *      tb_uint16_t             planes; 
 *      tb_uint16_t             bpp;                // bst per pixels
 *      tb_uint32_t             compression;        // GB_BMP_RGB, GB_BMP_RLE4, GB_BMP_RLE8, GB_BMP_BITFIELDS 
 *      tb_uint32_t             bitmap_size;         // bitmap size for GB_BMP_RGB 
 *      tb_int32_t              x_pels_per_meter; 
 *      tb_int32_t              y_pels_per_meter; 
 *      tb_uint32_t             clr_used; 
 *      tb_uint32_t             clr_important; 
 *
 * }gb_bmp_info_header_t; 
 * 
 * typedef struct __gb_bmp_rgb_t 
 * {
 *      tb_byte_t                b;
 *      tb_byte_t                g;
 *      tb_byte_t                r;
 *      tb_byte_t                reserved;
 *
 * }gb_bmp_rgb_t;
 *
 * @endcode
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static gb_bitmap_ref_t gb_bitmap_decoder_bmp_done(gb_bitmap_decoder_impl_t* decoder)
{
    // check
    tb_assert_and_check_return_val(decoder && decoder->type == GB_BITMAP_TYPE_BMP, tb_null);

    // done
    tb_bool_t       ok = tb_false;
    gb_bitmap_ref_t bitmap = tb_null;
    do
    {
        // the pixfmt
        tb_size_t pixfmt    = decoder->pixfmt;
        tb_assert_and_check_break(GB_PIXFMT_OK(pixfmt));

        // the width and height
        tb_size_t width     = decoder->width;
        tb_size_t height    = decoder->height;
        tb_assert_and_check_break(width && height);

        // the stream
        tb_stream_ref_t stream = decoder->stream;
        tb_assert_and_check_break(stream);

        // the file size
        tb_hize_t filesize = tb_stream_size(stream);
        tb_assert_and_check_break(filesize > 14);

        // seek to the bpp position
        if (!tb_stream_skip(stream, 2)) break;

        // read bpp
        tb_size_t bpp = tb_stream_bread_u16_le(stream);
        tb_assert_and_check_break(bpp && bpp <= 32);

        // trace
        tb_trace_d("size: %lux%lu, bpp: %lu", width, height, bpp);

        // compression? not support 
        tb_size_t bc = tb_stream_bread_u32_le(stream);
        tb_assert_and_check_break(bc != GB_BMP_RLE4 && bc != GB_BMP_RLE8);

        // the data size
        tb_size_t linesize = (width * bpp) >> 3;
        tb_size_t datasize = tb_stream_bread_u32_le(stream);
        if (!datasize) datasize = tb_align4(linesize) * height;
        tb_assert_and_check_break(datasize && datasize < filesize);

        // trace
        tb_trace_d("data: %lu bytes", datasize);

        // has palette?
        gb_color_t  pals[256];
        tb_size_t   paln = 1 << bpp;
        if (bpp <= 8)
        {
#if 0
            // the palette count
            if (!tb_stream_skip(stream, 8)) break;
            paln = tb_stream_bread_u32_le(stream);
            if (!tb_stream_skip(stream, 4)) break;
#else
            if (!tb_stream_skip(stream, 16)) break;
#endif

            // trace
            tb_trace_d("pal: %lu", paln);

            // read the palette entries
            tb_size_t i = 0;
            for (i = 0; i < paln; i++)
            {
                pals[i].b = tb_stream_bread_u8(stream);
                pals[i].g = tb_stream_bread_u8(stream);
                pals[i].r = tb_stream_bread_u8(stream);
                if (!tb_stream_skip(stream, 1)) break;
                pals[i].a = 0xff;
            }
            tb_assert_and_check_break(i == paln);
        }

        // bitfields?
        gb_pixmap_ref_t sp = tb_null;
        gb_pixmap_ref_t dp = gb_pixmap(pixfmt, 0xff);
        if (bc == GB_BMP_BITFIELDS)
        {
            // seek to the color mask position
            if (!tb_stream_skip(stream, 16)) break;

            // read r, g, b mask
            tb_size_t rm = tb_stream_bread_u32_le(stream);
            tb_size_t gm = tb_stream_bread_u32_le(stream);
            tb_size_t bm = tb_stream_bread_u32_le(stream);

            // 16-bits?
            if (bpp == 16)
            {
                // rgb565?
                if (rm == 0xf800 && gm == 0x07e0 && bm == 0x001f)
                    sp = gb_pixmap(GB_PIXFMT_RGB565, 0xff);
                // xrgb1555?
                else if (rm == 0x7c00 && gm == 0x03e0 && bm == 0x001f)
                    sp = gb_pixmap(GB_PIXFMT_XRGB1555, 0xff);
            }
            // 32-bits?
            else if (bpp == 32)
            {
                // rgbx8888?
                if (rm == 0xff000000 && gm == 0xff0000 && bm == 0xff00)
                    sp = gb_pixmap(GB_PIXFMT_RGBX8888, 0xff);
            }
        }
        // rgb?
        else if (bc == GB_BMP_RGB)
        {
            switch (bpp)
            {
            case 32:
                // argb8888
                sp = gb_pixmap(GB_PIXFMT_ARGB8888, 0xff);
                break;
            case 24:
                // rgb888
                sp = gb_pixmap(GB_PIXFMT_RGB888, 0xff);
                break;
            case 16:
                // xrgb1555
                sp = gb_pixmap(GB_PIXFMT_XRGB1555, 0xff);
                break;
            case 8:
            case 4:
            case 1:
                // pal8
                sp = gb_pixmap(GB_PIXFMT_PAL8, 0xff);
                break;
            default:
                // trace
                tb_trace_e("the bpp: %lu is not supported", bpp);
                break;
            }
        }

        // check
        tb_assert_and_check_break(sp && dp);

        // trace
        tb_trace_d("pixfmt: %s => %s", sp->name, dp->name);

        // seek to the bmp data position
        if (!tb_stream_seek(stream, filesize - datasize)) break;

        // init bitmap, default: no alpha
        bitmap = gb_bitmap_init(tb_null, pixfmt, width, height, 0, tb_false);
        tb_assert_and_check_break(bitmap);

        // the bitmap data
        tb_byte_t*  data = (tb_byte_t*)gb_bitmap_data(bitmap);
        tb_assert_and_check_break(data);
        
        // done
        gb_color_t  c;
        tb_byte_t   row_data[8192];
        tb_size_t   btp_dst = dp->btp;
        tb_size_t   btp_src = sp->btp;
        tb_size_t   has_alpha = 0;
	    tb_byte_t 	max_alpha = GB_ALPHA_MAXN;
        tb_size_t   row_bytes = gb_bitmap_row_bytes(bitmap);
        tb_size_t   row_bytes_align4 = tb_align4(linesize);
        tb_byte_t*  p = data + (height - 1) * row_bytes;
        tb_assert_and_check_break(row_bytes_align4 <= sizeof(row_data));
        if (bpp > 8)
        {
            while (height--)
            {
                // read line
                if (!tb_stream_bread(stream, row_data, row_bytes_align4)) break;
                
                // save bitmap data
                tb_byte_t*  d = p;
                tb_size_t   i = 0;
                for (i = 0; i < linesize; i += btp_src, d += btp_dst)
                {
                    // load color
                    c = sp->color_get(row_data + i);

                    // save color
                    dp->color_set(d, c);

                    // has alpha?
                    if (!has_alpha) has_alpha = c.a < max_alpha;
                }

                // next line
                p -= row_bytes;
            }
        }
        else if (bpp == 8)
        {
            while (height--)
            {
                // read line
                if (!tb_stream_bread(stream, row_data, row_bytes_align4)) break;
                
                // save bitmap data
                tb_byte_t*  d = p;
                tb_size_t   i = 0;
                for (i = 0; i < linesize; i++, d += btp_dst)
                {
                    // load color
                    c = pals[row_data[i]];

                    // save color
                    dp->color_set(d, c);

                    // has alpha?
                    if (!has_alpha) has_alpha = c.a < max_alpha;
                }

                // next line
                p -= row_bytes;
            }
        }
        // bpp < 8?
        else
        {
            while (height--)
            {
                // read line
                if (!tb_stream_bread(stream, row_data, row_bytes_align4)) break;
                
                // save bitmap data
                tb_byte_t*  d = p;
                tb_size_t   i = 0;
                tb_size_t   m = linesize << 3;
                for (i = 0; i < m; i += bpp, d += btp_dst)
                {
                    // load color
                    c = pals[tb_bits_get_ubits32(&row_data[i / 8], i & 7, bpp)];

                    // save color
                    dp->color_set(d, c);

                    // has alpha?
                    if (!has_alpha) has_alpha = c.a < max_alpha;
                }

                // next line
                p -= row_bytes;
            }
        }

        // check
        tb_assert_and_check_break(!(height + 1));

        // set alpha
        gb_bitmap_set_alpha(bitmap, has_alpha? tb_true : tb_false);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (bitmap) gb_bitmap_exit(bitmap);
        bitmap = tb_null;
    }

    // ok?
    return bitmap;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_size_t gb_bitmap_decoder_bmp_probe(tb_stream_ref_t stream)
{
    // check
    tb_assert_and_check_return_val(stream, 0);

    // need
    tb_byte_t* p = tb_null;
    if (!tb_stream_need(stream, &p, 2)) return 0;
    tb_assert_and_check_return_val(p, 0);

    // ok?
    return (p[0] == 'B' && p[1] == 'M')? 80 : 0;
}
gb_bitmap_decoder_ref_t gb_bitmap_decoder_bmp_init(tb_size_t pixfmt, tb_stream_ref_t stream)
{
    // check
    tb_assert_and_check_return_val(GB_PIXFMT_OK(pixfmt) && stream, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    gb_bitmap_decoder_impl_t*    impl = tb_null;
    do
    {
        // seek to the width and height position
        if (!tb_stream_skip(stream, 18)) break;

        // read width and height
        tb_size_t width     = tb_stream_bread_u32_le(stream);
        tb_size_t height    = tb_stream_bread_u32_le(stream);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make decoder
        impl = tb_malloc0_type(gb_bitmap_decoder_impl_t);
        tb_assert_and_check_break(impl);

        // init decoder
        impl->type          = GB_BITMAP_TYPE_BMP;
        impl->stream        = stream;
        impl->pixfmt        = pixfmt;
        impl->width         = width;
        impl->height        = height;
        impl->done          = gb_bitmap_decoder_bmp_done;

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_bitmap_decoder_exit((gb_bitmap_decoder_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_bitmap_decoder_ref_t)impl;
}
