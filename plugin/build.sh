#!/bin/bash
# Z-x64dbg-MCP Plugin Build Script (Linux/macOS)

set -e

X64DBG_DIR="${X64DBG_DIR:-/opt/x64dbg}"
BUILD_TYPE="${BUILD_TYPE:-Release}"

echo "========================================"
echo "  Building Z-x64dbg-MCP Plugin"
echo "========================================"
echo ""

# Create build directory
BUILD_DIR="$(dirname "$0")/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo ""
echo "Configuring CMake..."
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -Dx64dbgDir="$X64DBG_DIR"

# Build
echo ""
echo "Building..."
cmake --build . --config "$BUILD_TYPE"

# Install
echo ""
echo "Installing plugins..."
cp "z-x64dbg-mcp.dp64" "$X64DBG_DIR/x64/plugins/" 2>/dev/null || true
cp "z-x64dbg-mcp.dp32" "$X64DBG_DIR/x32/plugins/" 2>/dev/null || true

echo ""
echo "========================================"
echo "  Build Complete!"
echo "========================================"
echo ""
echo "Plugins built in: $BUILD_DIR"
echo ""
