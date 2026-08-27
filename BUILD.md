# Build Instructions

Complete guide for building Z-x64dbg-MCP plugin and server.

## Prerequisites

### Windows (x64dbg Plugin)
- **Visual Studio 2022** with C++ desktop workload
- **CMake 3.20+**
- **x64dbg** installed (C:\x64dbg or custom path)
- **Git**

### Server (TypeScript)
- **Node.js 20+**
- **npm** or **pnpm**

### Native (Zig - Optional)
- **Zig 0.11+** (for native build)

---

## Quick Build (Windows)

### 1. Build Plugin

```powershell
cd plugin
.\build.ps1 -x64dbgDir "C:\x64dbg"
```

**Output:**
- `build/z-x64dbg-mcp.dp64` (x64dbg plugin)
- `build/z-x64dbg-mcp.dp32` (x32dbg plugin)

### 2. Build Server

```powershell
cd server
npm install
npm run build
```

**Output:**
- `dist/index.js` (MCP server)

---

## Manual Build

### Plugin (CMake)

```bash
cd plugin

# Create build directory
mkdir build
cd build

# Configure (Windows x64)
cmake .. -G "Visual Studio 17 2022" -A x64 -Dx64dbgDir="C:/x64dbg"

# Build
cmake --build . --config Release

# Output files:
# - z-x64dbg-mcp.dp64 (x64dbg)
# - z-x64dbg-mcp.dp32 (x32dbg)
```

### Server (TypeScript)

```bash
cd server

# Install dependencies
npm install

# Build TypeScript
npm run build

# Output: dist/index.js
```

### Native (Zig - Optional)

```bash
# Build release binary
zig build -Doptimize=ReleaseFast

# Output: zig-out/bin/z-x64dbg-mcp.exe
```

---

## Installation

### 1. Install Plugin

Copy built plugins to x64dbg folders:

```
build/z-x64dbg-mcp.dp64 → C:\x64dbg\x64\plugins\
build/z-x64dbg-mcp.dp32 → C:\x64dbg\x32\plugins\
```

### 2. Install Server

```powershell
cd server
npm install --production
```

### 3. Configure Claude Desktop

Edit `%APPDATA%\Claude\claude_desktop_config.json`:

```json
{
  "mcpServers": {
    "z-x64dbg": {
      "command": "node",
      "args": ["C:/path/to/z-x64dbg-mcp/server/dist/index.js"],
      "env": {
        "X64DBG_HOST": "localhost",
        "X64DBG_PORT": "31964"
      }
    }
  }
}
```

---

## Testing

### 1. Verify Plugin

1. Start x64dbg
2. Check plugin menu for "Z-x64dbg-MCP"
3. Open log (Help > About > Log)
4. Look for: `[Z-x64dbg-MCP] Plugin initialized`

### 2. Verify REST API

```bash
curl http://localhost:31964/health
# Expected: {"status":"healthy","server":"z-x64dbg-mcp"}
```

### 3. Verify MCP Server

```bash
cd server
npm run dev
# Should start without errors
```

---

## Troubleshooting

### CMake Errors

**Problem:** "x64dbg SDK not found"

**Solution:**
```bash
cmake .. -Dx64dbgDir="C:/x64dbg" -DX64DBG_SDK_DIR="C:/x64dbg/src/sdk"
```

### Build Fails on Windows

**Problem:** MSVC not found

**Solution:**
1. Open "Developer Command Prompt for VS 2022"
2. Run build commands from there

### Plugin Doesn't Load

**Problem:** Plugin not appearing in x64dbg

**Solutions:**
- Ensure `.dp64` file is in `x64\plugins\` (not `plugins\x64\`)
- Check x64dbg log for errors
- Verify plugin architecture matches x64dbg (x64 vs x32)

### TypeScript Build Errors

**Problem:** "Cannot find module"

**Solution:**
```bash
cd server
rm -rf node_modules package-lock.json
npm install
npm run build
```

---

## Clean Build

```bash
# Plugin
cd plugin
rm -rf build

# Server
cd server
rm -rf node_modules dist

# Full clean
git clean -fdx
```

---

## Build Output Structure

```
z-x64dbg-mcp/
├── plugin/
│   ├── build/
│   │   ├── z-x64dbg-mcp.dp64    # x64dbg plugin
│   │   └── z-x64dbg-mcp.dp32    # x32dbg plugin
│   ├── src/
│   │   ├── main.cpp
│   │   ├── plugin.cpp
│   │   ├── rest_server.cpp
│   │   ├── breakpoints.cpp
│   │   ├── memory.cpp
│   │   └── debugger.cpp
│   └── CMakeLists.txt
├── server/
│   ├── dist/
│   │   └── index.js             # MCP server
│   ├── src/
│   │   └── index.ts
│   └── package.json
└── BUILD.md
```

---

## Performance

| Component | Build Time | Binary Size |
|-----------|------------|-------------|
| Plugin (x64) | ~30s | ~2.5 MB |
| Plugin (x32) | ~25s | ~2.3 MB |
| Server (TS) | ~10s | ~15 MB (with deps) |
| Native (Zig) | ~15s | ~800 KB |

---

## Next Steps

After successful build:

1. ✅ Install plugin in x64dbg
2. ✅ Install server dependencies
3. ✅ Configure Claude Desktop
4. ✅ Restart both applications
5. ✅ Start debugging! 🚀

For usage examples, see [README.md](README.md) and [docs/](docs/).
