# Z-x64dbg-MCP Server


<!-- BADGES-START -->
[![CI](https://github.com/Zekiog/z-x64dbg-mcp/actions/workflows/ci.yml/badge.svg)](https://github.com/Zekiog/z-x64dbg-mcp/actions/workflows/ci.yml)
![License](https://img.shields.io/github/license/Zekiog/z-x64dbg-mcp)
![Last commit](https://img.shields.io/github/last-commit/Zekiog/z-x64dbg-mcp)
<!-- BADGES-END -->
**Merged & Optimized x64dbg MCP Server** - Combining the best of `duty1g/x64dbg-mcp-server` (71 tools, Zig) and `bromoket/x64dbg_mcp` (23 mega-tools, TypeScript) into a unified, high-performance reverse engineering platform.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![MCP Server](https://img.shields.io/badge/MCP-Server-blue)](https://modelcontextprotocol.io)

## 🚀 Features

- **71+ MCP Tools** - Unified toolset from both implementations
- **Dual Architecture** - Zig (native, zero-dependency) + TypeScript (npm ecosystem)
- **HTTP + SSE + stdio** - Multiple transport protocols
- **x32 & x64 Support** - Full debugger coverage
- **22+ Event Callbacks** - Breakpoint, memory, trace, and debug events
- **REST API Bridge** - 151+ endpoints for AI agents
- **Claude Code Integration** - Direct AI-powered reverse engineering

## 📦 Installation

### Quick Install (PowerShell)
```powershell
# Clone the repository
git clone https://github.com/Zekiog/z-x64dbg-mcp.git
cd z-x64dbg-mcp

# Run the unified installer
.\install.ps1
```

### Manual Setup

#### 1. Install x64dbg Plugin
- Copy `plugin/z-x64dbg-mcp.dp32` to `x32dbg/plugins/`
- Copy `plugin/z-x64dbg-mcp.dp64` to `x64dbg/plugins/`

#### 2. Start MCP Server
```bash
# Zig build (native, recommended)
zig build run

# Or TypeScript (npm ecosystem)
cd server
npm install
npm start
```

#### 3. Configure Claude Code
Add to your `claude_desktop_config.json`:
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

## 🛠️ Tool Categories

| Category | Tools | Description |
|----------|-------|-------------|
| **Breakpoints** | 12 | Hardware, software, and memory breakpoints |
| **Memory** | 15 | Read, write, search, dump, and protect |
| **Disassembly** | 8 | Instructions, analysis, and navigation |
| **Tracing** | 10 | Step, trace, and execution logging |
| **Debug Control** | 14 | Start, stop, pause, and attach |
| **PE & Modules** | 7 | Dump, analyze, and module management |
| **Anti-Anti-Debug** | 5 | Bypass common anti-debugging techniques |

## 📊 Architecture

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│   AI Client     │────▶│  MCP Server      │────▶│  x64dbg Plugin  │
│  (Claude Code)  │◀────│ (Zig/TypeScript) │◀────│  (C++ Bridge)   │
└─────────────────┘     └──────────────────┘     └─────────────────┘
         │                       │                        │
         │  MCP Protocol         │  REST API              │  Debugger
         │  (stdio/SSE/HTTP)     │  (localhost:31964)     │  Events
```

## 🔧 Development

### Build from Source

#### Zig (Native)
```bash
zig build
zig build run
```

#### TypeScript (Server)
```bash
cd server
npm install
npm run build
npm start
```

#### Plugin (C++)
```bash
cd plugin
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Testing
```bash
# Run test suite
npm test

# Integration tests
npm run test:integration
```

## 📖 Documentation

- [Full API Reference](docs/API.md)
- [Tool Specifications](docs/TOOLS.md)
- [Event Callbacks](docs/EVENTS.md)
- [Troubleshooting Guide](docs/TROUBLESHOOTING.md)

## 🤝 Credits

This project merges and optimizes:
- [`duty1g/x64dbg-mcp-server`](https://github.com/duty1g/x64dbg-mcp-server) - 71 tools, Zig implementation
- [`bromoket/x64dbg_mcp`](https://github.com/bromoket/x64dbg_mcp) - 23 mega-tools, TypeScript server

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

## 🌟 Showcase

Example: AI-powered breakpoint analysis
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

---

**Built with ❤️ by Zekiog** | [Report Issues](https://github.com/Zekiog/z-x64dbg-mcp/issues)
