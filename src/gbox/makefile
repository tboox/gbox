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
						core/device/gl 

gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GLUT) += \
						core/gl/view/glut

# skia core files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_SKIA) += \
						core/device/skia

# ios files
ifeq ($(PLAT),ios)
gbox_M_FILES-$(GB_CONFIG_THIRD_HAVE_GL) += \
 						core/gl/view/ios
endif

# android files
ifeq ($(PLAT),android)
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GL) += \
 						core/gl/view/android
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
						core/gl/prefix.h \
						core/gl/gl.h \
						core/gl/view.h \
						core/svg/svg.h \
						core/svg/prefix.h \
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

