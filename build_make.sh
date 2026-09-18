#!/bin/bash
set -eu

# 1. Skript-Pfad ermitteln
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 2. Emscripten laden (über den Ordner-Wechsel-Trick)
set +u
START_DIR="$(pwd)"
cd "$SCRIPT_DIR/../emsdk"
. ./emsdk_env.sh
cd "$START_DIR"
set -u

# 3. Altes Build löschen und neu bauen per Makefile
make clean
make BUILD_TYPE=Release
