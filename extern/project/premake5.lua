local root      = "../../"

solution "Serialisation"

	location( root .. "serialisation/" )
		
	debugdir( root .. "bin/" )
	
	objdir( root .. "bin/obj/" )

	configurations { "Debug", "Release" }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"

    configuration "x32"
		architecture "x32"
		targetdir( root .. "bin/x32/" )

    configuration "x64"
		architecture "x64"		
		targetdir( root .. "bin/x64/" )
		
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
		location(  root .. "test/" )
		
		links "Serialisation"
		
		kind "ConsoleApp"
		flags "WinMain"
		defines "GTEST_HAS_TR1_TUPLE=0"
		
		includedirs {
			root .. "extern/gtest/include/",
			root .. "extern/gtest/",
			
			root .. "serialisation/include/",
			root .. "test/"
			}	
		
		files { 
			root .. "extern/gtest/src/gtest-all.cc",
			root .. "test/**.h",
			root .. "test/*.cpp"
			}
			
			
	project "Serialisation"
		targetname "serialisation"	 
		kind "StaticLib"

		includedirs {
			root .. "serialisation/include/"
			}	
			
		files { 
			root .. "serialisation/include/**.h",
			root .. "serialisation/src/**.cpp"
			}