set -e

cd bsp
make config=coverage_x86
cd ../

coveralls --exclude test --gcov-options '\-lp'