set -e

cd serialisation
make config=coverage_x86
cd ../

coveralls --exclude test --gcov-options '\-lp'