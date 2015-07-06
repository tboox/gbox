-- add skia package
add_option("skia")

    -- show menu
    set_option_showmenu(true)

    -- set category
    set_option_category("package")

    -- set description
    set_option_description("The skia package")
    
    -- add defines to config.h if checking ok
    add_option_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_SKIA")

    -- add links for checking
    add_option_links("skia")
    if os("macosx") then 
        add_option_cxflags("-framework AGL") 
        add_option_mxflags("-framework AGL") 
        add_option_ldflags("-framework AGL") 
    end

    -- add link directories
    add_option_linkdirs("lib/$(plat)/$(arch)")

    -- add c++ includes for checking
    add_option_cxxincludes("skia/skia.h")

    -- add include directories
    add_option_includedirs("inc/$(plat)", "inc")
