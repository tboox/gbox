# prefix
include $(PRO_DIR)/prefix.mak

# module name
NAMES 					= gbox

# module type
gbox_TYPE 				= LIB

# core files
gbox_C_FILES 			+= \
						gbox \
						core/core \
						core/matrix \
						core/pixmap \
						core/bitmap \
						core/paint \
						core/canvas \
						core/device \
						core/device/bitmap \
						core/prefix/color \
						core/prefix/quality \
						core/bitmap/decoder \
						core/bitmap/decoder/bmp 


# gl core files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GL) += \
						core/device/gl \
						core/device/gl/interface 

# skia core files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_SKIA) += \
						core/device/skia

# platform files
gbox_C_FILES 			+= \
						platform/platform \
						platform/event \
						platform/window \
						platform/window/prefix 

gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GLUT) += \
						platform/window/glut

gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_X11) += \
 						platform/window/x11

gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_SDL) += \
 						platform/window/sdl

gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_FRAMEBUFFER) += \
 						platform/window/framebuffer

# ios files
ifeq ($(PLAT),ios)
gbox_M_FILES 			+= \
						platform/window/ios
endif

# android files
ifeq ($(PLAT),android)
gbox_C_FILES 			+= \
 						platform/window/android
endif

# flags
gbox_CXFLAGS 			+= -D__tb_prefix__=\"gbox\"

# header files
INC_FILES 				= \
						gbox.h \
						config.h \
						prefix.h \
						prefix/prefix.h \
						prefix/config.h \
						prefix/version.h \
						ui/ui.h \
						ui/prefix.h \
						core/core.h \
						core/prefix.h \
						core/prefix/type.h \
						core/prefix/float.h \
						core/prefix/color.h \
						core/prefix/pixfmt.h \
						core/prefix/prefix.h \
						core/prefix/quality.h \
						core/pixmap.h \
						core/bitmap.h \
						core/matrix.h \
						core/paint.h \
						core/canvas.h \
						core/device.h \
						core/svg/svg.h \
						core/svg/prefix.h \
						platform/prefix.h \
						platform/touch.h \
						platform/event.h \
						platform/window.h \
						engine/engine.h \
						engine/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

