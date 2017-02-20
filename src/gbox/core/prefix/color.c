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
    tb_assert_and_check_return_val(item && name, 0);

    // comp
    return tb_strnicmp(((gb_named_color_t const*)item)->name, (tb_char_t const*)name, tb_strlen(((gb_named_color_t const*)item)->name));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_color_t const gb_color_from_name(tb_char_t const* name)
{
    // check
    tb_assert_and_check_return_val(name, GB_COLOR_DEFAULT);

    // init iterator
    tb_array_iterator_t     array_iterator;
    tb_iterator_ref_t       iterator = tb_iterator_make_for_mem(&array_iterator, g_named_colors, tb_arrayn(g_named_colors), sizeof(gb_named_color_t));
    tb_assert(iterator);

    // find it by the binary search
    tb_size_t itor = tb_binary_find_all_if(iterator, gb_named_color_comp, name);

    // the color
    gb_named_color_t const* color = (itor != tb_iterator_tail(iterator))? (gb_named_color_t const*)tb_iterator_item(iterator, itor) : tb_null;

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

