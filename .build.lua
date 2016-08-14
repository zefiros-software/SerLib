
project "SerLib"

    if zpm.option( "HeaderOnly" ) then
        kind "Utility"
    else
        zpm.export [[
            defines "SERIALISATION_NO_HEADER_ONLY"
        ]]  
        files "serialisation/src/**.cpp"   
        kind "StaticLib"
    end

    zpm.export [[
        includedirs "serialisation/include/"
        flags "C++11"
    ]]