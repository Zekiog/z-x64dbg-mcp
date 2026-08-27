/**
 * Z-x64dbg-MCP Plugin Core Implementation
 * 
 * Internal plugin state management and API
 */

#include "plugin.h"
#include "pluginbridge.h"
#include <cstdio>

namespace ZBridge {

static PluginState g_state = {};

PluginState& getState() {
    return g_state;
}

void setState(const PluginState& state) {
    g_state = state;
}

} // namespace ZBridge
