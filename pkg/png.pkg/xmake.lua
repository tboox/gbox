-- add png package
option("png")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The png package")
    
    -- add defines to config.h if checking ok
    set_configvar("GB_CONFIG_PACKAGE_HAVE_PNG", 1)

    -- add links for checking
    add_links("png")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("png/png.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")
