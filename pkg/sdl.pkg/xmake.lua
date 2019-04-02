-- add sdl package
option("sdl")

    -- show menu
    set_showmenu(true)

    -- set category
    set_category("package")

    -- set description
    set_description("The sdl package")

    -- add defines to config.h if checking ok
    set_configvar("GB_CONFIG_PACKAGE_HAVE_SDL", 1)

    -- add link directories
    add_linkdirs("lib/$(plat)/$(arch)")

    -- add c includes for checking
    add_cincludes("sdl/sdl.h")

    -- add include directories
    add_includedirs("inc/$(plat)", "inc")

    -- add links for checking
    add_links("SDL")
    before_check(function (option)
        if is_plat("macosx") then
            option:add("syslinks", "SDLmain")
            option:add("defines", "main=SDL_main")
            option:add("frameworks", "Foundation", "Cocoa")
        end
    end)


