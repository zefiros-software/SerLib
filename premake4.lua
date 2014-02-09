
solution "Serialisation"

	location "serialisation/"
		
	debugdir "bin/"
	
	objdir "build/"

	configurations { "Debug", "Release" }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"

    configuration "x32"
		architecture "x32"
		targetdir "bin/x32/"

    configuration "x64"
		architecture "x64"		
		targetdir "bin/x64/"
		
	configuration "Debug"
		targetsuffix ( "d" )
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"

	configuration "Release"
		flags "LinkTimeOptimization"
		optimize "Speed"

	configuration {}
			
	project "Serialisation Test"
		location ( "test/" )
		
		links "Serialisation"
		
		kind "ConsoleApp"
		flags "WinMain"
		defines "GTEST_HAS_TR1_TUPLE=0"
		
		includedirs {
			"extern/gtest/include/",
			"extern/gtest/",
			
			"serialisation/include/",
			"test/"
			}	
		
		files { 
			"extern/gtest/src/gtest-all.cc",
			"test/**.h",
			"test/*.cpp"
			}
			
			
	project "Serialisation"
		targetname "serialisation"	 
		kind "StaticLib"

		includedirs {
			"serialisation/include/"
			}	
			
		files { 
			"serialisation/include/**.h",
			"serialisation/src/**.cpp"
			}