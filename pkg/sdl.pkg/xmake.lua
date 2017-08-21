-- add sdl package
option("sdl")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The sdl package")

    -- add defines to config.h if checking ok
    add_defines_h("$(prefix)_PACKAGE_HAVE_SDL")

    -- add links for checking
    add_links("SDL")
    if is_plat("macosx") then 
        add_links("SDLmain") 
        add_defines("main=SDL_main")
        add_ldflags("-framework Foundation", "-framework Cocoa") 
    end

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("sdl/sdl.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")


