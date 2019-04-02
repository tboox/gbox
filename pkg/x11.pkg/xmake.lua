-- add x11 package
option("x11")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The x11 package")
    
    -- add defines to config.h if checking ok
    set_configvar("GB_CONFIG_PACKAGE_HAVE_X11", 1)

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("x11/x11.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")
