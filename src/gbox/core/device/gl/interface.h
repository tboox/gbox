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
 * @file        interface.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_GL_INTERFACE_H
#define GB_CORE_GL_INTERFACE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// gl apicall
#define GB_GL_APICALL                                       GB_CONFIG_GL_APICALL

// type
#define GB_GL_INTERFACE_TYPE(func)                          GB_GL_APICALL *gb_##func##_t

// extern
#define GB_GL_INTERFACE_EXTERN(func)                        extern gb_##func##_t gb_##func

// load the static function
#define GB_GL_INTERFACE_LOAD_S(func)                        gb_##func = func

// load the dynamic function
#define GB_GL_INTERFACE_LOAD_D(library, func)               gb_##func = (gb_##func##_t)tb_dynamic_func(library, #func); tb_assert_and_check_break(gb_##func)

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for gl
 */

// data type 
#define GB_GL_BYTE                      (0x1400)
#define GB_GL_UNSIGNED_BYTE             (0x1401)
#define GB_GL_SHORT                     (0x1402)
#define GB_GL_UNSIGNED_SHORT            (0x1403)
#define GB_GL_FLOAT                     (0x1406)
#define GB_GL_FIXED                     (0x140C)

// boolean
#define GB_GL_FALSE                     (0)
#define GB_GL_TRUE                      (1)

// string name
#define GB_GL_VENDOR                    (0x1F00)
#define GB_GL_RENDERER                  (0x1F01)
#define GB_GL_VERSION                   (0x1F02)
#define GB_GL_EXTENSIONS                (0x1F03)

// matrix mode
#define GB_GL_MATRIX_MODE               (0x0BA0)
#define GB_GL_MODELVIEW                 (0x1700)
#define GB_GL_PROJECTION                (0x1701)
#define GB_GL_TEXTURE                   (0x1702)

// vertex arrays
#define GB_GL_VERTEX_ARRAY              (0x8074)

// color arrays
#define GB_GL_COLOR_ARRAY               (0x8076)

// texture arrays
#define GB_GL_TEXTURE_COORD_ARRAY       (0x8078)

// texture parameter name
#define GB_GL_TEXTURE_WRAP_S            (0x2802)
#define GB_GL_TEXTURE_WRAP_T            (0x2803)
#define GB_GL_TEXTURE_MAG_FILTER        (0x2800)
#define GB_GL_TEXTURE_MIN_FILTER        (0x2801)

// texture env parameter
#define GB_GL_TEXTURE_ENV_MODE          (0x2200)
#define gb_GL_TEXTURE_ENV_COLOR         (0x2201)

// texture env target
#define GB_GL_TEXTURE_ENV               (0x2300)

// texture env mode
#define GB_GL_MODULATE                  (0x2100)
#define GB_GL_DECAL                     (0x2101)
//#define GB_GL_BLEND
#define GB_GL_ADD                       (0x0104)
//#define GB_GL_REPLACE

// texture mag filter
#define GB_GL_NEAREST                   (0x2600)
#define GB_GL_LINEAR                    (0x2601)

// blending
#define GB_GL_BLEND                     (0x0BE2)
#define GB_GL_BLEND_SRC                 (0x0BE1)
#define GB_GL_BLEND_DST                 (0x0BE0)
#define GB_GL_ZERO                      (0x0)
#define GB_GL_ONE                       (0x1)
#define GB_GL_SRC_COLOR                 (0x0300)
#define GB_GL_ONE_MINUS_SRC_COLOR       (0x0301)
#define GB_GL_SRC_ALPHA                 (0x0302)
#define GB_GL_ONE_MINUS_SRC_ALPHA       (0x0303)
#define GB_GL_DST_ALPHA                 (0x0304)
#define GB_GL_ONE_MINUS_DST_ALPHA       (0x0305)
#define GB_GL_DST_COLOR                 (0x0306)
#define GB_GL_ONE_MINUS_DST_COLOR       (0x0307)
#define GB_GL_SRC_ALPHA_SATURATE        (0x0308)

