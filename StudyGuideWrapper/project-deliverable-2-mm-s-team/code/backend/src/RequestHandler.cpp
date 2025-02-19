#include "RequestHandler.h"
#include <nlohmann/json.hpp>
#include <iostream>

std::string RequestHandler::processRequest(const std::string& request) {
    const std::string corsHeaders =
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n";

    try {
        if (request.find("POST /upload") != std::string::npos) {
            // Validate and process the request
            if (request.empty()) {
                throw std::invalid_argument("Request body is empty");
            }

            ContentProcessor processor;

            std::string content = "Example content from request"; // Mock example
            auto keywords = processor.extractKeywords(content);
            auto questions = processor.generateQuestions(content);
            auto summary = processor.generateSummary(content);

            nlohmann::json jsonResponse = {
                {"status", "success"},
                {"message", "File uploaded successfully"},
                {"plan", keywords},
                {"questions", questions},
                {"summary", summary}
            };

            return "HTTP/1.1 200 OK\r\n" + corsHeaders +
                   "Content-Type: application/json\r\n\r\n" +
                   jsonResponse.dump();
        }

        return "HTTP/1.1 404 Not Found\r\n" + corsHeaders +
               "Content-Type: application/json\r\n\r\n"
               "{\"status\": \"error\", \"message\": \"Endpoint not found\"}";

    } catch (const std::invalid_argument& e) {
        nlohmann::json errorResponse = {
            {"status", "error"},
            {"message", e.what()}
        };

        return "HTTP/1.1 400 Bad Request\r\n" + corsHeaders +
               "Content-Type: application/json\r\n\r\n" +
               errorResponse.dump();

    } catch (const std::exception& e) {
        nlohmann::json errorResponse = {
            {"status", "error"},
            {"message", "Internal server error: " + std::string(e.what())}
        };

        return "HTTP/1.1 500 Internal Server Error\r\n" + corsHeaders +
               "Content-Type: application/json\r\n\r\n" +
               errorResponse.dump();
    }
}
