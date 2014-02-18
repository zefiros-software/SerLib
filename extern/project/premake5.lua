local root      = "../../"
local getldflags = premake.tools.gcc.getldflags;
function premake.tools.gcc.getldflags(cfg)
    local ldflags = { pthread = "-pthread" }
    local r = getldflags(cfg);
    local r2 = table.translate(cfg.flags, ldflags);
    for _,v in ipairs(r2) do table.insert(r, v) end
    return r;
end
table.insert(premake.fields.flags.allowed, "pthread");

solution "serialisation"

	location( root .. "serialisation/" )
	objdir( root .. "bin/obj/" )
	debugdir( root .. "bin/" )
	
	configurations { "Debug", "Release" }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"	

    configuration "x32"
		targetdir( root .. "bin/x32/" )
		architecture "x32"

    configuration "x64"
		targetdir( root .. "bin/x64/" )
		architecture "x64"
		
	configuration "Debug"
		targetsuffix "d"
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"

	configuration "Release"		
		flags "LinkTimeOptimization"
		optimize "Speed"
				
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
			
		configuration "gmake"
			flags "pthread"
			
		configuration { "Debug", "x32" }
			defines "PREFIX=X32D_"
		
		configuration { "Debug", "x64" }
			defines "PREFIX=X64D_"
		
		configuration { "Release", "x32" }
			defines "PREFIX=X32R_"
		
		configuration { "Release", "x64" }
			defines "PREFIX=X64R_"
			
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