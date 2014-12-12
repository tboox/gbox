# preifx
include $(PRO_DIR)/prefix.mak

# module name
NAMES 											= demo

# type
demo_TYPE 										= BIN

# files
demo_C_FILES 									+= \
												demo \
												core/path \
												core/mesh \
												core/vector \
												core/bitmap \
												other/test

# libs
demo_LIBS 										+= gbox$(DTYPE) tbox$(DTYPE) 
demo_LIBS-y 									+= $(GB_CONFIG_DEMO_LIBS)
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_PNG)  			+= png 
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_JPEG) 			+= jpeg 
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_FREETYPE) 		+= freetype bz2
demo_LIBS-$(GB_CONFIG_DEVICE_HAVE_SKIA)  		+= skia 
demo_LIBS-$(GB_CONFIG_WINDOW_HAVE_SDL)  		+= SDL 

# path
demo_INC_DIR 									+= ../../
demo_LIB_DIR 									+= ../../gbox

# flags
demo_CXFLAGS 									+= -D__tb_prefix__=\"demo\"
demo_LDFLAGS 									+= $(GB_CONFIG_DEMO_LDFLAGS)

# command
demo_SUFFIX_CMD1 								+= $(GB_CONFIG_DEMO_CMD1)

# demo
SUB_PROS  										+= ../core

# suffix
include $(PRO_DIR)/suffix.mak

