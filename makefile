# ==============================================================================
# Emscripten SDL3 Makefile
# ==============================================================================

CC = emcc
PROJECT_NAME = sdl3game
TARGET = index.html
BUILD_DIR = build

# C-Standard and Include-Pfad for own headers (-Iinclude)
CFLAGS = -std=c11 -Iinclude -Wall -Wextra

# Build-Type (Standard: Release. Can be changed with: make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

# Source files
SRC_FILES = src/main.c \
			src/misc/log.c \
            src/screen/screen_manager.c \
            src/screen/screen_start.c \
			src/screen/screen_game.c \
            src/drawable/background.c \
            src/drawable/stars.c \
            src/game/game_state.c

# ------------------------------------------------------------------------------
# WebAssembly & SDL3 Linker-Flags
# ------------------------------------------------------------------------------
WASM_FLAGS = -sUSE_SDL=3 \
             -sEXPORTED_FUNCTIONS="['_main']" \
             -sALLOW_MEMORY_GROWTH=1 \
             -sALLOW_TABLE_GROWTH=1 \
             -sUSE_WEBGL2=1 #\
#             --shell-file src/shell.html

# Build-Type specific flags (optimizations vs. debugging)
ifeq ($(BUILD_TYPE), Release)
    CFLAGS += -O3
    WASM_FLAGS += -O3 -sMINIFY_HTML=1
else
    CFLAGS += -g -O0 -gsource-map
    WASM_FLAGS += -g -O0 -gsource-map -sASSERTIONS=2 -sSAFE_HEAP=1
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

# Start web server for the file
run:
	emrun --port 8080 --no_browser $(BUILD_DIR)/$(TARGET) 

.PHONY: all clean run
