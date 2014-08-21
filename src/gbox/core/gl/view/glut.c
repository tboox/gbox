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
 * @file        glut.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#ifdef TB_CONFIG_OS_MAC
# 	include <GLUT/glut.h>
#else
# 	define GL_GLEXT_PROTOTYPES
# 	include <GL/glut.h>
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the gl view impl type
typedef struct __gb_gl_view_impl_t
{
    // the name
    tb_string_t             name;

    // the width
    tb_size_t               width;

    // the height
    tb_size_t               height;

    // the pixfmt
    tb_size_t               pixfmt;

}gb_gl_view_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_gl_view_ref_t gb_gl_view_init(tb_char_t const* name, tb_size_t pixfmt, tb_size_t width, tb_size_t height)
{
    // done
    tb_bool_t           ok = tb_false;
    gb_gl_view_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);
	    tb_assert_and_check_break(  (pixfmt == (GB_PIXFMT_RGBA8888 | GB_PIXFMT_BENDIAN))
                                ||  (pixfmt == (GB_PIXFMT_RGB565))
                                || 	(pixfmt == (GB_PIXFMT_RGB888 | GB_PIXFMT_BENDIAN))
                                || 	(pixfmt == (GB_PIXFMT_RGBA4444))
                                || 	(pixfmt == (GB_PIXFMT_RGBA5551)));

        // make view
        impl = tb_malloc0_type(gb_gl_view_impl_t);
        tb_assert_and_check_break(impl);

        // init name
        if (!tb_string_init(&impl->name)) break;
        if (name) tb_string_cstrcpy(&impl->name, name);

        // init width and height
        impl->width     = width;
        impl->height    = height;

        // init pixfmt
        impl->pixfmt = pixfmt;

        // init glut
        tb_int_t    argc = 0;
        tb_char_t*  argv[1] = {0};
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(width, height);
        
        // make view
        glutCreateWindow(name? name : "");

#if 0
        glutDisplayFunc(tb_null);
        glutIdleFunc(tb_null);
        glutReshapeFunc(tb_null);
        glutMouseFunc(tb_null);
        glutPassiveMotionFunc(tb_null);
        glutMotionFunc(tb_null);
        glutKeyboardFunc(tb_null);
        glutSpecialFunc(tb_null);
#endif

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_gl_view_exit((gb_gl_view_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_gl_view_ref_t)impl;
}
tb_void_t gb_gl_view_exit(gb_gl_view_ref_t view)
{
    // check
    gb_gl_view_impl_t* impl = (gb_gl_view_impl_t*)view;
    tb_assert_and_check_return(impl);

    // exit name
    tb_string_exit(&impl->name);

    // exit it
    tb_free(view);
}
tb_size_t gb_gl_view_pixfmt(gb_gl_view_ref_t view)
{
    // check
    gb_gl_view_impl_t* impl = (gb_gl_view_impl_t*)view;
    tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
    return impl->pixfmt;
}
tb_size_t gb_gl_view_width(gb_gl_view_ref_t view)
{
    // check
    gb_gl_view_impl_t* impl = (gb_gl_view_impl_t*)view;
    tb_assert_and_check_return_val(impl, 0);

    // the width
    return impl->width;
}
tb_size_t gb_gl_view_height(gb_gl_view_ref_t view)
{
    // check
    gb_gl_view_impl_t* impl = (gb_gl_view_impl_t*)view;
    tb_assert_and_check_return_val(impl, 0);

    // the height
    return impl->height;
}
