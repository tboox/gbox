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
 * @file        interface.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "interface.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
// define func
#define GB_GL_INTERFACE_DEFINE(func)            gb_##func##_t gb_##func = tb_null

// check func
#define GB_GL_INTERFACE_CHECK(func)             tb_assert_and_check_return_val(gb_##func, tb_false)

// check func and the wrap func
#define GB_GL_INTERFACE_CHECKW(func, wrap)      if (!gb_##func && gb_##wrap) gb_##func = gb_gl_wraper_##func; tb_assert_and_check_return_val(gb_##func, tb_false)

/* //////////////////////////////////////////////////////////////////////////////////////
 * functions
 */

GB_GL_INTERFACE_DEFINE(glActiveTexture);
GB_GL_INTERFACE_DEFINE(glAlphaFunc);
GB_GL_INTERFACE_DEFINE(glAttachShader);
GB_GL_INTERFACE_DEFINE(glBindTexture);
GB_GL_INTERFACE_DEFINE(glBlendFunc);
GB_GL_INTERFACE_DEFINE(glClear);
GB_GL_INTERFACE_DEFINE(glClearColor);
GB_GL_INTERFACE_DEFINE(glClearStencil);
GB_GL_INTERFACE_DEFINE(glColor4f);
GB_GL_INTERFACE_DEFINE(glColorMask);
GB_GL_INTERFACE_DEFINE(glColorPointer);
GB_GL_INTERFACE_DEFINE(glCompileShader);
GB_GL_INTERFACE_DEFINE(glCreateProgram);
GB_GL_INTERFACE_DEFINE(glCreateShader);
GB_GL_INTERFACE_DEFINE(glDeleteProgram);
GB_GL_INTERFACE_DEFINE(glDeleteShader);
GB_GL_INTERFACE_DEFINE(glDeleteTextures);
GB_GL_INTERFACE_DEFINE(glDisable);
GB_GL_INTERFACE_DEFINE(glDisableClientState);
GB_GL_INTERFACE_DEFINE(glDisableVertexAttribArray);
GB_GL_INTERFACE_DEFINE(glDrawArrays);
GB_GL_INTERFACE_DEFINE(glEnable);
GB_GL_INTERFACE_DEFINE(glEnableClientState);
GB_GL_INTERFACE_DEFINE(glEnableVertexAttribArray);
GB_GL_INTERFACE_DEFINE(glGenTextures);
GB_GL_INTERFACE_DEFINE(glGetAttribLocation);
GB_GL_INTERFACE_DEFINE(glGetProgramiv);
GB_GL_INTERFACE_DEFINE(glGetProgramInfoLog);
GB_GL_INTERFACE_DEFINE(glGetShaderiv);
GB_GL_INTERFACE_DEFINE(glGetShaderInfoLog);
GB_GL_INTERFACE_DEFINE(glGetString);
GB_GL_INTERFACE_DEFINE(glGetUniformLocation);
GB_GL_INTERFACE_DEFINE(glHint);
GB_GL_INTERFACE_DEFINE(glIsTexture);
GB_GL_INTERFACE_DEFINE(glLineWidth);
GB_GL_INTERFACE_DEFINE(glLinkProgram);
GB_GL_INTERFACE_DEFINE(glLoadIdentity);
GB_GL_INTERFACE_DEFINE(glLoadMatrixf);
GB_GL_INTERFACE_DEFINE(glMatrixMode);
GB_GL_INTERFACE_DEFINE(glMultMatrixf);
GB_GL_INTERFACE_DEFINE(glOrtho);
GB_GL_INTERFACE_DEFINE(glOrthof);
GB_GL_INTERFACE_DEFINE(glPixelStorei);
GB_GL_INTERFACE_DEFINE(glPopMatrix);
GB_GL_INTERFACE_DEFINE(glPushMatrix);
GB_GL_INTERFACE_DEFINE(glRotatef);
GB_GL_INTERFACE_DEFINE(glScalef);
GB_GL_INTERFACE_DEFINE(glScissor);
GB_GL_INTERFACE_DEFINE(glShaderSource);
GB_GL_INTERFACE_DEFINE(glStencilFunc);
GB_GL_INTERFACE_DEFINE(glStencilMask);
GB_GL_INTERFACE_DEFINE(glStencilOp);
GB_GL_INTERFACE_DEFINE(glTexCoordPointer);
GB_GL_INTERFACE_DEFINE(glTexEnvi);
GB_GL_INTERFACE_DEFINE(glTexImage2D);
GB_GL_INTERFACE_DEFINE(glTexParameterf);
GB_GL_INTERFACE_DEFINE(glTexParameteri);
GB_GL_INTERFACE_DEFINE(glTranslatef);
GB_GL_INTERFACE_DEFINE(glUniform1i);
GB_GL_INTERFACE_DEFINE(glUniformMatrix4fv);
GB_GL_INTERFACE_DEFINE(glUseProgram);
GB_GL_INTERFACE_DEFINE(glVertexAttrib4f);
GB_GL_INTERFACE_DEFINE(glVertexAttribPointer);
GB_GL_INTERFACE_DEFINE(glVertexPointer);
GB_GL_INTERFACE_DEFINE(glViewport);

