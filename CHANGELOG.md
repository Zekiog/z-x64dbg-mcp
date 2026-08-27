# Changelog

## [1.0.0] - 2026-08-27

### ✨ Initial Release

**Merged Implementations:**
- Combined duty1g/x64dbg-mcp-server (71 tools, Zig)
- Combined bromoket/x64dbg_mcp (23 mega-tools, TypeScript)

**Features:**
- 71+ unified MCP tools
- Dual architecture (Zig + TypeScript)
- HTTP + SSE + stdio transports
- x32 & x64 debugger support
- 22+ event callbacks
- REST API bridge (151 endpoints)
- Claude Code integration

**Tool Categories:**
- Breakpoints: 12 tools
- Memory: 15 tools
- Debug Control: 14 tools
- Disassembly: 8 tools
- Tracing: 10 tools
- PE & Modules: 7 tools
- Anti-Anti-Debug: 5 tools

**Infrastructure:**
- TypeScript MCP server with Express bridge
- C++ plugin skeleton
- PowerShell installer
- CI/CD workflows (GitHub Actions)
- Comprehensive documentation (API, TOOLS, TROUBLESHOOTING)

**Optimizations:**
- Unified tool naming convention
- Merged overlapping functionality
- Added error handling and validation
- Improved rate limiting
- Enhanced logging

### 🔧 Technical Stack

- **Server**: TypeScript/Node.js, Express, @modelcontextprotocol/sdk
- **Plugin**: C++, CMake, x64dbg SDK
- **Native**: Zig (build.zig)
- **CI**: GitHub Actions (lint, test, build)
- **Docs**: Markdown (API reference, troubleshooting)

### 📦 Installation

```powershell
git clone https://github.com/Zekiog/z-x64dbg-mcp.git
cd z-x64dbg-mcp
.\install.ps1
```

### 🚀 Quick Start

1. Build plugin: `cd plugin && cmake -B build && cmake --build build`
2. Install server: `cd server && npm install && npm run build`
3. Configure Claude Desktop (see README.md)
4. Restart x64dbg and Claude Desktop
5. Start debugging!

---

**Built by Zekiog** | [GitHub](https://github.com/Zekiog/z-x64dbg-mcp)
