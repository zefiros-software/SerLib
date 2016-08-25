set -e

cd serialisation
make config=coverage_x86
cd ../

./serialisation-testcd

codecov --gcov-root serialisation