// multisample
#define GB_GL_MULTISAMPLE               (0x809D)
#define GB_GL_LINE_SMOOTH               (0x0B20)
#define GB_GL_LINE_SMOOTH_HINT          (0x0C52)
#define GB_GL_NICEST                    (0x1102)

// stencil op
//#define GB_GL_ZERO
#define GB_GL_KEEP                      (0x1E00)
#define GB_GL_REPLACE                   (0x1E01)
#define GB_GL_INCR                      (0x1E02)
#define GB_GL_DECR                      (0x1E03)
#define GB_GL_INVERT                    (0x150A)
#define GB_GL_INCR_WRAP                 (0x8507)
#define GB_GL_DECR_WRAP                 (0x8508)

// stencil function
#define GB_GL_NEVER                     (0x0200)
#define GB_GL_LESS                      (0x0201)
#define GB_GL_EQUAL                     (0x0202)
#define GB_GL_LEQUAL                    (0x0203)
#define GB_GL_GREATER                   (0x0204)
#define GB_GL_NOTEQUAL                  (0x0205)
#define GB_GL_GEQUAL                    (0x0206)
#define GB_GL_ALWAYS                    (0x0207)

// enable cap
#define GB_GL_TEXTURE_2D                (0x0DE1)
#define GB_GL_BLEND                     (0x0BE2)
#define GB_GL_DITHER                    (0x0BD0)
#define GB_GL_STENCIL_TEST              (0x0B90)
#define GB_GL_DEPTH_TEST                (0x0B71)
#define GB_GL_SCISSOR_TEST              (0x0C11)

// texture wrap mode
#define GB_GL_REPEAT                    (0x2901)
#define GB_GL_CLAMP_TO_EDGE             (0x812F)
#define GB_GL_MIRRORED_REPEAT           (0x8370)
#define GB_GL_CLAMP_TO_BORDER           (0x812D)

// clear buffer mask 
#define GB_GL_DEPTH_BUFFER_BIT          (0x00000100)
#define GB_GL_STENCIL_BUFFER_BIT        (0x00000400)
#define GB_GL_COLOR_BUFFER_BIT          (0x00004000)

// pixel format 
#define GB_GL_ALPHA                     (0x1906)
#define GB_GL_RGB                       (0x1907)
#define GB_GL_RGBA                      (0x1908)
#define GB_GL_LUMINANCE                 (0x1909)
#define GB_GL_LUMINANCE_ALPHA           (0x190A)
#define GB_GL_BGR                       (0x80E0)
#define GB_GL_BGRA                      (0x80E1)

// pixel type
//#define GB_GL_UNSIGNED_BYTE
#define GB_GL_UNSIGNED_SHORT_4_4_4_4    (0x8033)
#define GB_GL_UNSIGNED_SHORT_5_5_5_1    (0x8034)
#define GB_GL_UNSIGNED_SHORT_5_6_5      (0x8363)

// pixel store parameter
#define GB_GL_UNPACK_ALIGNMENT          (0x0CF5)
#define GB_GL_PACK_ALIGNMENT            (0x0D05)

// begin mode 
#define GB_GL_POINTS                    (0x0000)
#define GB_GL_LINES                     (0x0001)
#define GB_GL_LINE_LOOP                 (0x0002)
#define GB_GL_LINE_STRIP                (0x0003)
#define GB_GL_TRIANGLES                 (0x0004)
#define GB_GL_TRIANGLE_STRIP            (0x0005)
#define GB_GL_TRIANGLE_FAN              (0x0006)

// shaders
#define GB_GL_FRAGMENT_SHADER           (0x8B30)
#define GB_GL_VERTEX_SHADER             (0x8B31)
#define GB_GL_SHADER_TYPE               (0x8B4F)
#define GB_GL_DELETE_STATUS             (0x8B80)
#define GB_GL_LINK_STATUS               (0x8B82)
#define GB_GL_VALIDATE_STATUS           (0x8B83)
#define GB_GL_ATTACHED_SHADERS          (0x8B85)

