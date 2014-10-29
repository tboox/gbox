#ifndef GB_CONFIG_H
#define GB_CONFIG_H

// major version
#define GB_CONFIG_VERSION_MAJOR         [major]

// minor version
#define GB_CONFIG_VERSION_MINOR         [minor]

// alter version
#define GB_CONFIG_VERSION_ALTER         [alter]

// build version
#define GB_CONFIG_VERSION_BUILD         [build]

// small
#define GB_CONFIG_SMALL                 [small]

// the fixed type for float
#define GB_CONFIG_FLOAT_FIXED

// gl
#define GB_CONFIG_GL_APICALL            __tb_cdecl__

// device
#define GB_CONFIG_DEVICE_HAVE_GL
//#define GB_CONFIG_DEVICE_HAVE_SKIA
#define GB_CONFIG_DEVICE_HAVE_BITMAP

// window
#define GB_CONFIG_WINDOW_HAVE_GLUT
#define GB_CONFIG_WINDOW_HAVE_SDL
//#define GB_CONFIG_WINDOW_HAVE_X11
//#define GB_CONFIG_WINDOW_HAVE_FRAMEBUFFER

// application
//#define GB_CONFIG_APPLICATION_WINDOW_GLUT
//#define GB_CONFIG_APPLICATION_WINDOW_SDL
//#define GB_CONFIG_APPLICATION_WINDOW_FRAMEBUFFER

// third party
#define GB_CONFIG_THIRD_HAVE_PNG
//#define GB_CONFIG_THIRD_HAVE_JPG
//#define GB_CONFIG_THIRD_HAVE_FREETYPE

#endif
