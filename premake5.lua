local zefiros = require( "Zefiros-Software/Zefiros-Defaults", "@head" )

workspace "SerLib"

	zefiros.setDefaults( "serialisation", {
        configurations = { "HeaderOnlyDebug", "HeaderOnlyRelease" }
    } )

    filter "not HeaderOnly*"
        defines "SERIALISATION_NO_HEADER_ONLY"

    project "serialisation-test"
            
        filter "not HeaderOnly*"
            links "serialisation"
            
    project "serialisation"
            
        filter "not HeaderOnly*"                
            files { 
                "serialisation/src/**.cpp",
                }