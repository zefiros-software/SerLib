set -e

cd serialisation
make config=coverage_x86
cd ../bin/x86/

./serialisation-testcd

cd ../../

coveralls --exclude test --exclude extern -i extern -i test --build-root serialisation --gcov-options '\-lp'