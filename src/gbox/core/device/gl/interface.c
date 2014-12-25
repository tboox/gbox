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
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_interface"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "interface.h"
#include "opengl/opengl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
// define func
#define GB_GL_INTERFACE_DEFINE(func)            gb_##func##_t gb_##func = tb_null

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
 * private implementation
 */
static gb_GLvoid_t GB_GL_APICALL gb_gl_interface_glOrthof(gb_GLfloat_t left, gb_GLfloat_t right, gb_GLfloat_t bottom, gb_GLfloat_t top, gb_GLfloat_t nearp, gb_GLfloat_t farp)
{
    gb_glOrtho(left, right, bottom, top, nearp, farp);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_gl_interface_load()
{
    // done
    tb_bool_t ok = tb_false;
    do
    {
#ifdef TB_CONFIG_OS_ANDROID

        // load v2 library first
        tb_dynamic_ref_t library = tb_null;
        if ((library = tb_dynamic_init("libGLESv2.so")))
        {
            // load interfaces for common
            GB_GL_INTERFACE_LOAD_D(library, glActiveTexture);
            GB_GL_INTERFACE_LOAD_D(library, glAlphaFunc);
            GB_GL_INTERFACE_LOAD_D(library, glBindTexture);
            GB_GL_INTERFACE_LOAD_D(library, glBlendFunc);
            GB_GL_INTERFACE_LOAD_D(library, glClear);
            GB_GL_INTERFACE_LOAD_D(library, glClearColor);
            GB_GL_INTERFACE_LOAD_D(library, glClearStencil);
            GB_GL_INTERFACE_LOAD_D(library, glColorMask);
            GB_GL_INTERFACE_LOAD_D(library, glDeleteTextures);
            GB_GL_INTERFACE_LOAD_D(library, glDisable);
            GB_GL_INTERFACE_LOAD_D(library, glDrawArrays);
            GB_GL_INTERFACE_LOAD_D(library, glEnable);
            GB_GL_INTERFACE_LOAD_D(library, glGenTextures);
            GB_GL_INTERFACE_LOAD_D(library, glGetString);
            GB_GL_INTERFACE_LOAD_D(library, glIsTexture);
            GB_GL_INTERFACE_LOAD_D(library, glPixelStorei);
            GB_GL_INTERFACE_LOAD_D(library, glScissor);
            GB_GL_INTERFACE_LOAD_D(library, glStencilFunc);
            GB_GL_INTERFACE_LOAD_D(library, glStencilMask);
            GB_GL_INTERFACE_LOAD_D(library, glStencilOp);
            GB_GL_INTERFACE_LOAD_D(library, glTexImage2D);
            GB_GL_INTERFACE_LOAD_D(library, glTexParameterf);
            GB_GL_INTERFACE_LOAD_D(library, glTexParameteri);
            GB_GL_INTERFACE_LOAD_D(library, glViewport);

            // load interfaces for gl >= 2.0
            GB_GL_INTERFACE_LOAD_D(library, glAttachShader);
            GB_GL_INTERFACE_LOAD_D(library, glCompileShader);
            GB_GL_INTERFACE_LOAD_D(library, glCreateProgram);
            GB_GL_INTERFACE_LOAD_D(library, glCreateShader);
            GB_GL_INTERFACE_LOAD_D(library, glDeleteProgram);
            GB_GL_INTERFACE_LOAD_D(library, glDeleteShader);
            GB_GL_INTERFACE_LOAD_D(library, glDisableVertexAttribArray);
            GB_GL_INTERFACE_LOAD_D(library, glEnableVertexAttribArray);
            GB_GL_INTERFACE_LOAD_D(library, glGetAttribLocation);
            GB_GL_INTERFACE_LOAD_D(library, glGetProgramiv);
            GB_GL_INTERFACE_LOAD_D(library, glGetProgramInfoLog);
            GB_GL_INTERFACE_LOAD_D(library, glGetShaderiv);
            GB_GL_INTERFACE_LOAD_D(library, glGetShaderInfoLog);
            GB_GL_INTERFACE_LOAD_D(library, glGetUniformLocation);
            GB_GL_INTERFACE_LOAD_D(library, glLinkProgram);
            GB_GL_INTERFACE_LOAD_D(library, glShaderSource);
            GB_GL_INTERFACE_LOAD_D(library, glUniform1i);
            GB_GL_INTERFACE_LOAD_D(library, glUniformMatrix4fv);
            GB_GL_INTERFACE_LOAD_D(library, glUseProgram);
            GB_GL_INTERFACE_LOAD_D(library, glVertexAttrib4f);
            GB_GL_INTERFACE_LOAD_D(library, glVertexAttribPointer);
        }
        // load v1 library
        else if ((library = tb_dynamic_init("libGLESv1_CM.so")))
        {
            // load interfaces for common
            GB_GL_INTERFACE_LOAD_D(library, glActiveTexture);
            GB_GL_INTERFACE_LOAD_D(library, glAlphaFunc);
            GB_GL_INTERFACE_LOAD_D(library, glBindTexture);
            GB_GL_INTERFACE_LOAD_D(library, glBlendFunc);
            GB_GL_INTERFACE_LOAD_D(library, glClear);
            GB_GL_INTERFACE_LOAD_D(library, glClearColor);
            GB_GL_INTERFACE_LOAD_D(library, glClearStencil);
            GB_GL_INTERFACE_LOAD_D(library, glColorMask);
            GB_GL_INTERFACE_LOAD_D(library, glDeleteTextures);
            GB_GL_INTERFACE_LOAD_D(library, glDisable);
            GB_GL_INTERFACE_LOAD_D(library, glDrawArrays);
            GB_GL_INTERFACE_LOAD_D(library, glEnable);
            GB_GL_INTERFACE_LOAD_D(library, glGenTextures);
            GB_GL_INTERFACE_LOAD_D(library, glGetString);
            GB_GL_INTERFACE_LOAD_D(library, glIsTexture);
            GB_GL_INTERFACE_LOAD_D(library, glPixelStorei);
            GB_GL_INTERFACE_LOAD_D(library, glScissor);
            GB_GL_INTERFACE_LOAD_D(library, glStencilFunc);
            GB_GL_INTERFACE_LOAD_D(library, glStencilMask);
            GB_GL_INTERFACE_LOAD_D(library, glStencilOp);
            GB_GL_INTERFACE_LOAD_D(library, glTexImage2D);
            GB_GL_INTERFACE_LOAD_D(library, glTexParameterf);
            GB_GL_INTERFACE_LOAD_D(library, glTexParameteri);
            GB_GL_INTERFACE_LOAD_D(library, glViewport);

            // load interfaces for gl 1.x
            GB_GL_INTERFACE_LOAD_D(library, glColor4f);
            GB_GL_INTERFACE_LOAD_D(library, glColorPointer);
            GB_GL_INTERFACE_LOAD_D(library, glDisableClientState);
            GB_GL_INTERFACE_LOAD_D(library, glEnableClientState);
            GB_GL_INTERFACE_LOAD_D(library, glLoadIdentity);
            GB_GL_INTERFACE_LOAD_D(library, glLoadMatrixf);
            GB_GL_INTERFACE_LOAD_D(library, glMatrixMode);
            GB_GL_INTERFACE_LOAD_D(library, glMultMatrixf);
            GB_GL_INTERFACE_LOAD_D(library, glOrthof);
            GB_GL_INTERFACE_LOAD_D(library, glPopMatrix);
            GB_GL_INTERFACE_LOAD_D(library, glPushMatrix);
            GB_GL_INTERFACE_LOAD_D(library, glRotatef);
            GB_GL_INTERFACE_LOAD_D(library, glScalef);
            GB_GL_INTERFACE_LOAD_D(library, glTexCoordPointer);
            GB_GL_INTERFACE_LOAD_D(library, glTexEnvi);
            GB_GL_INTERFACE_LOAD_D(library, glTranslatef);
            GB_GL_INTERFACE_LOAD_D(library, glVertexPointer);
        }

#else
        // load interfaces for common
#ifndef TB_CONFIG_OS_WINDOWS
        GB_GL_INTERFACE_LOAD_S(glActiveTexture);
#endif
        GB_GL_INTERFACE_LOAD_S(glAlphaFunc);
        GB_GL_INTERFACE_LOAD_S(glBindTexture);
        GB_GL_INTERFACE_LOAD_S(glBlendFunc);
        GB_GL_INTERFACE_LOAD_S(glClear);
        GB_GL_INTERFACE_LOAD_S(glClearColor);
        GB_GL_INTERFACE_LOAD_S(glClearStencil);
        GB_GL_INTERFACE_LOAD_S(glColorMask);
        GB_GL_INTERFACE_LOAD_S(glDeleteTextures);
        GB_GL_INTERFACE_LOAD_S(glDisable);
        GB_GL_INTERFACE_LOAD_S(glDrawArrays);
        GB_GL_INTERFACE_LOAD_S(glEnable);
        GB_GL_INTERFACE_LOAD_S(glGenTextures);
        GB_GL_INTERFACE_LOAD_S(glGetString);
        GB_GL_INTERFACE_LOAD_S(glHint);
        GB_GL_INTERFACE_LOAD_S(glIsTexture);
        GB_GL_INTERFACE_LOAD_S(glLineWidth);
        GB_GL_INTERFACE_LOAD_S(glPixelStorei);
        GB_GL_INTERFACE_LOAD_S(glScissor);
        GB_GL_INTERFACE_LOAD_S(glStencilFunc);
        GB_GL_INTERFACE_LOAD_S(glStencilMask);
        GB_GL_INTERFACE_LOAD_S(glStencilOp);
        GB_GL_INTERFACE_LOAD_S(glTexImage2D);
        GB_GL_INTERFACE_LOAD_S(glTexParameterf);
        GB_GL_INTERFACE_LOAD_S(glTexParameteri);
        GB_GL_INTERFACE_LOAD_S(glViewport);

        // load interfaces for gl 1.x
        GB_GL_INTERFACE_LOAD_S(glColor4f);
        GB_GL_INTERFACE_LOAD_S(glColorPointer);
        GB_GL_INTERFACE_LOAD_S(glDisableClientState);
        GB_GL_INTERFACE_LOAD_S(glEnableClientState);
        GB_GL_INTERFACE_LOAD_S(glLoadIdentity);
        GB_GL_INTERFACE_LOAD_S(glLoadMatrixf);
        GB_GL_INTERFACE_LOAD_S(glMatrixMode);
        GB_GL_INTERFACE_LOAD_S(glMultMatrixf);
#   ifdef TB_CONFIG_OS_IOS
        GB_GL_INTERFACE_LOAD_S(glOrthof);
#   else
        GB_GL_INTERFACE_LOAD_S(glOrtho);
        GB_GL_INTERFACE_LOAD_S_(glOrthof, gb_gl_interface_glOrthof);
#   endif
        GB_GL_INTERFACE_LOAD_S(glPopMatrix);
        GB_GL_INTERFACE_LOAD_S(glPushMatrix);
        GB_GL_INTERFACE_LOAD_S(glRotatef);
        GB_GL_INTERFACE_LOAD_S(glScalef);
        GB_GL_INTERFACE_LOAD_S(glTexCoordPointer);
        GB_GL_INTERFACE_LOAD_S(glTexEnvi);
        GB_GL_INTERFACE_LOAD_S(glTranslatef);
        GB_GL_INTERFACE_LOAD_S(glVertexPointer);

#   ifndef TB_CONFIG_OS_WINDOWS
        // load interfaces for gl >= 2.0
        GB_GL_INTERFACE_LOAD_S(glAttachShader);
        GB_GL_INTERFACE_LOAD_S(glCompileShader);
        GB_GL_INTERFACE_LOAD_S(glCreateProgram);
        GB_GL_INTERFACE_LOAD_S(glCreateShader);
        GB_GL_INTERFACE_LOAD_S(glDeleteProgram);
        GB_GL_INTERFACE_LOAD_S(glDeleteShader);
        GB_GL_INTERFACE_LOAD_S(glDisableVertexAttribArray);
        GB_GL_INTERFACE_LOAD_S(glEnableVertexAttribArray);
        GB_GL_INTERFACE_LOAD_S(glGetAttribLocation);
        GB_GL_INTERFACE_LOAD_S(glGetProgramiv);
        GB_GL_INTERFACE_LOAD_S(glGetProgramInfoLog);
        GB_GL_INTERFACE_LOAD_S(glGetShaderiv);
        GB_GL_INTERFACE_LOAD_S(glGetShaderInfoLog);
        GB_GL_INTERFACE_LOAD_S(glGetUniformLocation);
        GB_GL_INTERFACE_LOAD_S(glLinkProgram);
        GB_GL_INTERFACE_LOAD_S(glShaderSource);
        GB_GL_INTERFACE_LOAD_S(glUniform1i);
        GB_GL_INTERFACE_LOAD_S(glUniformMatrix4fv);
        GB_GL_INTERFACE_LOAD_S(glUseProgram);
        GB_GL_INTERFACE_LOAD_S(glVertexAttrib4f);
        GB_GL_INTERFACE_LOAD_S(glVertexAttribPointer);
#   endif
#endif

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
