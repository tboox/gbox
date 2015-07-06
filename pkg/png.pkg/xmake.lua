-- add png package
add_option("png")

    -- show menu
    set_option_showmenu(true)

    -- set category
    set_option_category("package")

    -- set description
    set_option_description("The png package")
    
    -- add defines to config.h if checking ok
    add_option_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_PNG")

    -- add links for checking
    add_option_links("png")

    -- add link directories
    add_option_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_option_cincludes("png/png.h")

    -- add include directories
    add_option_includedirs("inc/$(plat)", "inc")
