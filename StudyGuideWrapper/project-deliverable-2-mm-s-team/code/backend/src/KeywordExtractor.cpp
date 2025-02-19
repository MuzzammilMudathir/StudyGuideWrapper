#include "Server.h"
#include "StudyPlanGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <curl/curl.h> // For making API requests

// API Key and Endpoint
const std::string API_KEY = "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"; // Replace with your actual API key
const std::string API_ENDPOINT = "https://api.openai.com/v1/keywords"; // Replace with the correct API endpoint

// Constructor
Server::Server(int port) : port(port), serverSocket(INVALID_SOCKET) {}

// Destructor
Server::~Server() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

// Run server
void Server::run() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server running on port " << port << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client connection failed: " << WSAGetLastError() << std::endl;
            continue;
        }
        std::thread(&Server::handleClient, this, clientSocket).detach();
    }
}

// Handle client connection
void Server::handleClient(SOCKET clientSocket) {
    std::cout << "Handling new client connection." << std::endl;

    char buffer[8192] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive data or client disconnected." << std::endl;
        closesocket(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';
    std::cout << "Request received:\n" << buffer << std::endl;

    std::string response = processRequest(buffer);

    send(clientSocket, response.c_str(), response.size(), 0);

    shutdown(clientSocket, SD_SEND);
    closesocket(clientSocket);
    std::cout << "Client connection closed." << std::endl;
}

// Process request
std::string Server::processRequest(const std::string& request) {
    const std::string corsHeaders =
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n";

    if (request.find("POST /upload") != std::string::npos) {
        auto files = parseMultipart(request);
        if (files.empty()) {
            return "HTTP/1.1 400 Bad Request\r\n" + corsHeaders +
                   "Content-Type: application/json\r\n\r\n"
                   "{\"status\": \"error\", \"message\": \"No files uploaded\"}";
        }

        std::string combinedContent;
        for (const auto& file : files) {
            combinedContent += file;
        }

        // Make API request
        std::string apiResponse = callApiForKeywords(combinedContent);

        // Construct JSON response
        std::ostringstream jsonResponse;
        jsonResponse << apiResponse;

        return "HTTP/1.1 200 OK\r\n" + corsHeaders +
               "Content-Type: application/json\r\n\r\n" + jsonResponse.str();
    }

    return "HTTP/1.1 404 Not Found\r\n" + corsHeaders +
           "Content-Type: application/json\r\n\r\n"
           "{\"status\": \"error\", \"message\": \"Endpoint not found\"}";
}

// Helper function to call the external API
std::string Server::callApiForKeywords(const std::string& content) {
    CURL* curl;
    CURLcode res;
    std::string apiResponse;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string postData = "{\"text\": \"" + content + "\"}";

        curl_easy_setopt(curl, CURLOPT_URL, API_ENDPOINT.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiResponse);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "API request failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return apiResponse.empty() ? "{\"status\": \"error\", \"message\": \"API call failed\"}" : apiResponse;
}

std::vector<std::string> Server::parseMultipart(const std::string& request) {
    std::vector<std::string> files;

    // Find the boundary from the Content-Type header
    std::string boundary;
    size_t boundaryPos = request.find("boundary=");
    if (boundaryPos != std::string::npos) {
        boundary = "--" + request.substr(boundaryPos + 9);
    } else {
        std::cerr << "Boundary not found in request." << std::endl;
        return files;
    }

    // Find the start of the multipart content
    size_t startPos = 0;
    while ((startPos = request.find(boundary, startPos)) != std::string::npos) {
        size_t endPos = request.find(boundary, startPos + boundary.length());
        if (endPos == std::string::npos) {
            break; // No more parts
        }

        // Locate the start of the file content
        size_t fileStart = request.find("\r\n\r\n", startPos) + 4;
        if (fileStart == std::string::npos || fileStart >= endPos) {
            startPos = endPos; // Skip invalid part
            continue;
        }

        // Extract the file content (excluding trailing \r\n)
        std::string fileContent = request.substr(fileStart, endPos - fileStart - 2);
        files.push_back(fileContent);

        // Move to the next part
        startPos = endPos;
    }

    if (files.empty()) {
        std::cerr << "No files parsed from multipart request." << std::endl;
    } else {
        std::cout << "Extracted " << files.size() << " file(s) from multipart request." << std::endl;
    }

    return files;
}
