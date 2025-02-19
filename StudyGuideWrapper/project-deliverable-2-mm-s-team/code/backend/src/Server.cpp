#include "Server.h"
#include <iostream>
#include <thread>

// Constructor
Server::Server(int port) : port(port), socketManager(port), requestHandler() {}

// Destructor
Server::~Server() {}

// Run server
void Server::run() {
    if (!socketManager.initialize()) {
        std::cerr << "Failed to initialize socket manager" << std::endl;
        return;
    }

    std::cout << "Server running on port " << port << std::endl;

    while (true) {
        SOCKET clientSocket = socketManager.acceptClient();
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }
        std::thread(&Server::handleClient, this, clientSocket).detach();
    }
}

// Handle client connection
void Server::handleClient(SOCKET clientSocket) {
    std::cout << "Handling new client connection." << std::endl;

    std::string request = socketManager.receiveData(clientSocket);
    if (request.empty()) {
        std::cerr << "Failed to receive data or client disconnected." << std::endl;
        socketManager.closeSocket(clientSocket);
        return;
    }

    std::string response = requestHandler.processRequest(request);

    socketManager.sendData(clientSocket, response);
    socketManager.closeSocket(clientSocket);

    std::cout << "Client connection closed." << std::endl;
}
