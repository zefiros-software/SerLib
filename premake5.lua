
workspace "SerLib"
    
    configurations { "Debug", "Release", "NoHeaderOnly", "Coverage" }
    platforms { "x86_64", "x86" }
    flags { "Unicode", "Symbols" } 

    startproject "serialisation-test"
    location "serialisation/"
    objdir "bin/obj/"

    vectorextensions "SSE2"
    warnings "Extra"
    
    filter "platforms:x86"
        targetdir "bin/x86/"
        debugdir "bin/x86/"
        architecture "x86"
    
    filter "platforms:x86_64"
        targetdir "bin/x86_64/"
        debugdir "bin/x86_64/"
        architecture "x86_64"
        
    filter "configurations:Debug"
        targetsuffix "d"
        defines "DEBUG"
        optimize "Off"

    filter "configurations:Release or configurations:NoHeaderOnly"
        flags "LinkTimeOptimization"
        optimize "Speed"
        
    filter "configurations:Coverage"
        targetsuffix "cd"
        links "gcov"
        buildoptions "-coverage"
        
    filter "configurations:NoHeaderOnly"
        defines "SERIALISATION_NO_HEADER_ONLY"
        
    zpm.buildLibraries()
                
    filter {}
            
    project "serialisation-test"
                    
        kind "ConsoleApp"
        flags "WinMain"
        
        location "test/"    
        
        zpm.uses "Zefiros-Software/GoogleTest"
        
        includedirs {
            "serialisation/include/",
            "test/"
            }   
        
        files { 
            "test/**.h",
            "test/**.cpp"
            }     
            
        filter "configurations:NoHeaderOnly"
            links "serialisation"
			
		filter "action:gmake"
			links "pthread"
            
        filter { "configurations:Debug", "platforms:x86" }
            defines "PREFIX=X86D_"
        
        filter { "configurations:Debug", "platforms:x86_64" }
            defines "PREFIX=X86_64D_"
        
        filter { "configurations:Release", "platforms:x86" }
            defines "PREFIX=X86R_"
        
        filter { "configurations:Release", "platforms:x86_64" }
            defines "PREFIX=X86_64R_"
            
    project "serialisation"
    
        kind "StaticLib"

        includedirs {
            "serialisation/include/"
            }   
            
        files { 
            "serialisation/include/**.h",
            }
            
        filter "configurations:NoHeaderOnly"                
            files { 
                "serialisation/src/**.cpp",
                }