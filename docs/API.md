# Z-x64dbg-MCP API Reference

## Overview

The Z-x64dbg-MCP server exposes 71+ tools through the Model Context Protocol (MCP), enabling AI agents to control x64dbg for reverse engineering tasks.

## Transport Protocols

### stdio (Recommended)
```json
{
  "command": "node",
  "args": ["/path/to/server/dist/index.js"]
}
```

### HTTP + SSE
```bash
# Server starts on localhost:31965
# SSE events: /sse
# HTTP POST: /message
```

## Tool Categories

### 1. Breakpoint Tools (12 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.set_breakpoint` | Set hardware/software breakpoint | `address`, `type`, `condition` |
| `x64dbg.delete_breakpoint` | Remove breakpoint | `address` |
| `x64dbg.list_breakpoints` | List all active breakpoints | - |
| `x64dbg.enable_breakpoint` | Enable disabled breakpoint | `address` |
| `x64dbg.disable_breakpoint` | Disable without deleting | `address` |

### 2. Memory Tools (15 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.read_memory` | Read memory region | `address`, `size` |
| `x64dbg.write_memory` | Write to memory | `address`, `data` |
| `x64dbg.search_memory` | Search for pattern | `pattern`, `range` |
| `x64dbg.dump_memory` | Dump memory to file | `address`, `size`, `path` |
| `x64dbg.protect_memory` | Change memory protection | `address`, `size`, `protection` |

### 3. Disassembly Tools (8 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.disassemble` | Disassemble at address | `address`, `count` |
| `x64dbg.analyze_module` | Analyze module | `module_name` |
| `x64dbg.find_references` | Find code references | `address` |

### 4. Tracing Tools (10 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.step_into` | Step into instruction | - |
| `x64dbg.step_over` | Step over instruction | - |
| `x64dbg.trace_execute` | Execute with trace logging | `count` |

### 5. Debug Control (14 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.start_debugging` | Start debug session | `executable`, `arguments` |
| `x64dbg.stop_debugging` | Stop debugging | - |
| `x64dbg.attach` | Attach to process | `pid` |
| `x64dbg.detach` | Detach from process | - |

### 6. PE & Module Tools (7 tools)

| Tool | Description | Parameters |
|------|-------------|------------|
| `x64dbg.dump_pe` | Dump PE file | `path` |
| `x64dbg.list_modules` | List loaded modules | - |
| `x64dbg.get_module_info` | Get module details | `module_name` |

### 7. Anti-Anti-Debug (5 tools)

| Tool | Description |
|------|-------------|
| `x64dbg.bypass_peb` | Bypass PEB checks |
| `x64dbg.bypass_ntquery` | Bypass NtQueryInformationProcess |
| `x64dbg.bypass_outputdebugstring` | Bypass OutputDebugStringA traps |

## Example Usage

### Claude Code Integration
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

### Tool Call Example
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

## Error Codes

| Code | Description |
|------|-------------|
| `ERR_DEBUGGER_NOT_RUNNING` | No active debug session |
| `ERR_INVALID_ADDRESS` | Invalid memory address |
| `ERR_BREAKPOINT_EXISTS` | Breakpoint already set |
| `ERR_MODULE_NOT_FOUND` | Module not loaded |

## Rate Limiting

- Default: 100 requests/second
- Configurable via `X64DBG_RATE_LIMIT` env var
