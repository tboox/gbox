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
						core/prefix/color \
						core/prefix/quality \
						core/bitmap/decoder \
						core/bitmap/decoder/bmp 

# gl core files
gbox_C_FILES-$(GB_CONFIG_CORE_DEVICE_HAVE_GL) += \
						core/device/gl

# bitmap core files
gbox_C_FILES-$(GB_CONFIG_CORE_DEVICE_HAVE_BITMAP) += \
						core/device/bitmap

# skia core files
gbox_C_FILES-$(GB_CONFIG_CORE_DEVICE_HAVE_SKIA) += \
						core/device/skia

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
						core/3d/prefix.h \
						core/3d/3d.h \
						core/svg/svg.h \
						core/svg/prefix.h \
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

