# preifx
include $(PRO_DIR)/prefix.mak

# module name
NAMES 														= application

# package name
application_PKG_NAME 										= demo

# type
application_TYPE 											= BIN

# files
application_C_FILES 										+= \
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
															../../tiger \
															../../point \
															../../points \
															../../circle \
															../../ellipse \
															../../triangle \
															../../round_rect  

# packages
application_PKGS-$(TBOX)  									+= tbox 
application_PKGS-$(OPENGL)  								+= opengl 
application_PKGS-$(SKIA)  									+= skia 
application_PKGS-$(GLUT)  									+= glut 
application_PKGS-$(SDL)  									+= sdl 
application_PKGS-$(X11)  									+= x11 
application_PKGS-$(FREETYPE)  								+= freetype 
application_PKGS-$(PNG)  									+= png 
application_PKGS-$(JPEG)  									+= jpeg 
application_PKGS-$(ZLIB)  									+= zlib 
application_PKGS-$(BASE)  									+= base 

# others
application_LIBS 											+= gbox$(DTYPE)
application_INC_DIRS 										+= ../../../../
application_LIB_DIRS 										+= ../../../../gbox
application_CXFLAGS 										+= -D__tb_prefix__=\"demo\"
application_CXFLAGS-$(SDL) 									+= -Dmain=SDL_main

# suffix
include $(PRO_DIR)/suffix.mak

