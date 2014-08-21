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
 * @file        color.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "color.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// init color from argb
#ifdef TB_WORDS_BIGENDIAN
#   define GB_COLOR_INIT_ARGB(a, r, g, b)       { a, r, g, b }
#else
#   define GB_COLOR_INIT_ARGB(a, r, g, b)       { b, g, r, a }
#endif

// init color from name & argb
#define GB_COLOR_INIT_NAME(name, a, r, g, b) 	{name, GB_COLOR_INIT_ARGB(a, r, g, b)}

// init color contants
#define GB_COLOR_INIT_BLACK 			GB_COLOR_INIT_NAME( "black", 		0xff, 	0x00, 	0x00, 	0x00 	)
#define GB_COLOR_INIT_BLUE 				GB_COLOR_INIT_NAME( "blue", 		0xff, 	0x00, 	0x00, 	0xff 	)
#define GB_COLOR_INIT_BROWN 			GB_COLOR_INIT_NAME( "brown", 		0xff, 	0x80, 	0x00, 	0x00 	)
#define GB_COLOR_INIT_DARKBLUE 			GB_COLOR_INIT_NAME( "darkblue", 	0xff, 	0x00, 	0x00, 	0x8b 	)
#define GB_COLOR_INIT_GOLD 				GB_COLOR_INIT_NAME( "gold", 		0xff, 	0xff, 	0xd7, 	0x00 	)
#define GB_COLOR_INIT_GRAY 				GB_COLOR_INIT_NAME( "gray", 		0xff, 	0x80, 	0x80, 	0x80 	)
#define GB_COLOR_INIT_GREY 				GB_COLOR_INIT_NAME( "grey", 		0xff, 	0x80, 	0x80, 	0x80 	)
#define GB_COLOR_INIT_GREEN 			GB_COLOR_INIT_NAME( "green", 		0xff, 	0x00, 	0xff, 	0x00 	)
#define GB_COLOR_INIT_LIGHTBLUE 		GB_COLOR_INIT_NAME( "lightblue", 	0xff, 	0xad, 	0xd8, 	0xa6 	)
#define GB_COLOR_INIT_LIGHTGRAY 		GB_COLOR_INIT_NAME( "lightgray", 	0xff, 	0xd3, 	0xd3, 	0xd3 	)
#define GB_COLOR_INIT_LIGHTGREY 		GB_COLOR_INIT_NAME( "lightgrey", 	0xff, 	0xd3, 	0xd3, 	0xd3 	)
#define GB_COLOR_INIT_LIGHTPINK 		GB_COLOR_INIT_NAME( "lightpink", 	0xff, 	0xff, 	0xb6, 	0xc1 	)
#define GB_COLOR_INIT_LIGHTYELLOW 		GB_COLOR_INIT_NAME( "lightyellow", 	0xff, 	0xff, 	0xff, 	0xe0 	)
#define GB_COLOR_INIT_NAVY 				GB_COLOR_INIT_NAME( "navy", 		0xff, 	0x00, 	0x00, 	0x80 	)
#define GB_COLOR_INIT_ORANGE 			GB_COLOR_INIT_NAME( "orange", 		0xff, 	0xff, 	0xa5, 	0x00 	)
#define GB_COLOR_INIT_PINK 				GB_COLOR_INIT_NAME( "pink", 		0xff, 	0xff, 	0x00, 	0xff 	)
#define GB_COLOR_INIT_PURPLE 			GB_COLOR_INIT_NAME( "purple", 		0xff, 	0x80, 	0x00, 	0x80 	)
#define GB_COLOR_INIT_RED 				GB_COLOR_INIT_NAME( "red", 			0xff, 	0xff, 	0x00, 	0x00 	)
#define GB_COLOR_INIT_SNOW 				GB_COLOR_INIT_NAME( "snow", 		0xff, 	0xff, 	0xfa, 	0xfa 	)
#define GB_COLOR_INIT_TOMATO 			GB_COLOR_INIT_NAME( "tomato", 		0xff, 	0xff, 	0x63, 	0x47 	)
#define GB_COLOR_INIT_YELLOW 			GB_COLOR_INIT_NAME( "yellow", 		0xff, 	0xff, 	0xff, 	0x00 	)
#define GB_COLOR_INIT_WHEAT 			GB_COLOR_INIT_NAME( "wheat", 		0xff, 	0xf5, 	0xde, 	0xb3 	)
#define GB_COLOR_INIT_WHITE 			GB_COLOR_INIT_NAME( "white", 		0xff, 	0xff, 	0xff, 	0xff 	)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the named color type
typedef struct __gb_named_color_t
{
	// the name
	tb_char_t const* 	name;

	// the color
	gb_color_t 			color;

}gb_named_color_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the colors
static gb_named_color_t g_named_colors[] = 
{
    GB_COLOR_INIT_BLACK
,   GB_COLOR_INIT_BLUE 
,   GB_COLOR_INIT_BROWN
,   GB_COLOR_INIT_DARKBLUE
,   GB_COLOR_INIT_GOLD 
,   GB_COLOR_INIT_GRAY
,   GB_COLOR_INIT_GREEN
,   GB_COLOR_INIT_GREY
,   GB_COLOR_INIT_LIGHTBLUE
,   GB_COLOR_INIT_LIGHTGRAY
,   GB_COLOR_INIT_LIGHTGREY
,   GB_COLOR_INIT_LIGHTPINK
,   GB_COLOR_INIT_LIGHTYELLOW
,   GB_COLOR_INIT_NAVY
,   GB_COLOR_INIT_ORANGE
,   GB_COLOR_INIT_PINK
,   GB_COLOR_INIT_PURPLE
,   GB_COLOR_INIT_RED
,   GB_COLOR_INIT_SNOW
,   GB_COLOR_INIT_TOMATO
,   GB_COLOR_INIT_YELLOW
,   GB_COLOR_INIT_WHEAT
,   GB_COLOR_INIT_WHITE

};

/* //////////////////////////////////////////////////////////////////////////////////////
 * comparator
 */
static tb_long_t gb_named_color_comp(tb_iterator_ref_t iterator, tb_cpointer_t item, tb_cpointer_t name)
{
    // check
    tb_assert_return_val(item && name, 0);

    // comp
    return tb_strnicmp(((gb_named_color_t const*)item)->name, name, tb_strlen(((gb_named_color_t const*)item)->name));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_color_t const gb_color_from_name(tb_char_t const* name)
{
    // check
    tb_assert_and_check_return_val(name, GB_COLOR_DEFAULT);

    // init iterator
    tb_iterator_t iterator = tb_iterator_init_mem(g_named_colors, tb_arrayn(g_named_colors), sizeof(gb_named_color_t));

    // find it by the binary search
    tb_size_t itor = tb_binary_find_all_if(&iterator, gb_named_color_comp, name);

    // the color
    gb_named_color_t const* color = (itor != tb_iterator_tail(&iterator))? (gb_named_color_t const*)tb_iterator_item(&iterator, itor) : tb_null;

    // ok?
    return color? color->color : GB_COLOR_DEFAULT;
}
gb_color_t const gb_color_from_index(tb_size_t index)
{
    // check
    tb_assert_and_check_return_val(index < tb_arrayn(g_named_colors), GB_COLOR_DEFAULT);

    // ok
    return g_named_colors[index].color;
}

