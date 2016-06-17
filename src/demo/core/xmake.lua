
-- add target
target("core")

    -- add the dependent target
    add_deps("gbox")

    -- make as a binary
    set_kind("binary")

    -- add defines
    add_defines("__tb_prefix__=\"demo\"")

    -- set the object files directory
    set_objectdir("$(buildir)/.objs")

    -- add links directory
    add_linkdirs("$(buildir)")

    -- add includes directory
    add_includedirs("$(buildir)")
    add_includedirs("$(buildir)/gbox")

    -- add links
    add_links("gbox")

    -- add packages for window
    if is_os("ios", "android") then 
    elseif is_option("x11") then add_options("x11")
    elseif is_option("glut") then add_options("glut") 
    elseif is_option("sdl") then add_options("sdl")
    end

    -- add packages
    add_options("tbox", "opengl", "skia", "png", "jpeg", "freetype", "zlib", "base")

    -- add the source files
    add_files("*.c") 

