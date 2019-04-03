
-- add option: fixed
option("fixed")
    set_default(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the fixed type")
    set_configvar("GB_CONFIG_FLOAT_FIXED", 1)

-- add option: bitmap
option("bitmap")
    set_default(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the bitmap device")
    add_deps("small")
    set_configvar("GB_CONFIG_DEVICE_HAVE_BITMAP", 1)
    after_check(function (option)
            if option:dep("small"):enabled() then
                option:enable(false)
            end
        end)

-- add option: small
option("small")
    set_default(false)
    set_showmenu(true)
    set_category("option")
    set_description("Enable the smallest compile mode and disable all modules.")

-- define options for package
for _, name in ipairs({"libjpeg", "libpng", "zlib"}) do
    option(name)
        add_deps("small")
        set_default(true)
        set_showmenu(true)
        set_description("Enable the " .. name .. " package.")
        before_check(function (option)
            if option:dep("small"):enabled() then
                option:enable(false)
            end
        end)
    option_end()
end

-- add requires
for idx, require_name in ipairs({"libjpeg", "libpng", "zlib"}) do
    local name = require_name:split('%s')[1]
    if has_config(name) then
        add_requires(require_name, {optional = true, on_load = function (package)
            package:set("configvar", {["GB_CONFIG_PACKAGE_HAVE_" .. name:upper()] = 1})
        end})
    end
end

-- add target
target("gbox")

    -- make as a static library
    set_kind("static")

    -- add defines
    add_defines("__tb_prefix__=\"gbox\"")

    -- set the auto-generated config.h
    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("gbox.config.h.in")

    -- add include directories
    add_includedirs("..", {public = true})
    add_includedirs("$(buildir)/$(plat)/$(arch)/$(mode)", {public = true})

    -- add the header files for installing
    add_headerfiles("../(gbox/**.h)|**/impl/**.h")
    add_headerfiles("$(buildir)/$(plat)/$(arch)/$(mode)/gbox.config.h", {prefixdir = "gbox"})

    -- add has_config
    add_options("bitmap", "fixed")

    -- add packages for window
    if is_plat("iphoneos", "android") then 
    elseif has_config("x11") then add_options("x11")
    elseif has_config("glut") then add_options("glut") 
    elseif has_config("sdl") then add_options("sdl")
    end

    -- add packages
    add_options("tbox", "opengl", "skia", "png", "jpeg", "freetype", "zlib")

    -- add the common source files
    add_files("*.c")
    add_files("core/**.c|device/**.c")
    add_files("platform/*.c")
    add_files("platform/impl/*.c")
    add_files("utils/**.c|impl/tessellator/profiler.c")

    -- add the source files for debug
    if is_mode("debug") then add_files("utils/impl/tessellator/profiler.c") end

    -- add the source files for device
    if has_config("opengl") then add_files("core/device/gl.c", "core/device/gl/**.c") end
    if has_config("bitmap") then add_files("core/device/bitmap.c", "core/device/bitmap/**.c") end
    if has_config("skia") then add_files("core/device/skia.cpp") end

    -- add the source files for window
    if is_plat("iphoneos") then add_files("platform/ios/window.c") 
    elseif is_os("android") then add_files("platform/android/window.c") 
    elseif has_config("x11") then add_files("platform/x11/window.c") 
    elseif has_config("glut") then add_files("platform/glut/window.c") 
    elseif has_config("sdl") then add_files("platform/sdl/window.c") 
    end




