-- add skia package
option("skia")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The skia package")
    
    -- add defines to config.h if checking ok
    add_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_SKIA")

    -- add links for checking
    add_links("skia")
    if is_os("macosx") then 
        add_cxflags("-framework AGL") 
        add_mxflags("-framework AGL") 
        add_ldflags("-framework AGL") 
    end

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c++ includes for checking
    add_cxxincludes("skia/skia.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")
