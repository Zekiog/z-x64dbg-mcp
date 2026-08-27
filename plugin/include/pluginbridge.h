#pragma once

/**
 * Z-x64dbg-MCP Plugin Bridge
 * 
 * Exposes x64dbg functionality via REST API (localhost:31964)
 * Merged from duty1g and bromoket implementations
 */

#include <windows.h>
#include <string>
#include <functional>

namespace ZBridge {

// REST Server configuration
constexpr const char* DEFAULT_HOST = "localhost";
constexpr int DEFAULT_PORT = 31964;

// Plugin state
struct PluginState {
    bool debuggerActive = false;
    bool pluginLoaded = false;
    ULONG_PTR baseAddress = 0;
    std::string version = "1.0.0";
};

// Breakpoint types
enum class BreakpointType {
    Hardware,
    Software,
    Memory
};

// Memory protection flags
enum class MemoryProtection {
    ReadOnly,
    ReadWrite,
    Execute,
    ExecuteRead,
    ExecuteReadWrite,
    NoAccess
};

// Callback types for events
using BreakpointCallback = std::function<void(ULONG_PTR address)>;
using DebugEventCallback = std::function<void(const char* event)>;
using ModuleCallback = std::function<void(const char* moduleName, bool loaded)>;

// Core API
PluginState& getState();
void setState(const PluginState& state);

// Breakpoint management
bool setBreakpoint(ULONG_PTR address, BreakpointType type, const char* condition = nullptr);
bool deleteBreakpoint(ULONG_PTR address);
bool enableBreakpoint(ULONG_PTR address);
bool disableBreakpoint(ULONG_PTR address);
std::string listBreakpoints();

// Memory operations
std::string readMemory(ULONG_PTR address, SIZE_T size);
bool writeMemory(ULONG_PTR address, const void* data, SIZE_T size);
std::string searchMemory(const char* pattern, ULONG_PTR rangeStart, ULONG_PTR rangeEnd);
bool dumpMemory(ULONG_PTR address, SIZE_T size, const char* outputPath);
bool protectMemory(ULONG_PTR address, SIZE_T size, MemoryProtection protection);

// Debugger control
bool startDebugging(const char* executable, const char* arguments);
bool stopDebugging();
bool attachToProcess(DWORD pid);
bool detachFromProcess();
bool isDebuggingActive();

// Disassembly
std::string disassemble(ULONG_PTR address, int count);
std::string analyzeModule(const char* moduleName);

// Modules
std::string listModules();
std::string getModuleInfo(const char* moduleName);
bool dumpPE(const char* outputPath);

// Tracing
bool stepInto();
bool stepOver();
bool traceExecute(int count);

// Anti-anti-debug
bool bypassPEB();
bool bypassNtQuery();
bool bypassOutputDebugString();

// Event callbacks
void registerBreakpointCallback(BreakpointCallback callback);
void registerDebugEventCallback(DebugEventCallback callback);
void registerModuleCallback(ModuleCallback callback);

// REST API helpers
std::string handleRequest(const char* endpoint, const char* method, const char* body);

} // namespace ZBridge
