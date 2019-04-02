
-- add target
target("console")

    -- add the dependent target
    add_deps("gbox")

    -- make as a binary
    set_kind("binary")

    -- add defines
    add_defines("__tb_prefix__=\"demo\"")

    -- add packages for window
    if is_os("ios", "android") then 
    elseif has_config("x11") then add_options("x11")
    elseif has_config("glut") then add_options("glut") 
    elseif has_config("sdl") then add_options("sdl")
    end

    -- add packages
    add_options("tbox", "opengl", "skia", "png", "jpeg", "freetype", "zlib")

    -- add the source files
    add_files("**.c") 

