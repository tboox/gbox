-- add opengl package
option("opengl")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The opengl package")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("opengl/opengl.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")

    -- add links for checking
    before_check(function (option)
        if is_plat("windows", "mingw") then
            option:add("syslinks", "opengl32")
        elseif is_plat("macosx") then
            option:add("frameworks", "OpenGL")
        else
            option:add("syslinks", "GL")
        end

        -- add defines to config.h if checking ok
        option:set("configvar", "GB_CONFIG_PACKAGE_HAVE_OPENGL", 1)
        if is_plat("windows", "mingw") then
            option:set("configvar", "GB_CONFIG_GL_APICALL", "__tb_stdcall__")
        else
            option:set("configvar", "GB_CONFIG_GL_APICALL", "__tb_cdecl__")
        end
    end)
