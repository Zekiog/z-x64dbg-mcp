#pragma once

/**
 * Z-x64dbg-MCP Plugin Core
 * 
 * Internal plugin implementation
 */

#include <windows.h>
#include "bridgegraph.h"

// x64dbg bridge API
namespace Bridge {
    // Debuggee state
    typedef struct {
        ULONG_PTR breakpoint;
        ULONG_PTR exceptionAddress;
        ULONG_PTR exceptionCode;
        ULONG_PTR threadId;
        ULONG_PTR processId;
    } PLUG_DEBUGDATA;
    
    // Module info
    typedef struct {
        char name[256];
        ULONG_PTR base;
        ULONG_PTR size;
    } PLUG_MODULEINFO;
    
    // Plugin structures
    typedef struct {
        int pluginVersion;
        char pluginName[256];
        char pluginDescription[256];
    } PLUG_INFO;
    
    typedef struct { int reserved; } PLUG_INITSTRUCT;
    typedef struct { int reserved; } PLUG_STOPSTRUCT;
    typedef struct { int reserved; } PLUG_SETUPSTRUCT;
    typedef struct { int reserved; } PLUG_BREAKSTRUCT;
    typedef struct { int reserved; } PLUG_CONTINUESTRUCT;
    typedef struct { int reserved; } PLUG_DEBUGSTARTEDSTRUCT;
    typedef struct { int reserved; } PLUG_DEBUGSTOPPEDSTRUCT;
    typedef struct { int reserved; } PLUG_BPTYPE;
    
    // API functions
    PLUG_EXPORT PLUG_DEBUGDATA* GetDebugData();
    PLUG_EXPORT bool DbgCmdExec(const char* cmd);
    PLUG_EXPORT bool DbgMemRead(ULONG_PTR addr, void* data, SIZE_T size);
    PLUG_EXPORT bool DbgMemWrite(ULONG_PTR addr, const void* data, SIZE_T size);
    PLUG_EXPORT ULONG_PTR DbgModBaseFromName(const char* name);
    PLUG_EXPORT bool DbgDisasmAt(ULONG_PTR addr, char* output, int size);
}

// Plugin callback macro
#define CBPLUGIN(name) _CBPLUGIN_##name

// Plugin exports
extern "C" {
    __declspec(dllexport) bool CBPLUGIN(PluginInit, PLUG_INITSTRUCT* initStruct);
    __declspec(dllexport) void CBPLUGIN(PluginStop, PLUG_STOPSTRUCT* stopStruct);
    __declspec(dllexport) void CBPLUGIN(PluginSetup, PLUG_SETUPSTRUCT* setupStruct);
    __declspec(dllexport) void CBPLUGIN(PluginBreak, PLUG_BREAKSTRUCT* breakStruct);
    __declspec(dllexport) void CBPLUGIN(PluginContinue, PLUG_CONTINUESTRUCT* continueStruct);
    __declspec(dllexport) void CBPLUGIN(DebuggingStarted, PLUG_DEBUGSTARTEDSTRUCT* startedStruct);
    __declspec(dllexport) void CBPLUGIN(DebuggingStopped, PLUG_DEBUGSTOPPEDSTRUCT* stoppedStruct);
    __declspec(dllexport) void CBPLUGIN(BreakpointReached, PLUG_BPTYPE* bpType);
    __declspec(dllexport) void CBPLUGIN(ModuleLoad, PLUG_MODULEINFO* info);
    __declspec(dllexport) void CBPLUGIN(ModuleUnload, PLUG_MODULEINFO* info);
    PLUG_EXPORT void CBPLUGIN(PluginGetInfo, PLUG_INFO* info);
}
