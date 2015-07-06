-- the base package
add_option("base")
    
    -- set category
    set_option_category("package")
   
    -- add links
    if os("windows") then add_option_links("ws2_32") 
    elseif os("macosx") then 
        add_option_cxflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
        add_option_mxflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
        add_option_ldflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
    elseif os("ios") then 
        add_option_cxflags("-framework Foundation", "-framework UIKit") 
        add_option_mxflags("-framework Foundation", "-framework UIKit") 
        add_option_ldflags("-framework Foundation", "-framework UIKit") 
    else add_option_links("m", "dl", "pthread") end

