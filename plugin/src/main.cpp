/**
 * Z-x64dbg-MCP Plugin Entry Point
 * 
 * Merged implementation from:
 * - duty1g/x64dbg-mcp-server (71 tools, Zig)
 * - bromoket/x64dbg_mcp (23 mega-tools, TypeScript)
 * 
 * License: MIT
 */

#include <windows.h>
#include <string>
#include <cstdio>

#include "pluginbridge.h"
#include "plugin.h"
#include "rest_server.h"

// Global plugin state
static ZBridge::PluginState g_pluginState;
static RestServer* g_restServer = nullptr;

// Plugin exports for x64dbg
extern "C" {

__declspec(dllexport) bool CBPLUGIN(PluginInit, PLUG_INITSTRUCT* initStruct) {
    ZBridge::setState(g_pluginState);
    g_pluginState.pluginLoaded = true;
    g_pluginState.version = "1.0.0";
    
    printf("[Z-x64dbg-MCP] Plugin initialized (v%s)\n", g_pluginState.version.c_str());
    
    // Initialize REST server
    g_restServer = new RestServer(ZBridge::DEFAULT_HOST, ZBridge::DEFAULT_PORT);
    if (g_restServer->start()) {
        printf("[Z-x64dbg-MCP] REST API listening on http://%s:%d\n", 
               ZBridge::DEFAULT_HOST, ZBridge::DEFAULT_PORT);
    } else {
        printf("[Z-x64dbg-MCP] Failed to start REST server\n");
        delete g_restServer;
        g_restServer = nullptr;
    }
    
    return true;
}

__declspec(dllexport) void CBPLUGIN(PluginStop, PLUG_STOPSTRUCT* stopStruct) {
    g_pluginState.pluginLoaded = false;
    ZBridge::setState(g_pluginState);
    
    if (g_restServer) {
        g_restServer->stop();
        delete g_restServer;
        g_restServer = nullptr;
    }
    
    printf("[Z-x64dbg-MCP] Plugin stopped\n");
}

__declspec(dllexport) void CBPLUGIN(PluginSetup, PLUG_SETUPSTRUCT* setupStruct) {
    // Plugin setup (menu, shortcuts, etc.)
    printf("[Z-x64dbg-MCP] Setup complete\n");
}

__declspec(dllexport) void CBPLUGIN(PluginBreak, PLUG_BREAKSTRUCT* breakStruct) {
    // Debugger break event
    if (g_restServer) {
        g_restServer->emitEvent("debug.break", "{}" );
    }
}

__declspec(dllexport) void CBPLUGIN(PluginContinue, PLUG_CONTINUESTRUCT* continueStruct) {
    // Debugger continue event
    if (g_restServer) {
        g_restServer->emitEvent("debug.continue", "{}" );
    }
}

__declspec(dllexport) void CBPLUGIN(DebuggingStarted, PLUG_DEBUGSTARTEDSTRUCT* startedStruct) {
    g_pluginState.debuggerActive = true;
    ZBridge::setState(g_pluginState);
    
    if (g_restServer) {
        g_restServer->emitEvent("debug.started", "{}" );
    }
    
    printf("[Z-x64dbg-MCP] Debugging started\n");
}

__declspec(dllexport) void CBPLUGIN(DebuggingStopped, PLUG_DEBUGSTOPPEDSTRUCT* stoppedStruct) {
    g_pluginState.debuggerActive = false;
    ZBridge::setState(g_pluginState);
    
    if (g_restServer) {
        g_restServer->emitEvent("debug.stopped", "{}" );
    }
    
    printf("[Z-x64dbg-MCP] Debugging stopped\n");
}

__declspec(dllexport) void CBPLUGIN(BreakpointReached, PLUG_BPTYPE* bpType) {
    // Breakpoint hit - emit event via REST
    if (g_restServer) {
        char buffer[256];
        sprintf_s(buffer, "{\"address\":\"0x%p\"}", (void*)Bridge::GetDebugData()->breakpoint);
        g_restServer->emitEvent("breakpoint.hit", buffer);
    }
}

__declspec(dllexport) void CBPLUGIN(ModuleLoad, PLUG_MODULEINFO* info) {
    // Module loaded
    if (g_restServer && info) {
        char buffer[512];
        sprintf_s(buffer, "{\"name\":\"%s\",\"base\":\"0x%p\",\"loaded\":true}", 
                  info->name, (void*)info->base);
        g_restServer->emitEvent("module.load", buffer);
    }
}

__declspec(dllexport) void CBPLUGIN(ModuleUnload, PLUG_MODULEINFO* info) {
    // Module unloaded
    if (g_restServer && info) {
        char buffer[512];
        sprintf_s(buffer, "{\"name\":\"%s\",\"base\":\"0x%p\",\"loaded\":false}", 
                  info->name, (void*)info->base);
        g_restServer->emitEvent("module.unload", buffer);
    }
}

} // extern "C"

// Plugin metadata
PLUG_EXPORT void CBPLUGIN(PluginGetInfo, PLUG_INFO* info) {
    if (info) {
        info->pluginVersion = 1;
        strcpy_s(info->pluginName, "Z-x64dbg-MCP");
        strcpy_s(info->pluginDescription, "Merged MCP Server Plugin (duty1g + bromoket)");
    }
}
