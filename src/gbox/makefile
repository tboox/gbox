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
						core/impl/arc \
						core/impl/quad \
						core/impl/cubic \
						core/impl/float \
						core/impl/stroker \
						core/impl/mesh \
						core/impl/mesh_edge \
						core/impl/mesh_face \
						core/impl/mesh_vertex \
						core/impl/geometry \
						core/impl/polygon_tesselator \
						core/impl/polygon_raster 


# bitmap device files
gbox_C_FILES-$(GB_CONFIG_DEVICE_HAVE_BITMAP) += \
						core/device/bitmap \
						core/device/bitmap/render \
						core/device/bitmap/biltter \
						core/device/bitmap/biltter/solid \
						core/device/bitmap/biltter/shader \
						core/device/bitmap/render/rect \
						core/device/bitmap/render/lines \
						core/device/bitmap/render/points \
						core/device/bitmap/render/polygon 


# gl device files
gbox_C_FILES-$(GB_CONFIG_DEVICE_HAVE_GL) += \
						core/device/gl \
						core/device/gl/render \
						core/device/gl/shader \
						core/device/gl/interface \
						core/device/gl/program \
						core/device/gl/program/color \
						core/device/gl/program/bitmap 

# skia device files
gbox_CPP_FILES-$(GB_CONFIG_DEVICE_HAVE_SKIA) += \
						core/device/skia

# platform files
gbox_C_FILES 			+= \
						platform/platform \
						platform/event \
						platform/window \
						platform/application \
						platform/impl/window 

# glut window files
gbox_C_FILES-$(GB_CONFIG_WINDOW_HAVE_GLUT) += \
						platform/glut/window

# sdl window files
gbox_C_FILES-$(GB_CONFIG_WINDOW_HAVE_SDL) += \
 						platform/sdl/window

# x11 window files
gbox_C_FILES-$(GB_CONFIG_WINDOW_HAVE_X11) += \
 						platform/x11/window

# framebuffer window files
gbox_C_FILES-$(GB_CONFIG_WINDOW_HAVE_FRAMEBUFFER) += \
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

