
if "%TYPE%" == "zpm" (    
    cd test
    
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm vs2015 --allow-install || exit /b 1

    msbuild zpm/SerLib-ZPM.sln || exit /b 1

    bin\x86\serialisation-zpm-test.exe || exit /b 1
) else (
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm %VSTUD% --allow-install || exit /b 1

    msbuild serialisation/SerLib.sln /property:Configuration=Release /property:Platform=%PLAT% || exit /b 1

    if %TYPE% == "debug" (
        bin\%ARCH%\serialisation-testd
    ) else (
        bin\%ARCH%\serialisation-test
    )
)