-- add x11 package
option("x11")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The x11 package")
    
    -- add defines to config.h if checking ok
    add_defines_h("$(prefix)_PACKAGE_HAVE_X11")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("x11/x11.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")
