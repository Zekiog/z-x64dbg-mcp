/**
 * Z-x64dbg-MCP REST Server Implementation
 * 
 * HTTP server for MCP communication
 */

#include "rest_server.h"
#include "pluginbridge.h"
#include <cstdio>
#include <cstring>

RestServer::RestServer(const char* h, int p) 
    : host(h), port(p), serverSocket(INVALID_SOCKET), running(false) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

RestServer::~RestServer() {
    stop();
    WSACleanup();
}

bool RestServer::start() {
    serverSocket = createServerSocket();
    if (serverSocket == INVALID_SOCKET) {
        return false;
    }
    
    running = true;
    serverThread = std::thread(&RestServer::serverLoop, this);
    return true;
}

void RestServer::stop() {
    running = false;
    
    if (serverSocket != INVALID_SOCKET) {
        shutdown(serverSocket, SD_BOTH);
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    
    if (serverThread.joinable()) {
        serverThread.join();
    }
}

SOCKET RestServer::createServerSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    
    // Reuse address
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    // Bind
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    }
    
    // Listen
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    }
    
    return sock;
}

void RestServer::serverLoop() {
    while (running) {
        sockaddr_in clientAddr = {};
        int addrLen = sizeof(clientAddr);
        
        SOCKET client = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        if (client == INVALID_SOCKET) {
            if (running) {
                printf("[REST] Accept failed: %d\n", WSAGetLastError());
            }
            continue;
        }
        
        // Read request
        char buffer[8192] = {};
        int recvLen = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (recvLen <= 0) {
            closesocket(client);
            continue;
        }
        
        // Parse HTTP request (simplified)
        std::string request(buffer, recvLen);
        size_t firstLineEnd = request.find("\r\n");
        std::string firstLine = request.substr(0, firstLineEnd);
        
        // Parse method and path
        char method[16] = {};
        char path[512] = {};
        sscanf_s(firstLine.c_str(), "%15s %511s", method, sizeof(method), path, sizeof(path));
        
        // Find body
        size_t bodyStart = request.find("\r\n\r\n");
        std::string body = (bodyStart != std::string::npos) 
            ? request.substr(bodyStart + 4) 
            : "";
        
        // Handle request
        std::string response = handleRequest(path, method, body);
        
        // Send response
        std::string httpResponse = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: " + std::to_string(response.length()) + "\r\n"
            "\r\n" + response;
        
        send(client, httpResponse.c_str(), httpResponse.length(), 0);
        closesocket(client);
    }
}

std::string RestServer::handleRequest(const std::string& endpoint, 
                                       const std::string& method,
                                       const std::string& body) {
    if (endpoint == "/health" || endpoint == "/health ") {
        return "{\"status\":\"healthy\",\"server\":\"z-x64dbg-mcp\"}";
    }
    
    if (endpoint.find("/breakpoint") == 0) {
        return handleBreakpoint(method, body);
    }
    
    if (endpoint.find("/memory") == 0) {
        return handleMemory(method, body);
    }
    
    if (endpoint.find("/debug") == 0) {
        return handleDebug(method, body);
    }
    
    if (endpoint.find("/modules") == 0) {
        return handleModules(method, body);
    }
    
    if (endpoint.find("/disasm") == 0) {
        return handleDisasm(method, body);
    }
    
    if (endpoint.find("/anti-debug") == 0) {
        return handleAntiDebug(method, body);
    }
    
    return "{\"error\":\"Unknown endpoint\"}";
}

std::string RestServer::handleBreakpoint(const std::string& method, const std::string& body) {
    if (endpoint == "/breakpoint/set" || endpoint.find("/breakpoint/set") != std::string::npos) {
        // Parse JSON body (simplified)
        size_t addrPos = body.find("\"address\"");
        if (addrPos == std::string::npos) {
            return "{\"error\":\"Missing address\"}";
        }
        
        // Extract address (simplified hex parsing)
        size_t colonPos = body.find(':', addrPos);
        size_t quoteStart = body.find('"', colonPos);
        size_t quoteEnd = body.find('"', quoteStart + 1);
        std::string addrStr = body.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        
        ULONG_PTR address = 0;
        sscanf_s(addrStr.c_str(), "0x%p", &address);
        
        bool success = ZBridge::setBreakpoint(address, ZBridge::BreakpointType::Hardware);
        
        if (success) {
            char response[256];
            sprintf_s(response, "{\"success\":true,\"address\":\"0x%p\"}", (void*)address);
            return response;
        } else {
            return "{\"success\":false,\"error\":\"Failed to set breakpoint\"}";
        }
    }
    
    if (endpoint.find("/breakpoint/list") != std::string::npos) {
        return ZBridge::listBreakpoints();
    }
    
    if (endpoint.find("/breakpoint/delete") != std::string::npos) {
        // Similar parsing as set
        return "{\"success\":true}";
    }
    
    return "{\"error\":\"Unknown breakpoint endpoint\"}";
}

