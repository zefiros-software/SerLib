
solution "Serialisation"

	location "./serialisation/"
		
	debugdir "./bin/"
	
	objdir "./build/"
	
	targetdir "./bin/"
	
	configurations { "Debug", "Release" }

	platforms "x32"

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"

    configuration "x32"
		architecture "x32"

	configuration "Debug"
		targetsuffix ( "d" )
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"

	configuration "Release"
		flags "LinkTimeOptimization"
		optimize "Speed"

	configuration {}

	project "Serialisation"
		targetname ( "serialisation" )	 
		kind "ConsoleApp"
		flags "WinMain"
		
		defines "_SCL_SECURE_NO_WARNINGS"

		includedirs {
			"serialisation/"
			}	
			
		files { 
			"serialisation/**.cpp", 
			"serialisation/**.cc",
			"serialisation/**.h"
			}
			
		