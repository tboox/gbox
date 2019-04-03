
-- add target
target("core")

    -- add the dependent target
    add_deps("gbox")

    -- make as a binary
    set_kind("binary")

    -- add defines
    add_defines("__tb_prefix__=\"demo\"")

    -- add packages
    add_packages("tbox", "glut", "opengl")

    -- add the source files
    add_files("*.c") 

