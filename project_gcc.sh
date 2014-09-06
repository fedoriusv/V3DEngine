#!/bin/bash
PROJECT_DIR=project
CODE_DIR=linux

echo "Run poject_gcc"

if [  ! -d ${PROJECT_DIR} ] ; then
    mkdir ${PROJECT_DIR}
fi
cd ./${PROJECT_DIR}

if [  ! -d ${CODE_DIR} ] ; then
    mkdir ${CODE_DIR}
fi
cd ./${CODE_DIR}

cmake -DCOMPILER_GCC=ON  ../.. $@   || exit $?
#use -DCMAKE_BUILD_TYPE=Debug if need used gdb
make

cd ../..

