-- add opengl package
option("opengl")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The opengl package")

    -- add defines to config.h if checking ok
    add_defines_h("$(prefix)_PACKAGE_HAVE_OPENGL")
    if is_plat("windows", "mingw") then
        add_defines_h("$(prefix)_GL_APICALL=__tb_stdcall__")
    else
        add_defines_h("$(prefix)_GL_APICALL=__tb_cdecl__")
    end

    -- add links for checking
    if is_plat("windows", "mingw") then
        add_links("opengl32")
    elseif is_plat("macosx") then
        add_cxflags("-framework OpenGL")
        add_mxflags("-framework OpenGL")
        add_ldflags("-framework OpenGL")
    else
        add_links("GL")
    end

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("opengl/opengl.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")


