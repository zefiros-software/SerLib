set -e

premake5 install-package --allow-install --allow-module
premake5 gmake
cd serialisation
make config=debug_x86
make config=debug_x86_64
make config=release_x86
make config=release_x86_64
#make config=coverage_x86
#make config=coverage_x86_64

cd ../test/

premake5 gmake

cd zpm/
make
cd ../test

../bin/x86/serialisation-test
../bin/x86/serialisation-testd
#../bin/x86/serialisation-testcd

../bin/x86_64/serialisation-test
../bin/x86_64/serialisation-testd
#../bin/x86_64/serialisation-testcd

bin/x86/serialisation-zpm-test