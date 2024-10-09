#!/bin/bash

mkdir -p "${PWD}/build"

cd "${PWD}/build"

cmake ..

cmake --build .

echo "Successfully built dynamic-bits!"
