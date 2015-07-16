-- add sdl package
add_option("sdl")

    -- show menu
    set_option_showmenu(true)

    -- set category
    set_option_category("package")

    -- set description
    set_option_description("The sdl package")

    -- add defines to config.h if checking ok
    add_option_defines_h_if_ok("$(prefix)_PACKAGE_HAVE_SDL")

    -- add links for checking
    add_option_links("SDL")
    if plats("macosx") then 
        add_option_links("SDLmain") 
        add_option_defines("main=SDL_main")
        add_option_ldflags("-framework Foundation", "-framework Cocoa") 
    end

    -- add link directories
    add_option_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_option_cincludes("sdl/sdl.h")

    -- add include directories
    add_option_includedirs("inc/$(plat)", "inc")


