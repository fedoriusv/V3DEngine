#!/bin/bash
PROJECT_DIR=project
XCODE_DIR=xcode

echo "Run poject_clang_xcode"

if [  ! -d ${PROJECT_DIR} ] ; then
    mkdir ${PROJECT_DIR}
fi
cd ./${PROJECT_DIR}

if [  ! -d ${XCODE_DIR} ] ; then
    mkdir ${XCODE_DIR}
fi
cd ./${XCODE_DIR}

cmake -G Xcode -DCOMPILER_CLANG=ON -DCMAKE_OSX_ARCHITECTURES=x86_64  ../.. $@   || exit $?
cd ../..

