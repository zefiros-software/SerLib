premake5 install-package --allow-install --allow-module  || exit /b
premake5 vs2015 || exit /b
msbuild bsp/SerLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b
::msbuild bsp/SerLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b
msbuild bsp/SerLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b
::msbuild bsp/SerLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b

cd test/
premake5 vs2015 || exit /b
msbuild zpm/SerLib-ZPM.sln || exit /b

../bin\x86\serialisation-test.exe || exit /b
::../bin\x86\serialisation-testd.exe || exit /b

../bin\x86_64\serialisation-test.exe || exit /b
::../bin\x86_64\serialisation-testd.exe || exit /b

bin\x86\serialisation-zpm-test.exe || exit /b