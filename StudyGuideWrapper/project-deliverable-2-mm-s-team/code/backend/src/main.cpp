#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp" // Include the JSON library
#include <thread>

#pragma comment(lib, "Ws2_32.lib") // Link the Winsock library

using json = nlohmann::json;

#define PORT 5000

std::string handleFileUpload(const std::string& body) {
    // Save the file content (optional, for debugging)
    std::ofstream file("uploaded_file.txt");
    file << body;
    file.close();

    // Construct the JSON response
    json response = {
        {"status", "success"},
        {"message", "File uploaded successfully"},
        {"plan", {"Day 1: Read Chapter 1", "Day 2: Solve Exercises"}},
        {"questions", {"What is X?", "Define Y"}},
        {"summary", "A brief overview of the study materials."}
    };

    return response.dump(); // Serialize JSON to a string
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return;
    }

    std::string request(buffer, bytesReceived);
    std::string response;

    if (request.find("POST /upload") != std::string::npos) {
        std::string body = request.substr(request.find("\r\n\r\n") + 4); // Extract the body
        response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + handleFileUpload(body);
    } else if (request.find("GET /studyPlan") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + handleGenerateStudyPlan();
    } else {
        json errorResponse = {{"status", "error"}, {"message", "Endpoint not found"}};
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\n\r\n" + errorResponse.dump();
    }

    send(clientSocket, response.c_str(), response.size(), 0);
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create the socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Configure server address structure
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
    serverAddr.sin_port = htons(PORT);       // Convert port to network byte order

    // Bind the socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Socket binding failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        std::cout << "Binding successful on port " << PORT << std::endl;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        std::cout << "Server is now listening on port " << PORT << std::endl;
    }

    // Server loop to accept and handle clients
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client connection failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;
        std::thread(handleClient, clientSocket).detach(); // Handle client in a new thread
    }

    // Cleanup resources
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}