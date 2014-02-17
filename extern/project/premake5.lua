local root      = "../../"

solution "Serialisation"

	location( root .. "serialisation/" )
		
	configurations { "Debug", "Release" }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"

    configuration "x32"
		architecture "x32"

    configuration "x64"
		architecture "x64"
		
	configuration "Debug"
		debugdir( root .. "bin/debug/" )
		objdir( root .. "bin/obj/debug/" )
		targetsuffix ( "d" )
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"
		
		configuration "x64"
			targetdir( root .. "bin/debug/x64/" )
			
		configuration "x32"
			targetdir( root .. "bin/debug/x32/" )

	configuration "Release"
		debugdir( root .. "bin/release/" )
		objdir( root .. "bin/obj/release/" )
		flags "LinkTimeOptimization"
		optimize "Speed"
		
		configuration "x64"
			targetdir( root .. "bin/release/x64/" )
			
		configuration "x32"
			targetdir( root .. "bin/release/x32/" )
			
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