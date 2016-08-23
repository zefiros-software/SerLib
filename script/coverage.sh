set -e

cd serialisation
make config=coverage_x86
cd ../

coveralls --build-root bin/obj/x86/Coverage/ --exclude test --exclude extern --gcov-options '\-lp'