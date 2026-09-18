# ==============================================================================
# Emscripten SDL3 Makefile
# ==============================================================================

# Compiler & Projektname
CC = emcc
PROJECT_NAME = sdl3game
TARGET = index.html

# C-Standard und Include-Pfad für eigene Header hinzugefügt (-Iinclude)
CFLAGS = -std=c11 -Iinclude

# Build-Typ (Standard: Release. Kann geändert werden mit: make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

# Quell-Dateien (Hier einfach neue .c-Dateien mit Leerzeichen anhängen)
# Hinweis: .h-Dateien werden hier NICHT aufgelistet, da sie über #include geladen werden.
SRC_FILES = src/main.c \
            src/game.c

# ------------------------------------------------------------------------------
# WebAssembly & SDL3 Linker-Flags
# ------------------------------------------------------------------------------
WASM_FLAGS = -sUSE_SDL=3 \
             -sEXPORTED_FUNCTIONS=_main \
             -sALLOW_MEMORY_GROWTH=1 \
             -sUSE_WEBGL2=1

# Build-Typ spezifische Flags (Optimierung vs. Debugging)
ifeq ($(BUILD_TYPE), Release)
    CFLAGS += -O3
    WASM_FLAGS += -O3 -sMINIFY_HTML=1
else
    CFLAGS += -g
    WASM_FLAGS += -gsource-map -sASSERTIONS=2
endif

# ------------------------------------------------------------------------------
# Ressourcen (Assets) einbinden
# ------------------------------------------------------------------------------
ASSET_DIR = assets
ifneq ($(wildcard $(ASSET_DIR)/.),)
    WASM_FLAGS += --preload-file $(ASSET_DIR)@/assets
endif

# ==============================================================================
# Build-Regeln
# ==============================================================================

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC_FILES) $(WASM_FLAGS) -o build/$(TARGET)
	@echo "Build abgeschlossen: build/$(TARGET) ($(BUILD_TYPE)-Modus)"

clean:
	rm -rf build
	@echo "Build-Ordner bereinigt."

.PHONY: all clean
