#!/bin/bash

. $HOME/Desktop/emsdk/emsdk_env.sh

make clean

make BUILD_TYPE=Release
