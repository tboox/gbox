-- add skia package
option("skia")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The skia package")
    
    -- add defines to config.h if checking ok
    add_defines_h("$(prefix)_PACKAGE_HAVE_SKIA")

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c++ includes for checking
    add_cxxincludes("skia/skia.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")

    -- add links for checking
    add_links("skia")
    before_check(function (option)
        if is_plat("macosx") then 
            option:add("frameworks", "AGL")
        end
    end)
