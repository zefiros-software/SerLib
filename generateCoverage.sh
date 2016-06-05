rm -rf gcov
rm -rf target/site/clover
mkdir gcov
cd cgov
gcov --object-directory bin/obj/x86/Coverage/serialisation-test/* test
cd ../
lcov --capture --base-directory test --directory ./ --directory test/ --directory serialisation/include/ --output-file gcov/coverage.info --no-external
genhtml gcov/coverage.info --output-directory target/site/clover
python assets/Zefiros-Software/SerLib/Zefiros-Software/gcov2clover/gcov2clover.py gcov/*.gcov