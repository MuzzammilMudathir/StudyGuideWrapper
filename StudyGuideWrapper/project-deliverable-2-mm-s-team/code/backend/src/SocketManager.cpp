#include "SocketManager.h"
#include <iostream>

// Constructor
SocketManager::SocketManager(int port) : port(port), serverSocket(INVALID_SOCKET) {}

// Destructor
SocketManager::~SocketManager() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

// Initialize socket
bool SocketManager::initialize() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    return true;
}

// Accept client connection
SOCKET SocketManager::acceptClient() {
    return accept(serverSocket, nullptr, nullptr);
}

// Receive data
std::string SocketManager::receiveData(SOCKET clientSocket) {
    char buffer[8192] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        return "";
    }
    return std::string(buffer, bytesReceived);
}

// Send data
void SocketManager::sendData(SOCKET clientSocket, const std::string& data) {
    send(clientSocket, data.c_str(), data.size(), 0);
}

// Close socket
void SocketManager::closeSocket(SOCKET socket) {
    shutdown(socket, SD_SEND);
    closesocket(socket);
}
