#!/bin/bash

. $HOME/Desktop/emsdk/emsdk_env.sh

make clean

make BUILD_TYPE=Release

# run webserver
emrun --port 8080 --no_browser build/index.html 
