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
						core/color \
						core/matrix \
						core/pixmap \
						core/bitmap \
						core/paint \
						core/canvas \
						core/image/decoder \
						core/image/decoder/bmp 

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
						core/type.h \
						core/float.h \
						core/color.h \
						core/pixfmt.h \
						core/pixmap.h \
						core/bitmap.h \
						core/matrix.h \
						core/paint.h \
						core/canvas.h \
						core/3d/prefix.h \
						core/3d/3d.h \
						core/image/image.h \
						core/image/prefix.h \
						core/image/decoder.h \
						core/svg/svg.h \
						core/svg/prefix.h \
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

