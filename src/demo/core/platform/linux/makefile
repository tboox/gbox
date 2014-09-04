# preifx
include $(PRO_DIR)/prefix.mak

# module name
NAMES 										= demo

# type
demo_TYPE 									= BIN

# files
demo_C_FILES 								+= \
											demo \
											../../arc \
											../../rect \
											../../line \
											../../point \
											../../circle \
											../../ellipse \
											../../triangle \
											../../demo

# libs
demo_LIBS 									+= gbox$(DTYPE) tbox$(DTYPE) 
demo_LIBS-y 								+= $(GB_CONFIG_DEMO_LIBS)

# path
demo_INC_DIR 								+= ../../../../
demo_LIB_DIR 								+= ../../../../gbox

# flags
demo_CXFLAGS 								+= -D__tb_prefix__=\"demo\"
demo_LDFLAGS 								+= $(GB_CONFIG_DEMO_LDFLAGS)

# command
demo_SUFFIX_CMD1 							+= $(GB_CONFIG_DEMO_CMD1)

# suffix
include $(PRO_DIR)/suffix.mak