/* //////////////////////////////////////////////////////////////////////////////////////
 * wraper
 */
__tb_extern_c__ static gb_GLvoid_t GB_GL_APICALL gb_gl_wraper_glOrthof(gb_GLfloat_t left, gb_GLfloat_t right, gb_GLfloat_t bottom, gb_GLfloat_t top, gb_GLfloat_t near, gb_GLfloat_t far)
{
    gb_glOrtho(left, right, bottom, top, near, far);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_gl_interface_check(tb_byte_t version)
{
    // check interfaces for common
//  GB_GL_INTERFACE_CHECK   (glActiveTexture);
//  GB_GL_INTERFACE_CHECK   (glAlphaFunc);
    GB_GL_INTERFACE_CHECK   (glBindTexture);
    GB_GL_INTERFACE_CHECK   (glBlendFunc);
    GB_GL_INTERFACE_CHECK   (glClear);
    GB_GL_INTERFACE_CHECK   (glClearColor);
//  GB_GL_INTERFACE_CHECK   (glClearStencil);
    GB_GL_INTERFACE_CHECK   (glColorMask);
    GB_GL_INTERFACE_CHECK   (glDeleteTextures);
    GB_GL_INTERFACE_CHECK   (glDisable);
    GB_GL_INTERFACE_CHECK   (glDrawArrays);
    GB_GL_INTERFACE_CHECK   (glEnable);
    GB_GL_INTERFACE_CHECK   (glGenTextures);
    GB_GL_INTERFACE_CHECK   (glGetString);
//  GB_GL_INTERFACE_CHECK   (glIsTexture);
    GB_GL_INTERFACE_CHECK   (glPixelStorei);
    GB_GL_INTERFACE_CHECK   (glScissor);
    GB_GL_INTERFACE_CHECK   (glStencilFunc);
    GB_GL_INTERFACE_CHECK   (glStencilMask);
    GB_GL_INTERFACE_CHECK   (glStencilOp);
    GB_GL_INTERFACE_CHECK   (glTexImage2D);
    GB_GL_INTERFACE_CHECK   (glTexParameterf);
    GB_GL_INTERFACE_CHECK   (glTexParameteri);
    GB_GL_INTERFACE_CHECK   (glViewport);

    // check interfaces for gl 1.x
    if (version < 0x20)
    {
        GB_GL_INTERFACE_CHECK   (glColor4f);
        GB_GL_INTERFACE_CHECK   (glColorPointer);
        GB_GL_INTERFACE_CHECK   (glDisableClientState);
        GB_GL_INTERFACE_CHECK   (glEnableClientState);
        GB_GL_INTERFACE_CHECK   (glLoadIdentity);
        GB_GL_INTERFACE_CHECK   (glLoadMatrixf);
        GB_GL_INTERFACE_CHECK   (glMatrixMode);
        GB_GL_INTERFACE_CHECK   (glMultMatrixf);
        GB_GL_INTERFACE_CHECKW  (glOrthof, glOrtho);
        GB_GL_INTERFACE_CHECK   (glPopMatrix);
        GB_GL_INTERFACE_CHECK   (glPushMatrix);
//      GB_GL_INTERFACE_CHECK   (glRotatef);
//      GB_GL_INTERFACE_CHECK   (glScalef);
        GB_GL_INTERFACE_CHECK   (glTexCoordPointer);
        GB_GL_INTERFACE_CHECK   (glTexEnvi);
//      GB_GL_INTERFACE_CHECK   (glTranslatef);
        GB_GL_INTERFACE_CHECK   (glVertexPointer);
    }
    // check interfaces for gl >= 2.0
    else
    {
        GB_GL_INTERFACE_CHECK   (glAttachShader);
        GB_GL_INTERFACE_CHECK   (glCompileShader);
        GB_GL_INTERFACE_CHECK   (glCreateProgram);
        GB_GL_INTERFACE_CHECK   (glCreateShader);
        GB_GL_INTERFACE_CHECK   (glDeleteProgram);
        GB_GL_INTERFACE_CHECK   (glDeleteShader);
        GB_GL_INTERFACE_CHECK   (glDisableVertexAttribArray);
        GB_GL_INTERFACE_CHECK   (glEnableVertexAttribArray);
        GB_GL_INTERFACE_CHECK   (glGetAttribLocation);
        GB_GL_INTERFACE_CHECK   (glGetProgramiv);
        GB_GL_INTERFACE_CHECK   (glGetProgramInfoLog);
        GB_GL_INTERFACE_CHECK   (glGetShaderiv);
        GB_GL_INTERFACE_CHECK   (glGetShaderInfoLog);
        GB_GL_INTERFACE_CHECK   (glGetUniformLocation);
        GB_GL_INTERFACE_CHECK   (glLinkProgram);
        GB_GL_INTERFACE_CHECK   (glShaderSource);
        GB_GL_INTERFACE_CHECK   (glUniform1i);
        GB_GL_INTERFACE_CHECK   (glUniformMatrix4fv);
        GB_GL_INTERFACE_CHECK   (glUseProgram);
        GB_GL_INTERFACE_CHECK   (glVertexAttrib4f);
        GB_GL_INTERFACE_CHECK   (glVertexAttribPointer);
    }


    // ok
    return tb_true;
}
