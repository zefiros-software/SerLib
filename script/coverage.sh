set -e

cd serialisation
make config=coverage_x86
cd ../

./serialisation-testcd

ls

codecov
