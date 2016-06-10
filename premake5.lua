
workspace "SerLib"
    
    configurations { "Debug", "Release", "HeaderOnlyDebug", "HeaderOnlyRelease", "Coverage" }
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
        
    filter "*Debug"
        targetsuffix "d"
        defines "DEBUG"
        optimize "Off"

    filter "*Release"
        flags "LinkTimeOptimization"
        optimize "Speed"
        
    filter "Coverage"
        targetsuffix "cd"
        links "gcov"
        buildoptions "-coverage"
        
    filter "not HeaderOnly*"
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
            
        filter "not HeaderOnly*"
            links "serialisation"
			
		filter "action:gmake"
			links "pthread"
            
        filter { "*Debug", "platforms:x86" }
            defines "PREFIX=X86D_"
        
        filter { "*Debug", "platforms:x86_64" }
            defines "PREFIX=X86_64D_"
        
        filter { "*Release", "platforms:x86" }
            defines "PREFIX=X86R_"
        
        filter { "*Release", "platforms:x86_64" }
            defines "PREFIX=X86_64R_"

        filter {}
            
    project "serialisation"
    
        kind "StaticLib"

        includedirs {
            "serialisation/include/"
            }   
            
        files { 
            "serialisation/include/**.h",
            }
            
        filter "not HeaderOnly*"                
            files { 
                "serialisation/src/**.cpp",
                }