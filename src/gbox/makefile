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
						core/path \
						core/paint \
						core/shader \
						core/pixmap \
						core/bitmap \
						core/canvas \
						core/device \
						core/clipper \
						core/device/bitmap \
						core/device/bitmap/render \
						core/device/bitmap/render/stroke/lines \
						core/device/bitmap/render/stroke/points \
						core/device/bitmap/render/stroke/polygon \
						core/device/bitmap/render/fill/filler \
						core/device/bitmap/render/fill/rect \
						core/device/bitmap/render/fill/solid \
						core/device/bitmap/render/fill/polygon \
						core/prefix/prefix \
						core/prefix/arc \
						core/prefix/line \
						core/prefix/rect \
						core/prefix/color \
						core/prefix/point \
						core/prefix/vector \
						core/prefix/matrix \
						core/prefix/circle \
						core/prefix/ellipse \
						core/prefix/quality \
						core/prefix/triangle \
						core/prefix/round_rect \
						core/bitmap/decoder \
						core/bitmap/decoder/bmp \
						core/impl/path_cache \
						core/impl/cache_stack \
						core/impl/cache_stack \
						core/impl/stroker \
						core/impl/arc \
						core/impl/quad \
						core/impl/cubic \
						core/impl/polygon_raster 


# gl core files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GL) += \
						core/device/gl \
						core/device/gl/render \
						core/device/gl/shader \
						core/device/gl/interface \
						core/device/gl/program \
						core/device/gl/program/color \
						core/device/gl/program/bitmap 

# skia core files
gbox_CPP_FILES-$(GB_CONFIG_THIRD_HAVE_SKIA) += \
						core/device/skia

# platform files
gbox_C_FILES 			+= \
						platform/platform \
						platform/event \
						platform/window \
						platform/application \
						platform/impl/window 

# glut files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_GLUT) += \
						platform/glut/window

# sdl files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_SDL) += \
 						platform/sdl/window

# x11 files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_X11) += \
 						platform/x11/window

# framebuffer files
gbox_C_FILES-$(GB_CONFIG_THIRD_HAVE_FRAMEBUFFER) += \
 						platform/framebuffer/window

# ios files
ifeq ($(PLAT),ios)
gbox_M_FILES 			+= \
						platform/ios/window
endif

# android files
ifeq ($(PLAT),android)
gbox_C_FILES 			+= \
 						platform/android/window
endif

# flags
gbox_CXFLAGS 			+= -D__tb_prefix__=\"gbox\"

# header files
INC_FILES 				= \
						gbox.h \
						gbox.config.h \
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
						core/prefix/arc.h \
						core/prefix/line.h \
						core/prefix/rect.h \
						core/prefix/point.h \
						core/prefix/float.h \
						core/prefix/color.h \
						core/prefix/circle.h \
						core/prefix/vector.h \
						core/prefix/matrix.h \
						core/prefix/pixfmt.h \
						core/prefix/prefix.h \
						core/prefix/ellipse.h \
						core/prefix/quality.h \
						core/prefix/triangle.h \
						core/prefix/round_rect.h \
						core/path.h \
						core/paint.h \
						core/shader.h \
						core/pixmap.h \
						core/bitmap.h \
						core/canvas.h \
						core/device.h \
						core/clipper.h \
						core/svg/svg.h \
						core/svg/prefix.h \
						platform/prefix.h \
						platform/touch.h \
						platform/event.h \
						platform/window.h \
						platform/application.h \
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

