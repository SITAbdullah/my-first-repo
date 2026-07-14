#!/usr/bin/env sh
# =============================================================================
# build_web.sh – Build GLPIFrameworkIntro-Modern for WebGL using Emscripten
#
# Prerequisites:
#   1. Install the Emscripten SDK:  https://emscripten.org/docs/getting_started/
#   2. Activate it:   source /path/to/emsdk/emsdk_env.sh
#   3. Run from the project root:  ./build_web.sh
#
# Output: web/index.html  (+ index.js, index.wasm, index.data)
# Serve:  cd web && python3 -m http.server 8080
#
# Windowing backend (choose one):
#   GLFW  – default, recommended  (-DUSE_GLFW  -s USE_GLFW=3)
#   SDL2  – legacy fallback        (no -DUSE_GLFW, -s USE_SDL=2)
# =============================================================================

set -e

mkdir -p web

# --- GLM header-only dependency ---------------------------------------------
GLM_DIR="build_desktop/_deps/glm-src"
if [ ! -f "$GLM_DIR/glm/glm.hpp" ]; then
    GLM_DIR="web/.deps/glm"
    if [ ! -f "$GLM_DIR/glm/glm.hpp" ]; then
        echo "Fetching GLM 1.0.1..."
        git clone --depth 1 --branch 1.0.1 https://github.com/g-truc/glm.git "$GLM_DIR"
    fi
fi

# --- GLFW backend (default) -------------------------------------------------
emcc -std=c++17 \
    Scene/main.cpp \
    Scene/Renderer.cpp \
    Scene/Transform.cpp \
    Scene/Fan.cpp \
    -IScene \
    -I"$GLM_DIR" \
    -DUSE_GLFW \
    -s USE_GLFW=3 \
    -s USE_WEBGL2=1 \
    -s FULL_ES3=1 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file android/app/src/main/assets/shader@assets/shader \
    -o web/index.html

# --- SDL2 backend (opt-in) --------------------------------------------------
# To use SDL2 instead, comment out the GLFW block above and uncomment below:
# emcc -std=c++17 \
#     Scene/main.cpp \
#     Scene/Renderer.cpp \
#     Scene/Transform.cpp \
#     Scene/Fan.cpp \
#     -IScene \
#     -I"$GLM_DIR" \
#     -s USE_SDL=2 \
#     -s USE_WEBGL2=1 -s FULL_ES3=1 \
#     -s WASM=1 \
#     -s ALLOW_MEMORY_GROWTH=1 \
#     --preload-file android/app/src/main/assets/shader@assets/shader \
#     -o web/index.html

echo ""
echo "Build succeeded."
echo "Run:  cd web && python3 -m http.server 8080"
echo "Open: http://localhost:8080/"
