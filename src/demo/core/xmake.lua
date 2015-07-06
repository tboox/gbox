
-- add target
add_target("core")

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

    -- add packages
    add_options("tbox", "opengl", "glut", "skia", "sdl", "x11", "png", "jpeg", "freetype", "zlib", "base")

    -- add the source files
    add_files("*.c") 

