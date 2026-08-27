# Z-x64dbg-MCP Skill

## Overview

Z-x64dbg-MCP is a merged and optimized MCP server that combines:
- **duty1g/x64dbg-mcp-server**: 71 tools, Zig implementation, native performance
- **bromoket/x64dbg_mcp**: 23 mega-tools, TypeScript server, npm ecosystem

Result: **71+ unified tools** for AI-powered reverse engineering with x64dbg.

## Installation

### Quick Install
```powershell
git clone https://github.com/Zekiog/z-x64dbg-mcp.git
cd z-x64dbg-mcp
.\install.ps1
```

### Manual Setup

1. **Build Plugin** (C++)
```bash
cd plugin
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

2. **Install Server** (TypeScript)
```bash
cd server
npm install
npm run build
```

3. **Configure Claude Desktop**
```json
{
  "mcpServers": {
    "z-x64dbg": {
      "command": "node",
      "args": ["/path/to/z-x64dbg-mcp/server/dist/index.js"],
      "env": {
        "X64DBG_HOST": "localhost",
        "X64DBG_PORT": "31964"
      }
    }
  }
}
```

## Usage

### Example: Set Breakpoint
```json
{
  "tool": "x64dbg.set_breakpoint",
  "arguments": {
    "address": "0x140001000",
    "type": "hardware",
    "condition": "rax == 0x42"
  }
}
```

### Example: Read Memory
```json
{
  "tool": "x64dbg.read_memory",
  "arguments": {
    "address": "0x140001000",
    "size": 512
  }
}
```

### Example: Debug Executable
```json
{
  "tool": "x64dbg.start_debugging",
  "arguments": {
    "executable": "C:\\target.exe",
    "arguments": "--flag"
  }
}
```

## Tool Categories

| Category | Count | Description |
|----------|-------|-------------|
| Breakpoints | 12 | Hardware, software, memory BP |
| Memory | 15 | Read, write, search, dump |
| Debug Control | 14 | Start, stop, attach, detach |
| Disassembly | 8 | Instructions, analysis |
| Tracing | 10 | Step, trace, log |
| PE & Modules | 7 | Dump, list, info |
| Anti-Anti-Debug | 5 | Bypass common checks |
| Events | 22+ | SSE callbacks |

## Architecture

```
AI Client (Claude) → MCP Server (stdio/SSE) → REST API (localhost:31964) → x64dbg Plugin (C++)
```

## Transport Options

- **stdio** (recommended): Direct pipe, lowest latency
- **HTTP + SSE**: Network-transparent, event streaming
- **Combined**: stdio for commands, SSE for events

## Performance

- Native Zig build: ~5ms latency
- TypeScript server: ~10ms latency
- REST bridge: ~15ms round-trip

## Troubleshooting

1. Check x64dbg plugin loaded
2. Verify REST API: `curl http://localhost:31964/health`
3. Restart Claude Desktop
4. Check server logs

## Credits

- Original duty1g implementation: https://github.com/duty1g/x64dbg-mcp-server
- Original bromoket implementation: https://github.com/bromoket/x64dbg_mcp

## License

MIT - See LICENSE
