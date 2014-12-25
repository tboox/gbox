# preifx
include $(PRO_DIR)/prefix.mak

# module name
NAMES 										= demo

# type
demo_TYPE 									= BIN

# files
demo_C_FILES 								+=

# libs
demo_LIBS 									+= gbox$(DTYPE) tbox$(DTYPE) 
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_OPENSSL) 	+= ssl crypto
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_POLARSSL) 	+= polarssl
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_MYSQL) 	+= mysqlclient
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_SQLITE3) 	+= sqlite3
demo_LIBS-$(GB_CONFIG_THIRD_HAVE_ZLIB) 		+= z
demo_LIBS-y 								+= $(GB_CONFIG_DEMO_LIBS)

# path
demo_LIB_DIRS 								+= ../../gbox

# flags
demo_CXFLAGS 								+= -D__tb_prefix__=\"demo\"

# command
demo_SUFFIX_CMD1 							+= $(GB_CONFIG_DEMO_CMD1)

# suffix
include $(PRO_DIR)/suffix.mak

