local root      = "../../"
local getcxxflags = premake.tools.gcc.getcxxflags;
function premake.tools.gcc.getcxxflags(cfg)
    local cxxflags = { pthread = "-lpthread" }
    local r = getcxxflags(cfg);
    local r2 = table.translate(cfg.flags, cxxflags);
    for _,v in ipairs(r2) do table.insert(r, v) end
    return r;
end
table.insert(premake.fields.flags.allowed, "pthread");

solution "serialisation"

	location( root .. "serialisation/" )
	debugdir( root .. "bin/" )
	
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
		
		objdir( root .. "bin/obj/" )
		
		targetsuffix "d"
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"
		
		configuration "x64"
			targetdir( root .. "bin/debug/x64/" )
			
		configuration "x32"
			targetdir( root .. "bin/debug/x32/" )

	configuration "Release"
		objdir( root .. "bin/obj/" )
		
		flags "LinkTimeOptimization"
		optimize "Speed"
		
		configuration "x64"
			targetdir( root .. "bin/release/x64/" )
			
		configuration "x32"
			targetdir( root .. "bin/release/x32/" )
			
	configuration "gmake"
		flags "pthread"
	
	configuration {}
			
	project "serialisation-test"
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
			
			
	project "serialisation"
		targetname "serialisation"	 
		kind "StaticLib"

		includedirs {
			root .. "serialisation/include/"
			}	
			
		files { 
			root .. "serialisation/include/**.h",
			root .. "serialisation/src/**.cpp"
			}