
-- add option: fixed
add_option("fixed")
    set_option_enable(true)
    set_option_showmenu(true)
    set_option_category("option")
    set_option_description("Enable or disable the fixed type")
    add_option_defines_h_if_ok("$(prefix)_FLOAT_FIXED")

-- add option: bitmap
add_option("bitmap")
    set_option_enable(true)
    set_option_showmenu(true)
    set_option_category("option")
    set_option_description("Enable or disable the bitmap device")
    add_option_defines_h_if_ok("$(prefix)_DEVICE_HAVE_BITMAP")

-- add target
add_target("gbox")

    -- make as a static library
    set_kind("static")

    -- add defines
    add_defines("__tb_prefix__=\"gbox\"")

    -- set the auto-generated config.h
    set_config_h("$(buildir)/gbox/gbox.config.h")
    set_config_h_prefix("GB_CONFIG")

    -- set the object files directory
    set_objectdir("$(buildir)/.objs")

    -- add includes directory
    add_includedirs("$(buildir)")
    add_includedirs("$(buildir)/gbox")

    -- add the header files for installing
    add_headers("../(gbox/**.h)|**/impl/**.h")

    -- add options
    add_options("bitmap", "fixed")

    -- add packages for window
    if os("ios", "android") then 
    elseif options("x11") then add_options("x11")
    elseif options("glut") then add_options("glut") 
    elseif options("sdl") then add_options("sdl")
    end

    -- add packages
    add_options("tbox", "opengl", "skia", "png", "jpeg", "freetype", "zlib", "base")

    -- add the common source files
    add_files("*.c")
    add_files("core/**.c|device/**.c")
    add_files("platform/*.c")
    add_files("platform/impl/*.c")
    add_files("utils/**.c|impl/tessellator/profiler.c")

    -- add the source files for debug
    if modes("debug") then add_files("utils/impl/tessellator/profiler.c") end

    -- add the source files for device
    if options("opengl") then add_files("core/device/gl.c", "core/device/gl/**.c") end
    if options("bitmap") then add_files("core/device/bitmap.c", "core/device/bitmap/**.c") end
    if options("skia") then add_files("core/device/skia.cpp") end

    -- add the source files for window
    if os("ios") then add_files("platform/ios/window.c") 
    elseif os("android") then add_files("platform/android/window.c") 
    elseif options("x11") then add_files("platform/x11/window.c") 
    elseif options("glut") then add_files("platform/glut/window.c") 
    elseif options("sdl") then add_files("platform/sdl/window.c") 
    end




