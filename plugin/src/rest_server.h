#pragma once

/**
 * Z-x64dbg-MCP REST Server
 * 
 * HTTP server for MCP communication (localhost:31964)
 */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <map>

struct RestServer {
    RestServer(const char* host, int port);
    ~RestServer();
    
    bool start();
    void stop();
    
    // Event emission (SSE)
    void emitEvent(const char* eventName, const char* data);
    
    // Request handler
    std::string handleRequest(const std::string& endpoint, 
                              const std::string& method,
                              const std::string& body);
    
private:
    void serverLoop();
    SOCKET createServerSocket();
    std::string handleBreakpoint(const std::string& method, const std::string& body);
    std::string handleMemory(const std::string& method, const std::string& body);
    std::string handleDebug(const std::string& method, const std::string& body);
    std::string handleModules(const std::string& method, const std::string& body);
    std::string handleDisasm(const std::string& method, const std::string& body);
    std::string handleAntiDebug(const std::string& method, const std::string& body);
    
    std::string host;
    int port;
    SOCKET serverSocket;
    std::atomic<bool> running;
    std::thread serverThread;
    
    // SSE clients
    std::map<SOCKET, bool> sseClients;
};
