
-- add option: fixed
option("fixed")
    set_enable(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the fixed type")
    add_defines_h_if_ok("$(prefix)_FLOAT_FIXED")

-- add option: bitmap
option("bitmap")
    set_enable(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the bitmap device")
    add_defines_h_if_ok("$(prefix)_DEVICE_HAVE_BITMAP")

-- add option: smallest
option("smallest")
    set_enable(false)
    set_showmenu(true)
    set_category("option")
    set_description("Enable the smallest compile mode and disable all modules.")
    add_rbindings("bitmap")

-- add target
target("gbox")

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

    -- add is_option
    add_options("bitmap", "fixed")

    -- add packages for window
    if is_os("ios", "android") then 
    elseif is_option("x11") then add_options("x11")
    elseif is_option("glut") then add_options("glut") 
    elseif is_option("sdl") then add_options("sdl")
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
    if is_mode("debug") then add_files("utils/impl/tessellator/profiler.c") end

    -- add the source files for device
    if is_option("opengl") then add_files("core/device/gl.c", "core/device/gl/**.c") end
    if is_option("bitmap") then add_files("core/device/bitmap.c", "core/device/bitmap/**.c") end
    if is_option("skia") then add_files("core/device/skia.cpp") end

    -- add the source files for window
    if is_os("ios") then add_files("platform/ios/window.c") 
    elseif is_os("android") then add_files("platform/android/window.c") 
    elseif is_option("x11") then add_files("platform/x11/window.c") 
    elseif is_option("glut") then add_files("platform/glut/window.c") 
    elseif is_option("sdl") then add_files("platform/sdl/window.c") 
    end




