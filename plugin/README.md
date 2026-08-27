# Z-x64dbg-MCP Plugin

C++ plugin bridge for x64dbg/x32dbg. Exposes debugger functionality via REST API (localhost:31964).

## Build Instructions

### Prerequisites
- Visual Studio 2022 with C++ workload
- CMake 3.20+
- x64dbg SDK

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Configure (x64)
cmake .. -G "Visual Studio 17 2022" -A x64 -Dx64dbg_DIR="C:/x64dbg"

# Build
cmake --build . --config Release

# Output
# - z-x64dbg-mcp.dp64 (x64dbg plugin)
# - z-x64dbg-mcp.dp32 (x32dbg plugin - build with -A Win32)
```

## Installation

Copy built `.dp64` / `.dp32` files to:
- `x64dbg/plugins/`
- `x32dbg/plugins/`

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/breakpoint/set` | POST | Set breakpoint |
| `/breakpoint/delete` | POST | Delete breakpoint |
| `/memory/read` | POST | Read memory |
| `/memory/write` | POST | Write memory |
| `/debug/start` | POST | Start debugging |
| `/debug/stop` | POST | Stop debugging |
| `/modules/list` | GET | List modules |
| `/disasm` | POST | Disassemble |

## Example Request

```bash
curl -X POST http://localhost:31964/breakpoint/set \
  -H "Content-Type: application/json" \
  -d '{"address":"0x140001000","type":"hardware"}'
```

## License

MIT - See root LICENSE
