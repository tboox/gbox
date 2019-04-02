-- the base package
option("base")
    
    -- set category
    set_category("package")
   
    -- add links
    before_check(function (option)
        if is_plat("windows") then 
            option:add("links", "ws2_32") 
        elseif is_plat("macosx") then 
            option:add("frameworks", "Foundation", "Cocoa", "AppKit")
        elseif is_plat("iphoneos") then 
            option:add("frameworks", "Foundation", "UIKit")
        else 
            option:add("links", "m", "dl", "pthread") 
        end
    end)

