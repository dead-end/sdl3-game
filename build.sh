#!/bin/sh
set -eu

. ../emsdk/emsdk_env.sh

if [ -d build ]; then
  rm -rf build
fi

mkdir build
cd build

emcmake cmake ..              
emmake make
