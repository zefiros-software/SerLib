set -e

cd serialisation
make config=coverage_x86
cd ../bin/x86/

./serialisation-testcd

cd ../../

codecov --gcov-root bin/obj/x86/Coverage/serialisation-test/
