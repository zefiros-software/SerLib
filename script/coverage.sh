set -e

cd serialisation
make config=coverage_x86
cd ../

coveralls --verbose --root bin/obj/x86/Coverage/ --exclude test --exclude extern --gcov-options '\-lp'