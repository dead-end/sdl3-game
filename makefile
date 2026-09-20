# ==============================================================================
# Emscripten SDL3 Makefile
# ==============================================================================

CC = emcc
PROJECT_NAME = sdl3game
TARGET = index.html
BUILD_DIR=build

# C-Standard and Include-Pfad for own headers (-Iinclude)
CFLAGS = -std=c11 -Iinclude
CFLAGS += -Wall -Wextra

# Build-Type (Standard: Release. Can be changed with: make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

# Source files
SRC_FILES = src/main.c \
            src/screen_manager.c \
            src/start_screen.c
            

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
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) $(WASM_FLAGS) -o $(BUILD_DIR)/$(TARGET)
	@echo "Build finished: $(BUILD_DIR)/$(TARGET) ($(BUILD_TYPE)-Modus)"

clean:
	rm -rf $(BUILD_DIR)
	@echo "Build cleanup."

#
# Start web server for the file
#
run:
	emrun --port 8080 --no_browser $(BUILD_DIR)/$(TARGET) 

#
# Targets that do not build files
#
.PHONY: all clean run
