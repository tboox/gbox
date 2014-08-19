# prefix
include $(PRO_DIR)/prefix.mak

# module name
NAMES 					= gbox

# module type
gbox_TYPE 				= LIB

# core files
gbox_C_FILES 			+= \
						gbox \
						core/color \
						core/matrix
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
						svg/svg.h \
						svg/prefix.h \
						core/core.h \
						core/prefix.h \
						core/type.h \
						core/float.h \
						core/color.h \
						core/pixfmt.h \
						core/matrix.h \
						core/3d/prefix.h \
						core/3d/3d.h \
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

