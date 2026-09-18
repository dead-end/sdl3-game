# --------------------------------------------------------------
# Mindestversion und Projektdefinition
# --------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)      # Emscripten liefert mindestens 3.20

project(sdl3game LANGUAGES C)

# C-Standard festlegen
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --------------------------------------------------------------
# Build‑Typ (kann beim Aufruf überschrieben werden)
# --------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)        # Release = optimiert
endif()

# --------------------------------------------------------------
# Quellen des Projekts
# --------------------------------------------------------------
# Passe die Liste an deine eigenen Dateien an
set(SRC_FILES
    src/main.c           # Einstiegspunkt
#    src/renderer.c      # weitere C‑Dateien (falls vorhanden)
    # src/… weitere Dateien …
)

# --------------------------------------------------------------
# Executable anlegen
# --------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SRC_FILES})

# --------------------------------------------------------------
# SDL 3 finden
# --------------------------------------------------------------
# Emscripten liefert ein eigenes FindSDL3.cmake‑Modul.
# Dadurch genügt ein einfacher Aufruf:
find_package(SDL3 REQUIRED)

# Header‑Verzeichnis hinzufügen (z. B. <SDL3/SDL.h>)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${SDL3_INCLUDE_DIRS}
)

# Bibliotheken verlinken
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SDL3_LIBRARIES}
)

# --------------------------------------------------------------
# WebAssembly‑spezifische Optionen
# --------------------------------------------------------------
# 1. Basis‑Linker‑Flags für SDL 3 und Export der main‑Funktion
set(WASM_LINK_FLAGS
    "-sUSE_SDL=3"                # SDL 3 Port von Emscripten einbinden
    "-sEXPORTED_FUNCTIONS=_main" # Exportiere die main‑Funktion
    "-sALLOW_MEMORY_GROWTH=1"   # Heap kann dynamisch wachsen
    "-o" "index.html" 
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

# 2. Optional: WebGL 2‑Unterstützung (für SDL‑Renderer)
list(APPEND WASM_LINK_FLAGS "-sUSE_WEBGL2=1")

# 3. Optimierungen / Debug‑Optionen (kann beim Aufruf überschrieben werden)
#    Release: -O3, Debug: -gsource-map -sASSERTIONS=2
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND WASM_LINK_FLAGS "-O3" "-sMINIFY_HTML=1")
else()
    list(APPEND WASM_LINK_FLAGS "-gsource-map" "-sASSERTIONS=2")
endif()

# 4. Alle Flags dem Target hinzufügen
target_link_options(${PROJECT_NAME} PRIVATE ${WASM_LINK_FLAGS})

# --------------------------------------------------------------
# Ressourcen (z. B. Bilder, Audiodateien) einbinden
# --------------------------------------------------------------
# Emscripten packt alles, was über --preload-file angegeben wird,
# in ein .data‑File.  Hier ein Beispiel für ein Verzeichnis „assets“:

set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets")

if (EXISTS "${ASSET_DIR}")

    # --preload-file assets@/assets  → im Browser unter /assets erreichbar
    target_link_options(${PROJECT_NAME} PRIVATE
        "--preload-file" "${ASSET_DIR}@/assets"
    )

endif()

# --------------------------------------------------------------
# Hinweis für die Nutzung von emcmake / emmake
# --------------------------------------------------------------
# Wenn du das Projekt mit dem Wrapper‑Script `emcmake` konfigurierst,
# brauchst du die Toolchain‑Datei nicht selbst anzugeben:
#
#   source /pfad/zu/emsdk/emsdk_env.sh
#   mkdir build && cd build
#   emcmake cmake ..                # (optional weitere -D‑Optionen)
#   emmake make                     # oder: cmake --build .
#
# Ohne Wrapper musst du die Toolchain‑Datei explizit übergeben:
#
#   cmake -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake ..
#
# Beide Varianten funktionieren identisch – wähle die für dich bequemste.
# --------------------------------------------------------------
# Ziel‑Dateinamen (HTML‑Ausgabe)
# --------------------------------------------------------------
# Standardmäßig erzeugt Emscripten eine .js‑/.wasm‑Datei.
# Mit dem Flag -sEXPORT_NAME kannst du einen HTML‑Wrapper anfordern:
#
#target_link_options(${PROJECT_NAME} PRIVATE "-sEXPORT_NAME=${PROJECT_NAME}")

# --------------------------------------------------------------
# End of CMakeLists.txt
# --------------------------------------------------------------