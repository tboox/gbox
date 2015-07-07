-- add glut package
add_option("glut")

    -- show menu
    set_option_showmenu(true)

    -- set category
    set_option_category("package")

    -- set description
    set_option_description("The glut package")

    -- add defines to config.h if checking ok
    add_option_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_GLUT")

    -- set language: c99, c++11
    set_option_languages("c99", "cxx11")

    -- add links for checking
    if plats("windows", "mingw") then
        add_option_links("glut32", "glu32")
    elseif plats("macosx") then
        add_option_cxflags("-framework GLUT")
        add_option_mxflags("-framework GLUT")
        add_option_ldflags("-framework GLUT")
    else
        add_option_links("glut")
    end

    -- add link directories
    add_option_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_option_cincludes("glut/glut.h")

    -- add include directories
    add_option_includedirs("inc/$(plat)", "inc")


