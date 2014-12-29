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
												core/vector \
												core/bitmap \
												utils/mesh \
												other/test

# packages
demo_PKGS-$(TBOX)  								+= tbox 
demo_PKGS-$(OPENGL)  							+= opengl 
demo_PKGS-$(SKIA)  								+= skia 
demo_PKGS-$(GLUT)  								+= glut 
demo_PKGS-$(SDL)  								+= sdl 
demo_PKGS-$(X11)  								+= x11 
demo_PKGS-$(FREETYPE)  							+= freetype 
demo_PKGS-$(PNG)  								+= png 
demo_PKGS-$(JPEG)  								+= jpeg 
demo_PKGS-$(ZLIB)  								+= zlib 
demo_PKGS-$(BASE)  								+= base 

# others
demo_LIBS 										+= gbox$(DTYPE)
demo_INC_DIRS 									+= ../../
demo_LIB_DIRS 									+= ../../gbox
demo_CXFLAGS 									+= -D__tb_prefix__=\"demo\"
demo_CXFLAGS-$(SDL) 							+= -Dmain=SDL_main

# suffix
include $(PRO_DIR)/suffix.mak