// shader source
#define GB_GL_COMPILE_STATUS            (0x8B81)
#define GB_GL_INFO_LOG_LENGTH           (0x8B84)
#define GB_GL_SHADER_SOURCE_LENGTH      (0x8B88)
#define GB_GL_SHADER_COMPILER           (0x8DFA)

// texture unit
#define GB_GL_TEXTURE0                  (0x84C0)
#define GB_GL_TEXTURE1                  (0x84C1)
#define GB_GL_TEXTURE2                  (0x84C2)
#define GB_GL_TEXTURE3                  (0x84C3)
#define GB_GL_TEXTURE4                  (0x84C4)
#define GB_GL_TEXTURE5                  (0x84C5)
#define GB_GL_TEXTURE6                  (0x84C6)
#define GB_GL_TEXTURE7                  (0x84C7)
#define GB_GL_TEXTURE8                  (0x84C8)
#define GB_GL_TEXTURE9                  (0x84C9)
#define GB_GL_TEXTURE10                 (0x84CA)
#define GB_GL_TEXTURE11                 (0x84CB)
#define GB_GL_TEXTURE12                 (0x84CC)
#define GB_GL_TEXTURE13                 (0x84CD)
#define GB_GL_TEXTURE14                 (0x84CE)
#define GB_GL_TEXTURE15                 (0x84CF)
#define GB_GL_TEXTURE16                 (0x84D0)
#define GB_GL_TEXTURE17                 (0x84D1)
#define GB_GL_TEXTURE18                 (0x84D2)
#define GB_GL_TEXTURE19                 (0x84D3)
#define GB_GL_TEXTURE20                 (0x84D4)
#define GB_GL_TEXTURE21                 (0x84D5)
#define GB_GL_TEXTURE22                 (0x84D6)
#define GB_GL_TEXTURE23                 (0x84D7)
#define GB_GL_TEXTURE24                 (0x84D8)
#define GB_GL_TEXTURE25                 (0x84D9)
#define GB_GL_TEXTURE26                 (0x84DA)
#define GB_GL_TEXTURE27                 (0x84DB)
#define GB_GL_TEXTURE28                 (0x84DC)
#define GB_GL_TEXTURE29                 (0x84DD)
#define GB_GL_TEXTURE30                 (0x84DE)
#define GB_GL_TEXTURE31                 (0x84DF)
#define GB_GL_ACTIVE_TEXTURE            (0x84E0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * basic type
 */

