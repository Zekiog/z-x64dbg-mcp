cmake_minimum_required(VERSION 3.20)
project(z-x64dbg-mcp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# x64dbg SDK path (set via -Dx64dbg_DIR or environment)
if(NOT DEFINED x64dbg_DIR)
    set(x64dbg_DIR "C:/x64dbg" CACHE PATH "x64dbg installation directory")
endif()

# Find x64dbg SDK
find_path(X64DBG_SDK_DIR 
    NAMES "x64dbg_bridge.h" "x64dbg_plugin.h"
    PATHS "${x64dbg_DIR}/src/sdk" "${x64dbg_DIR}/sdk"
)

if(NOT X64DBG_SDK_DIR)
    message(WARNING "x64dbg SDK not found. Plugin will build but may not load.")
endif()

# Plugin sources
set(PLUGIN_SOURCES
    src/main.cpp
    src/plugin.cpp
    src/plugin.h
    src/rest_server.cpp
    src/rest_server.h
    src/breakpoints.cpp
    src/breakpoints.h
    src/memory.cpp
    src/memory.h
    src/debugger.cpp
    src/debugger.h
)

# x64dbg plugin (x64)
add_library(z-x64dbg-mcp-x64 SHARED ${PLUGIN_SOURCES})
target_include_directories(z-x64dbg-mcp-x64 PRIVATE
    ${X64DBG_SDK_DIR}
    include
)
target_compile_definitions(z-x64dbg-mcp-x64 PRIVATE
    _WIN32
    WIN32
    _WINDOWS
    UNICODE
    _UNICODE
    BUILDING_DLL
)
target_link_libraries(z-x64dbg-mcp-x64 PRIVATE
    ws2_32
    wsock32
)
set_target_properties(z-x64dbg-mcp-x64 PROPERTIES
    OUTPUT_NAME "z-x64dbg-mcp"
    SUFFIX ".dp64"
    PREFIX ""
)

# x32dbg plugin (x86)
add_library(z-x64dbg-mcp-x32 SHARED ${PLUGIN_SOURCES})
target_include_directories(z-x64dbg-mcp-x32 PRIVATE
    ${X64DBG_SDK_DIR}
    include
)
target_compile_definitions(z-x64dbg-mcp-x32 PRIVATE
    _WIN32
    WIN32
    _WINDOWS
    UNICODE
    _UNICODE
    BUILDING_DLL
    _M_IX86
)
target_link_libraries(z-x64dbg-mcp-x32 PRIVATE
    ws2_32
    wsock32
)
set_target_properties(z-x64dbg-mcp-x32 PROPERTIES
    OUTPUT_NAME "z-x64dbg-mcp"
    SUFFIX ".dp32"
    PREFIX ""
    WIN32_EXECUTABLE TRUE
)

# Install targets
install(TARGETS z-x64dbg-mcp-x64
    LIBRARY DESTINATION "${x64dbg_DIR}/x64/plugins"
)
install(TARGETS z-x64dbg-mcp-x32
    LIBRARY DESTINATION "${x64dbg_DIR}/x32/plugins"
)

message(STATUS "x64dbg SDK: ${X64DBG_SDK_DIR}")
message(STATUS "Output: .dp64 and .dp32 plugins")
