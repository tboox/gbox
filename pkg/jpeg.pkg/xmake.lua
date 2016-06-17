-- add jpeg package
option("jpeg")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The jpeg package")
    
    -- add defines to config.h if checking ok
    add_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_JPEG")

    -- add links for checking
    add_links("jpeg")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("jpeg/jpeg.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")