typedef tb_uint_t       gb_GLenum_t;
typedef tb_char_t       gb_GLchar_t;
typedef tb_byte_t       gb_GLboolean_t;
typedef tb_uint_t       gb_GLbitfield_t;
typedef tb_void_t       gb_GLvoid_t;
typedef tb_char_t       gb_GLbyte_t;
typedef tb_ushort_t     gb_GLushort_t;
typedef tb_short_t      gb_GLshort_t;
typedef tb_int_t        gb_GLint_t;
typedef tb_byte_t       gb_GLubyte_t;
typedef tb_uint_t       gb_GLuint_t;
typedef tb_int_t        gb_GLsizei_t;
typedef tb_float_t      gb_GLfloat_t;
typedef tb_float_t      gb_GLclampf_t;
typedef tb_double_t     gb_GLdouble_t;
typedef tb_double_t     gb_GLclampd_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface types
 */
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glActiveTexture))             (gb_GLenum_t texture);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glAlphaFunc))                 (gb_GLenum_t func, gb_GLclampf_t ref);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glAttachShader))              (gb_GLuint_t program, gb_GLuint_t shader);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glBindTexture))               (gb_GLenum_t target, gb_GLuint_t texture);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glBlendFunc))                 (gb_GLenum_t sfactor, gb_GLenum_t dfactor);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glClear))                     (gb_GLbitfield_t mask);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glClearColor))                (gb_GLclampf_t red, gb_GLclampf_t green, gb_GLclampf_t blue, gb_GLclampf_t alpha);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glClearStencil))              (gb_GLint_t s);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glColor4f))                   (gb_GLfloat_t red, gb_GLfloat_t green, gb_GLfloat_t blue, gb_GLfloat_t alpha);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glColorMask))                 (gb_GLboolean_t red, gb_GLboolean_t green, gb_GLboolean_t blue, gb_GLboolean_t alpha);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glColorPointer))              (gb_GLint_t size, gb_GLenum_t type, gb_GLsizei_t stride, gb_GLvoid_t const* pointer);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glCompileShader))             (gb_GLuint_t shader);
typedef gb_GLuint_t             (GB_GL_INTERFACE_TYPE(glCreateProgram))             (gb_GLvoid_t);
typedef gb_GLuint_t             (GB_GL_INTERFACE_TYPE(glCreateShader))              (gb_GLenum_t type);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDeleteProgram))             (gb_GLuint_t program);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDeleteShader))              (gb_GLuint_t shader);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDeleteTextures))            (gb_GLsizei_t n, gb_GLuint_t const* textures);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDisable))                   (gb_GLenum_t cap);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDisableClientState))        (gb_GLenum_t cap);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDisableVertexAttribArray))  (gb_GLuint_t index);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glDrawArrays))                (gb_GLenum_t mode, gb_GLint_t first, gb_GLsizei_t count);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glEnable))                    (gb_GLenum_t cap);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glEnableClientState))         (gb_GLenum_t cap);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glEnableVertexAttribArray))   (gb_GLuint_t index);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glGenTextures))               (gb_GLsizei_t n, gb_GLuint_t* textures);
typedef gb_GLint_t              (GB_GL_INTERFACE_TYPE(glGetAttribLocation))         (gb_GLuint_t program, gb_GLchar_t const* name);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glGetProgramiv))              (gb_GLuint_t program, gb_GLenum_t pname, gb_GLint_t* params);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glGetProgramInfoLog))         (gb_GLuint_t program, gb_GLsizei_t bufsize, gb_GLsizei_t* length, gb_GLchar_t* infolog);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glGetShaderiv))               (gb_GLuint_t shader, gb_GLenum_t pname, gb_GLint_t* params);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glGetShaderInfoLog))          (gb_GLuint_t shader, gb_GLsizei_t bufsize, gb_GLsizei_t* length, gb_GLchar_t* infolog);
typedef gb_GLubyte_t const*     (GB_GL_INTERFACE_TYPE(glGetString))                 (gb_GLenum_t name);
typedef gb_GLint_t              (GB_GL_INTERFACE_TYPE(glGetUniformLocation))        (gb_GLuint_t program, gb_GLchar_t const* name);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glHint))                      (gb_GLenum_t target, gb_GLenum_t mode);
typedef gb_GLboolean_t          (GB_GL_INTERFACE_TYPE(glIsTexture))                 (gb_GLuint_t texture);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glLineWidth))                 (gb_GLfloat_t width);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glLinkProgram))               (gb_GLuint_t program);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glLoadIdentity))              (gb_GLvoid_t);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glLoadMatrixf))               (gb_GLfloat_t const* matrix);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glMatrixMode))                (gb_GLenum_t mode);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glMultMatrixf))               (gb_GLfloat_t const* matrix);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glOrtho))                     (gb_GLdouble_t left, gb_GLdouble_t right, gb_GLdouble_t bottom, gb_GLdouble_t top, gb_GLdouble_t near, gb_GLdouble_t far);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glOrthof))                    (gb_GLfloat_t left, gb_GLfloat_t right, gb_GLfloat_t bottom, gb_GLfloat_t top, gb_GLfloat_t near, gb_GLfloat_t far);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glPixelStorei))               (gb_GLenum_t pname, gb_GLint_t param);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glPopMatrix))                 (gb_GLvoid_t);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glPushMatrix))                (gb_GLvoid_t);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glRotatef))                   (gb_GLfloat_t angle, gb_GLfloat_t x, gb_GLfloat_t y, gb_GLfloat_t z);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glScalef))                    (gb_GLfloat_t x, gb_GLfloat_t y, gb_GLfloat_t z);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glScissor))                   (gb_GLint_t x, gb_GLint_t y, gb_GLsizei_t width, gb_GLsizei_t height);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glShaderSource))              (gb_GLuint_t shader, gb_GLsizei_t count, gb_GLchar_t const* const* string, const gb_GLint_t* length);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glStencilFunc))               (gb_GLenum_t func, gb_GLint_t ref, gb_GLuint_t mask);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glStencilMask))               (gb_GLuint_t mask);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glStencilOp))                 (gb_GLenum_t fail, gb_GLenum_t zfail, gb_GLenum_t zpass);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTexCoordPointer))           (gb_GLint_t size, gb_GLenum_t type, gb_GLsizei_t stride, gb_GLvoid_t const* ptr);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTexEnvi))                   (gb_GLenum_t target, gb_GLenum_t pname, gb_GLint_t param);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTexImage2D))                (gb_GLenum_t target, gb_GLint_t level, gb_GLint_t internalFormat, gb_GLsizei_t width, gb_GLsizei_t height, gb_GLint_t border, gb_GLenum_t format, gb_GLenum_t type, gb_GLvoid_t const* pixels);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTexParameterf))             (gb_GLenum_t target, gb_GLenum_t pname, gb_GLfloat_t param);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTexParameteri))             (gb_GLenum_t target, gb_GLenum_t pname, gb_GLint_t param);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glTranslatef))                (gb_GLfloat_t x, gb_GLfloat_t y, gb_GLfloat_t z);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glUniform1i))                 (gb_GLint_t location, gb_GLint_t x);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glUniformMatrix4fv))          (gb_GLint_t location, gb_GLsizei_t count, gb_GLboolean_t transpose, gb_GLfloat_t const* value);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glUseProgram))                (gb_GLuint_t program);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glVertexAttrib4f))            (gb_GLuint_t indx, gb_GLfloat_t x, gb_GLfloat_t y, gb_GLfloat_t z, gb_GLfloat_t w);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glVertexAttribPointer))       (gb_GLuint_t indx, gb_GLint_t size, gb_GLenum_t type, gb_GLboolean_t normalized, gb_GLsizei_t stride, gb_GLvoid_t const* ptr);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glVertexPointer))             (gb_GLint_t size, gb_GLenum_t type, gb_GLsizei_t stride,  gb_GLvoid_t const* ptr);
typedef gb_GLvoid_t             (GB_GL_INTERFACE_TYPE(glViewport))                  (gb_GLint_t x, gb_GLint_t y, gb_GLsizei_t width, gb_GLsizei_t height);

