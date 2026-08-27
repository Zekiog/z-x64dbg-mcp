/**
 * Z-x64dbg-MCP Memory Operations
 */

#include "pluginbridge.h"
#include "plugin.h"
#include <cstdio>
#include <vector>
#include <fstream>

namespace ZBridge {

std::string readMemory(ULONG_PTR address, SIZE_T size) {
    if (!g_pluginState.debuggerActive) {
        return "";
    }
    
    std::vector<BYTE> buffer(size);
    if (!Bridge::DbgMemRead(address, buffer.data(), size)) {
        return "";
    }
    
    // Convert to hex string
    std::string result;
    result.reserve(size * 3);
    
    for (SIZE_T i = 0; i < size; i++) {
        char byte[4];
        sprintf_s(byte, "%02X ", buffer[i]);
        result += byte;
    }
    
    return result;
}

bool writeMemory(ULONG_PTR address, const void* data, SIZE_T size) {
    if (!g_pluginState.debuggerActive) {
        return false;
    }
    
    return Bridge::DbgMemWrite(address, data, size);
}

std::string searchMemory(const char* pattern, ULONG_PTR rangeStart, ULONG_PTR rangeEnd) {
    // Pattern search implementation (simplified)
    // In production: use x64dbg's pattern search API
    
    std::string result = "{\"matches\":[]}";
    return result;
}

bool dumpMemory(ULONG_PTR address, SIZE_T size, const char* outputPath) {
    if (!g_pluginState.debuggerActive) {
        return false;
    }
    
    std::vector<BYTE> buffer(size);
    if (!Bridge::DbgMemRead(address, buffer.data(), size)) {
        return false;
    }
    
    std::ofstream file(outputPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(buffer.data()), size);
    file.close();
    
    printf("[MEM] Dumped %zu bytes from 0x%p to %s\n", 
           size, (void*)address, outputPath);
    
    return true;
}

bool protectMemory(ULONG_PTR address, SIZE_T size, MemoryProtection protection) {
    // Memory protection change (simplified)
    // In production: use x64dbg's memory protection API
    
    DWORD oldProtect;
    DWORD newProtect;
    
    switch (protection) {
        case MemoryProtection::ReadOnly:
            newProtect = PAGE_READONLY;
            break;
        case MemoryProtection::ReadWrite:
            newProtect = PAGE_READWRITE;
            break;
        case MemoryProtection::Execute:
            newProtect = PAGE_EXECUTE;
            break;
        case MemoryProtection::ExecuteRead:
            newProtect = PAGE_EXECUTE_READ;
            break;
        case MemoryProtection::ExecuteReadWrite:
            newProtect = PAGE_EXECUTE_READWRITE;
            break;
        case MemoryProtection::NoAccess:
            newProtect = PAGE_NOACCESS;
            break;
        default:
            return false;
    }
    
    // VirtualProtectEx would be called here in production
    return true;
}

} // namespace ZBridge
