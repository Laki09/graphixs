#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_DIR="$PROJECT_DIR/lib"

GLFW_DIR="$LIB_DIR/glfw"
CGLM_DIR="$LIB_DIR/cglm"

EXECUTABLE="$PROJECT_DIR/graphixs"
VERT_SHADER="$PROJECT_DIR/data/shaders/vertShader.vs"
FRAG_SHADER="$PROJECT_DIR/data/shaders/fragShader.fs"

echo "Project directory: $PROJECT_DIR"

require_command() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Error: required command '$1' is not installed."
        exit 1
    fi
}

clone_if_missing() {
    local repo_url="$1"
    local target_dir="$2"

    if [ ! -d "$target_dir" ]; then
        echo "Cloning $(basename "$target_dir")..."
        git clone --depth 1 "$repo_url" "$target_dir"
    else
        echo "$(basename "$target_dir") already exists."
    fi
}

build_glfw_if_needed() {
    if [ ! -f "$GLFW_DIR/src/libglfw3.a" ] && [ ! -f "$GLFW_DIR/build/src/libglfw3.a" ]; then
        echo "Building GLFW..."
        mkdir -p "$GLFW_DIR/build"
        cd "$GLFW_DIR/build"
        cmake ..
        cmake --build .
        cd "$PROJECT_DIR"
    else
        echo "GLFW already built."
    fi
}

setup_dependencies() {
    require_command git
    require_command make

    if [ ! -d "$LIB_DIR" ]; then
        mkdir -p "$LIB_DIR"
    fi

    clone_if_missing "https://github.com/glfw/glfw.git" "$GLFW_DIR"
    clone_if_missing "https://github.com/recp/cglm.git" "$CGLM_DIR"

    require_command cmake
    build_glfw_if_needed
}

run_project() {
    if [ ! -f "$VERT_SHADER" ] || [ ! -f "$FRAG_SHADER" ]; then
        echo "Error: shader files not found."
        exit 1
    fi

    echo "Building project..."
    make

    echo "Running engine..."
    "$EXECUTABLE" "$VERT_SHADER" "$FRAG_SHADER"
}

setup_dependencies
run_project