/* //////////////////////////////////////////////////////////////////////////////////////
 * functions
 */
__tb_extern_c_enter__

GB_GL_INTERFACE_EXTERN(glActiveTexture);
GB_GL_INTERFACE_EXTERN(glAlphaFunc);
GB_GL_INTERFACE_EXTERN(glAttachShader);
GB_GL_INTERFACE_EXTERN(glBindTexture);
GB_GL_INTERFACE_EXTERN(glBlendFunc);
GB_GL_INTERFACE_EXTERN(glClear);
GB_GL_INTERFACE_EXTERN(glClearColor);
GB_GL_INTERFACE_EXTERN(glClearStencil);
GB_GL_INTERFACE_EXTERN(glColor4f);
GB_GL_INTERFACE_EXTERN(glColorMask);
GB_GL_INTERFACE_EXTERN(glColorPointer);
GB_GL_INTERFACE_EXTERN(glCompileShader);
GB_GL_INTERFACE_EXTERN(glCreateProgram);
GB_GL_INTERFACE_EXTERN(glCreateShader);
GB_GL_INTERFACE_EXTERN(glDeleteProgram);
GB_GL_INTERFACE_EXTERN(glDeleteShader);
GB_GL_INTERFACE_EXTERN(glDeleteTextures);
GB_GL_INTERFACE_EXTERN(glDisable);
GB_GL_INTERFACE_EXTERN(glDisableClientState);
GB_GL_INTERFACE_EXTERN(glDisableVertexAttribArray);
GB_GL_INTERFACE_EXTERN(glDrawArrays);
GB_GL_INTERFACE_EXTERN(glEnable);
GB_GL_INTERFACE_EXTERN(glEnableClientState);
GB_GL_INTERFACE_EXTERN(glEnableVertexAttribArray);
GB_GL_INTERFACE_EXTERN(glGenTextures);
GB_GL_INTERFACE_EXTERN(glGetAttribLocation);
GB_GL_INTERFACE_EXTERN(glGetProgramiv);
GB_GL_INTERFACE_EXTERN(glGetProgramInfoLog);
GB_GL_INTERFACE_EXTERN(glGetShaderiv);
GB_GL_INTERFACE_EXTERN(glGetShaderInfoLog);
GB_GL_INTERFACE_EXTERN(glGetString);
GB_GL_INTERFACE_EXTERN(glGetUniformLocation);
GB_GL_INTERFACE_EXTERN(glHint);
GB_GL_INTERFACE_EXTERN(glIsTexture);
GB_GL_INTERFACE_EXTERN(glLineWidth);
GB_GL_INTERFACE_EXTERN(glLinkProgram);
GB_GL_INTERFACE_EXTERN(glLoadIdentity);
GB_GL_INTERFACE_EXTERN(glLoadMatrixf);
GB_GL_INTERFACE_EXTERN(glMatrixMode);
GB_GL_INTERFACE_EXTERN(glMultMatrixf);
GB_GL_INTERFACE_EXTERN(glOrtho);
GB_GL_INTERFACE_EXTERN(glOrthof);
GB_GL_INTERFACE_EXTERN(glPixelStorei);
GB_GL_INTERFACE_EXTERN(glPopMatrix);
GB_GL_INTERFACE_EXTERN(glPushMatrix);
GB_GL_INTERFACE_EXTERN(glRotatef);
GB_GL_INTERFACE_EXTERN(glScalef);
GB_GL_INTERFACE_EXTERN(glScissor);
GB_GL_INTERFACE_EXTERN(glShaderSource);
GB_GL_INTERFACE_EXTERN(glStencilFunc);
GB_GL_INTERFACE_EXTERN(glStencilMask);
GB_GL_INTERFACE_EXTERN(glStencilOp);
GB_GL_INTERFACE_EXTERN(glTexCoordPointer);
GB_GL_INTERFACE_EXTERN(glTexEnvi);
GB_GL_INTERFACE_EXTERN(glTexImage2D);
GB_GL_INTERFACE_EXTERN(glTexParameterf);
GB_GL_INTERFACE_EXTERN(glTexParameteri);
GB_GL_INTERFACE_EXTERN(glTranslatef);
GB_GL_INTERFACE_EXTERN(glUniform1i);
GB_GL_INTERFACE_EXTERN(glUniformMatrix4fv);
GB_GL_INTERFACE_EXTERN(glUseProgram);
GB_GL_INTERFACE_EXTERN(glVertexAttrib4f);
GB_GL_INTERFACE_EXTERN(glVertexAttribPointer);
GB_GL_INTERFACE_EXTERN(glVertexPointer);
GB_GL_INTERFACE_EXTERN(glViewport);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interface
 */

/* load gl interfaces
 *
 * @return          the gl version
 */
tb_bool_t           gb_gl_interface_load(tb_noarg_t);

__tb_extern_c_leave__
#endif
