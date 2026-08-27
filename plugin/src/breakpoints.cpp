/**
 * Z-x64dbg-MCP Breakpoint Management
 */

#include "pluginbridge.h"
#include "plugin.h"
#include <cstdio>
#include <map>
#include <string>

namespace ZBridge {

// Breakpoint storage
struct BreakpointInfo {
    ULONG_PTR address;
    BreakpointType type;
    bool enabled;
    std::string condition;
};

static std::map<ULONG_PTR, BreakpointInfo> g_breakpoints;

bool setBreakpoint(ULONG_PTR address, BreakpointType type, const char* condition) {
    if (!g_pluginState.debuggerActive) {
        return false;
    }
    
    BreakpointInfo bp = {};
    bp.address = address;
    bp.type = type;
    bp.enabled = true;
    
    if (condition) {
        bp.condition = condition;
    }
    
    g_breakpoints[address] = bp;
    
    // Execute x64dbg command to set actual breakpoint
    char cmd[256];
    switch (type) {
        case BreakpointType::Hardware:
            sprintf_s(cmd, "bp 0x%p", (void*)address);
            break;
        case BreakpointType::Software:
            sprintf_s(cmd, "bp 0x%p", (void*)address);
            break;
        case BreakpointType::Memory:
            sprintf_s(cmd, "bpm 0x%p", (void*)address);
            break;
    }
    
    Bridge::DbgCmdExec(cmd);
    
    printf("[BP] Set %s breakpoint at 0x%p\n", 
           type == BreakpointType::Hardware ? "hardware" : "software",
           (void*)address);
    
    return true;
}

bool deleteBreakpoint(ULONG_PTR address) {
    auto it = g_breakpoints.find(address);
    if (it == g_breakpoints.end()) {
        return false;
    }
    
    g_breakpoints.erase(it);
    
    char cmd[256];
    sprintf_s(cmd, "bc 0x%p", (void*)address);
    Bridge::DbgCmdExec(cmd);
    
    printf("[BP] Deleted breakpoint at 0x%p\n", (void*)address);
    return true;
}

bool enableBreakpoint(ULONG_PTR address) {
    auto it = g_breakpoints.find(address);
    if (it == g_breakpoints.end()) {
        return false;
    }
    
    it->second.enabled = true;
    
    char cmd[256];
    sprintf_s(cmd, "be 0x%p", (void*)address);
    Bridge::DbgCmdExec(cmd);
    
    return true;
}

bool disableBreakpoint(ULONG_PTR address) {
    auto it = g_breakpoints.find(address);
    if (it == g_breakpoints.end()) {
        return false;
    }
    
    it->second.enabled = false;
    
    char cmd[256];
    sprintf_s(cmd, "bd 0x%p", (void*)address);
    Bridge::DbgCmdExec(cmd);
    
    return true;
}

std::string listBreakpoints() {
    std::string result = "{";
    result += "\"breakpoints\":[";
    
    bool first = true;
    for (const auto& [addr, bp] : g_breakpoints) {
        if (!first) result += ",";
        first = false;
        
        char entry[512];
        sprintf_s(entry,
            "{\"address\":\"0x%p\",\"type\":\"%s\",\"enabled\":%s}",
            (void*)addr,
            bp.type == BreakpointType::Hardware ? "hardware" : 
            bp.type == BreakpointType::Software ? "software" : "memory",
            bp.enabled ? "true" : "false"
        );
        result += entry;
    }
    
    result += "]}";
    return result;
}

} // namespace ZBridge