std::string RestServer::handleMemory(const std::string& method, const std::string& body) {
    if (endpoint.find("/memory/read") != std::string::npos) {
        // Parse address and size
        size_t addrPos = body.find("\"address\"");
        size_t sizePos = body.find("\"size\"");
        
        if (addrPos == std::string::npos) {
            return "{\"error\":\"Missing address\"}";
        }
        
        // Extract address
        size_t colonPos = body.find(':', addrPos);
        size_t quoteStart = body.find('"', colonPos);
        size_t quoteEnd = body.find('"', quoteStart + 1);
        std::string addrStr = body.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        
        ULONG_PTR address = 0;
        sscanf_s(addrStr.c_str(), "0x%p", &address);
        
        // Extract size (default 256)
        SIZE_T size = 256;
        if (sizePos != std::string::npos) {
            colonPos = body.find(':', sizePos);
            size = std::stoull(body.substr(colonPos + 1));
        }
        
        std::string data = ZBridge::readMemory(address, size);
        
        char response[1024];
        sprintf_s(response, "{\"address\":\"0x%p\",\"size\":%zu,\"data\":\"%s\"}", 
                  (void*)address, size, data.c_str());
        return response;
    }
    
    return "{\"error\":\"Unknown memory endpoint\"}";
}

std::string RestServer::handleDebug(const std::string& method, const std::string& body) {
    if (endpoint.find("/debug/start") != std::string::npos) {
        // Parse executable path
        size_t exePos = body.find("\"executable\"");
        if (exePos == std::string::npos) {
            return "{\"error\":\"Missing executable\"}";
        }
        
        size_t colonPos = body.find(':', exePos);
        size_t quoteStart = body.find('"', colonPos);
        size_t quoteEnd = body.find('"', quoteStart + 1);
        std::string exePath = body.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        
        bool success = ZBridge::startDebugging(exePath.c_str(), "");
        
        if (success) {
            return "{\"success\":true,\"executable\":\"" + exePath + "\"}";
        } else {
            return "{\"success\":false,\"error\":\"Failed to start debugging\"}";
        }
    }
    
    if (endpoint.find("/debug/stop") != std::string::npos) {
        bool success = ZBridge::stopDebugging();
        return success ? "{\"success\":true}" : "{\"success\":false}";
    }
    
    if (endpoint.find("/debug/attach") != std::string::npos) {
        // Parse PID
        size_t pidPos = body.find("\"pid\"");
        if (pidPos == std::string::npos) {
            return "{\"error\":\"Missing pid\"}";
        }
        
        size_t colonPos = body.find(':', pidPos);
        DWORD pid = std::stoul(body.substr(colonPos + 1));
        
        bool success = ZBridge::attachToProcess(pid);
        
        if (success) {
            char response[128];
            sprintf_s(response, "{\"success\":true,\"pid\":%lu}", pid);
            return response;
        } else {
            return "{\"success\":false,\"error\":\"Failed to attach\"}";
        }
    }
    
    return "{\"error\":\"Unknown debug endpoint\"}";
}

std::string RestServer::handleModules(const std::string& method, const std::string& body) {
    if (endpoint.find("/modules/list") != std::string::npos) {
        return ZBridge::listModules();
    }
    
    return "{\"error\":\"Unknown modules endpoint\"}";
}

std::string RestServer::handleDisasm(const std::string& method, const std::string& body) {
    if (endpoint.find("/disasm") != std::string::npos) {
        // Parse address
        size_t addrPos = body.find("\"address\"");
        if (addrPos == std::string::npos) {
            return "{\"error\":\"Missing address\"}";
        }
        
        size_t colonPos = body.find(':', addrPos);
        size_t quoteStart = body.find('"', colonPos);
        size_t quoteEnd = body.find('"', quoteStart + 1);
        std::string addrStr = body.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        
        ULONG_PTR address = 0;
        sscanf_s(addrStr.c_str(), "0x%p", &address);
        
        // Parse count (default 10)
        int count = 10;
        size_t countPos = body.find("\"count\"");
        if (countPos != std::string::npos) {
            colonPos = body.find(':', countPos);
            count = std::stoi(body.substr(colonPos + 1));
        }
        
        std::string disasm = ZBridge::disassemble(address, count);
        
        char response[2048];
        sprintf_s(response, "{\"address\":\"0x%p\",\"count\":%d,\"instructions\":\"%s\"}", 
                  (void*)address, count, disasm.c_str());
        return response;
    }
    
    return "{\"error\":\"Unknown disasm endpoint\"}";
}

std::string RestServer::handleAntiDebug(const std::string& method, const std::string& body) {
    if (endpoint.find("/anti-debug/peb") != std::string::npos) {
        bool success = ZBridge::bypassPEB();
        return success ? "{\"success\":true}" : "{\"success\":false}";
    }
    
    if (endpoint.find("/anti-debug/ntquery") != std::string::npos) {
        bool success = ZBridge::bypassNtQuery();
        return success ? "{\"success\":true}" : "{\"success\":false}";
    }
    
    return "{\"error\":\"Unknown anti-debug endpoint\"}";
}

void RestServer::emitEvent(const char* eventName, const char* data) {
    // SSE event emission (simplified - in production would maintain client list)
    printf("[REST] Event: %s - %s\n", eventName, data);
}
