-- add opengl package
add_option("opengl")

    -- show menu
    set_option_showmenu(true)

    -- set category
    set_option_category("package")

    -- set description
    set_option_description("The opengl package")

    -- add defines to config.h if checking ok
    add_option_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_OPENGL")
    if plats("windows", "mingw") then
        add_option_defines_h_if_ok("$(prefix)_GL_APICALL=__tb_stdcall__")
    else
        add_option_defines_h_if_ok("$(prefix)_GL_APICALL=__tb_cdecl__")
    end

    -- add links for checking
    if plats("windows", "mingw") then
        add_option_links("opengl32")
    elseif plats("macosx") then
        add_option_cxflags("-framework OpenGL")
        add_option_mxflags("-framework OpenGL")
        add_option_ldflags("-framework OpenGL")
    else
        add_option_links("GL")
    end

    -- add link directories
    add_option_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_option_cincludes("opengl/opengl.h")

    -- add include directories
    add_option_includedirs("inc/$(plat)", "inc")


