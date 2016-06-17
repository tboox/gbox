-- the base package
option("base")
    
    -- set category
    set_category("package")
   
    -- add links
    if is_os("windows") then add_links("ws2_32") 
    elseif is_os("macosx") then 
        add_cxflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
        add_mxflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
        add_ldflags("-framework Foundation", "-framework Cocoa", "-framework AppKit") 
    elseif is_os("ios") then 
        add_cxflags("-framework Foundation", "-framework UIKit") 
        add_mxflags("-framework Foundation", "-framework UIKit") 
        add_ldflags("-framework Foundation", "-framework UIKit") 
    else add_links("m", "dl", "pthread") end

