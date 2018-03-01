
project "SerLib"
    kind "StaticLib"


    files "serialisation/src/**.cpp"   

    
    zpm.export [[
        includedirs "serialisation/include/"
        cppdialect "C++11"

        defines "SERIALISATION_NO_HEADER_ONLY"
    ]]  