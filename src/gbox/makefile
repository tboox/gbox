# prefix
include $(PRO_DIR)/prefix.mak

# module name
NAMES 					= gbox

# module type
gbox_TYPE 				= LIB

# basic files
gbox_C_FILES 			+= \
						gbox
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
						game/game.h \
						game/prefix.h 		

# demo
SUB_PROS-$(DEMO) 		+= ../demo

# suffix
include $(PRO_DIR)/suffix.mak

