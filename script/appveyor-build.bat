premake5 install-package --allow-install --allow-module  || exit /b 1
premake5 vs2015 || exit /b 1
msbuild serialisation/SerLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b 1
::msbuild serialisation/SerLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b 1
msbuild serialisation/SerLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b 1
::msbuild serialisation/SerLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b 1

cd test/
premake5 vs2015 || exit /b 1
msbuild zpm/SerLib-ZPM.sln || exit /b 1

cd ..\bin\x86\
.\serialisation-test.exe || exit /b 1
::.\serialisation-testd.exe || exit /b 1

cd ..\x86_64\
.\serialisation-test.exe || exit /b 1
::.\serialisation-testd.exe || exit /b 1

cd ..\..\test\bin\
.\x86\serialisation-zpm-test.exe || exit /b 1