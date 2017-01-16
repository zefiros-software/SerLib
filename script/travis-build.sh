#!/bin/bash
set -e

if [ "$TYPE" == "zpm" ]; then
    cd test
    
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install

    cd zpm/
    make
    cd ../../

    test/bin/x86/serialisation-zpm-test

else
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install
    cd serialisation
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        bin/${ARCH}/serialisation-testd

    elif [ "$TYPE" == "coverage" ]; then
        ./serialisation-testcd
    else
        bin/${ARCH}/serialisation-test
    fi
fi