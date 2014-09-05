# demo
GB_CONFIG_DEMO_LIBS							= polarssl sqlite3 z m dl pthread
GB_CONFIG_DEMO_CMD1 						=
GB_CONFIG_DEMO_LDFLAGS 						= -framework GLUT -framework OpenGL -framework AppKit

# third party
GB_CONFIG_THIRD_HAVE_GL 	 				= y
GB_CONFIG_THIRD_HAVE_GLUT 					= y
GB_CONFIG_THIRD_HAVE_SKIA 					= y
GB_CONFIG_THIRD_HAVE_X11 	 				= n
GB_CONFIG_THIRD_HAVE_SDL 					= y
GB_CONFIG_THIRD_HAVE_FRAMEBUFFER 			= n

# app window
GB_CONFIG_APP_WINDOW_NATIVE 				= n
GB_CONFIG_APP_WINDOW_SDL 					= y
GB_CONFIG_APP_WINDOW_FRAMEBUFFER 			= n

