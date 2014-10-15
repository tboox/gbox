# preifx
include $(PRO_DIR)/prefix.mak

# module name
NAMES 												= application

# type
application_TYPE 									= BIN

# files
application_C_FILES 								+= \
													main \
													../../application \
													../../demo \
													../../arc \
													../../rect \
													../../path \
													../../quad \
													../../cubic \
													../../line \
													../../lines \
													../../point \
													../../points \
													../../circle \
													../../ellipse \
													../../triangle \
													../../round_rect  

# libs
application_LIBS 									+= gbox$(DTYPE) tbox$(DTYPE)
application_LIBS-y 									+= $(GB_CONFIG_DEMO_LIBS)
application_LIBS-$(GB_CONFIG_THIRD_HAVE_PNG)  		+= png 
application_LIBS-$(GB_CONFIG_THIRD_HAVE_JPG)  		+= jpeg 
application_LIBS-$(GB_CONFIG_THIRD_HAVE_FREETYPE)  	+= freetype bz2
application_LIBS-$(GB_CONFIG_DEVICE_HAVE_SKIA)  	+= skia 
application_LIBS-$(GB_CONFIG_WINDOW_HAVE_SDL)  		+= SDL 
application_LIBS-$(GB_CONFIG_APPLICATION_WINDOW_SDL)+= SDLmain

# path
application_INC_DIR 								+= ../../../../
application_LIB_DIR 								+= ../../../../gbox

# flags
application_CXFLAGS 								+= -D__tb_prefix__=\"demo\"
application_LDFLAGS 								+= -L/usr/local/lib $(GB_CONFIG_DEMO_LDFLAGS)

# command
application_SUFFIX_CMD1 							+= $(GB_CONFIG_DEMO_CMD1)

# suffix
include $(PRO_DIR)/suffix.mak

