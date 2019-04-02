-- add glut package
option("glut")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The glut package")

    -- add defines to config.h if checking ok
    add_defines_h("$(prefix)_PACKAGE_HAVE_GLUT")

    -- set language: c99, c++11
    set_languages("c99", "cxx11")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("glut/glut.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")

    -- add links for checking
    before_check(function (option)
        if is_plat("windows", "mingw") then
            option:add("links", "glut32", "glu32")
        elseif is_plat("macosx") then
            option:add("frameworks", "GLUT")
        else
            option:add("links", "glut")
        end
    end)

