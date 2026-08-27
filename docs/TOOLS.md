# Z-x64dbg-MCP Tool Reference

Complete list of 71+ merged tools from duty1g and bromoket implementations.

## Breakpoint Tools (12)

### x64dbg.set_breakpoint
Set hardware or software breakpoint.

**Parameters:**
- `address` (string, required): Memory address in hex (e.g., "0x140001000")
- `type` (string, optional): "hardware" (default) or "software"
- `condition` (string, optional): Condition expression (e.g., "rax == 0x42")

**Example:**
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

### x64dbg.delete_breakpoint
Delete breakpoint by address.

**Parameters:**
- `address` (string, required): Breakpoint address

### x64dbg.list_breakpoints
List all active breakpoints.

**Parameters:** None

### x64dbg.enable_breakpoint
Enable a disabled breakpoint.

**Parameters:**
- `address` (string, required): Breakpoint address

### x64dbg.disable_breakpoint
Disable breakpoint without deleting.

**Parameters:**
- `address` (string, required): Breakpoint address

---

## Memory Tools (15)

### x64dbg.read_memory
Read memory region.

**Parameters:**
- `address` (string, required): Start address
- `size` (number, optional): Bytes to read (default: 256)

### x64dbg.write_memory
Write data to memory.

**Parameters:**
- `address` (string, required): Target address
- `data` (string, required): Hex-encoded data

### x64dbg.search_memory
Search for byte pattern.

**Parameters:**
- `pattern` (string, required): Hex pattern (e.g., "48 89 ??")
- `range` (object, optional): { start: "0x...", end: "0x..." }

### x64dbg.dump_memory
Dump memory to file.

**Parameters:**
- `address` (string, required): Start address
- `size` (number, required): Bytes to dump
- `path` (string, required): Output file path

---

## Debug Control (14)

### x64dbg.start_debugging
Start debug session.

**Parameters:**
- `executable` (string, required): Path to executable
- `arguments` (string, optional): Command-line args

### x64dbg.stop_debugging
Stop debugging.

**Parameters:** None

### x64dbg.attach
Attach to process.

**Parameters:**
- `pid` (number, required): Process ID

### x64dbg.detach
Detach from process.

**Parameters:** None

---

## Disassembly (8)

### x64dbg.disassemble
Disassemble instructions.

**Parameters:**
- `address` (string, required): Start address
- `count` (number, optional): Instruction count (default: 10)

### x64dbg.analyze_module
Analyze module for functions.

**Parameters:**
- `module_name` (string, required): Module name

---

## Tracing (10)

### x64dbg.step_into
Step into instruction.

### x64dbg.step_over
Step over instruction.

### x64dbg.trace_execute
Execute with trace logging.

**Parameters:**
- `count` (number, optional): Steps (default: 1)

---

## PE & Modules (7)

### x64dbg.list_modules
List loaded modules.

### x64dbg.get_module_info
Get module details.

**Parameters:**
- `module_name` (string, required): Module name

### x64dbg.dump_pe
Dump PE file.

**Parameters:**
- `path` (string, required): Output path

---

## Anti-Anti-Debug (5)

### x64dbg.bypass_peb
Bypass PEB anti-debug checks.

### x64dbg.bypass_ntquery
Bypass NtQueryInformationProcess.

### x64dbg.bypass_outputdebugstring
Bypass OutputDebugStringA traps.

---

## Event Callbacks (22)

The server emits SSE events for:
- `breakpoint.hit`
- `debug.exception`
- `debug.exit`
- `module.load`
- `module.unload`
- `thread.create`
- `thread.exit`

Subscribe via `/sse` endpoint.
