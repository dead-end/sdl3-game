# ==============================================================================
# Emscripten SDL3 Makefile
# ==============================================================================

# Compiler & Projectname
CC = emcc
PROJECT_NAME = sdl3game
TARGET = index.html

# C-Standard and Include-Pfad for own headers (-Iinclude)
CFLAGS = -std=c11 -Iinclude

# Build-Type (Standard: Release. Can be changed with: make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

# Source files
SRC_FILES = src/main.c \
            src/game.c

# ------------------------------------------------------------------------------
# WebAssembly & SDL3 Linker-Flags
# ------------------------------------------------------------------------------
WASM_FLAGS = -sUSE_SDL=3 \
             -sEXPORTED_FUNCTIONS=_main \
             -sALLOW_MEMORY_GROWTH=1 \
             -sUSE_WEBGL2=1

# Build-Type specific flags (optimizations vs. debugging)
ifeq ($(BUILD_TYPE), Release)
    CFLAGS += -O3
    WASM_FLAGS += -O3 -sMINIFY_HTML=1
else
    CFLAGS += -g
    WASM_FLAGS += -gsource-map -sASSERTIONS=2
endif

# ------------------------------------------------------------------------------
# Include resources (Assets)
# ------------------------------------------------------------------------------
ASSET_DIR = assets
ifneq ($(wildcard $(ASSET_DIR)/.),)
    WASM_FLAGS += --preload-file $(ASSET_DIR)@/assets
endif

# ==============================================================================
# Build-rules
# ==============================================================================

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC_FILES) $(WASM_FLAGS) -o build/$(TARGET)
	@echo "Build finished: build/$(TARGET) ($(BUILD_TYPE)-Modus)"

clean:
	rm -rf build
	@echo "Build cleanup."

.PHONY: all clean
