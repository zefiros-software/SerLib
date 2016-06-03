rm -rf gcov
rm -rf target/site/clover
mkdir gcov
cd gcov/
gcov --object-directory ../bin/obj/x86/Coverage/serialisation-test/* ../test
lcov --capture --base-directory ../test --directory .. --directory ../test/ --directory ../serialisation/include/ --output-file coverage.info --no-external
genhtml coverage.info --output-directory ../target/site/clover
python  ../assets/Zefiros-Software/SerLib-Dev/Zefiros-Software/gcov2clover/gcov_to_clover.py *.gcov