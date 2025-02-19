#include "ContentProcessor.h"
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstdlib> // For getenv

// Helper for handling CURL responses
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Fetch API key from environment variables
std::string ContentProcessor::getAPIKey() {
    const char* apiKey = std::getenv("GPT_API_KEY");
    if (!apiKey) {
        throw std::runtime_error("API key not found in environment variables");
    }
    return std::string(apiKey);
}

// Fetch data from GPT API
std::string ContentProcessor::fetchGPTAPI(const std::string& content) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        try {
            std::string apiKey = getAPIKey(); // Fetch API key securely
            std::string apiUrl = "https://api.openai.com/v1/completions";

            // Construct the JSON request payload
            nlohmann::json requestPayload = {
                {"model", "text-davinci-003"}, // Change to your desired GPT model
                {"prompt", content},
                {"max_tokens", 500},
                {"temperature", 0.7}
            };

            std::string requestBody = requestPayload.dump();

            // Set CURL options
            curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            // Perform the API call
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
                throw std::runtime_error("Failed to fetch data from GPT API");
            }

            curl_slist_free_all(headers);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            curl_easy_cleanup(curl);
            throw;
        }

        curl_easy_cleanup(curl);
    } else {
        throw std::runtime_error("Failed to initialize CURL");
    }

    return response;
}

// Extract keywords using GPT API
std::vector<std::string> ContentProcessor::extractKeywords(const std::string& content) {
    try {
        std::string apiResponse = fetchGPTAPI("Extract keywords from the following content:\n" + content);
        nlohmann::json jsonResponse = nlohmann::json::parse(apiResponse);

        // Extract keywords from JSON response (adjust based on GPT's response format)
        std::vector<std::string> keywords = {jsonResponse["choices"][0]["text"].get<std::string>()};
        return keywords;
    } catch (const std::exception& e) {
        std::cerr << "Error extracting keywords: " << e.what() << std::endl;
        return {"Error fetching keywords"};
    }
}

// Generate questions using GPT API
std::vector<std::string> ContentProcessor::generateQuestions(const std::string& content) {
    try {
        std::string apiResponse = fetchGPTAPI("Generate questions based on the following content:\n" + content);
        nlohmann::json jsonResponse = nlohmann::json::parse(apiResponse);

        // Extract questions from JSON response (adjust based on GPT's response format)
        std::vector<std::string> questions = {jsonResponse["choices"][0]["text"].get<std::string>()};
        return questions;
    } catch (const std::exception& e) {
        std::cerr << "Error generating questions: " << e.what() << std::endl;
        return {"Error fetching questions"};
    }
}

// Generate summary using GPT API
std::string ContentProcessor::generateSummary(const std::string& content) {
    try {
        std::string apiResponse = fetchGPTAPI("Generate a summary for the following content:\n" + content);
        nlohmann::json jsonResponse = nlohmann::json::parse(apiResponse);

        // Extract summary from JSON response (adjust based on GPT's response format)
        return jsonResponse["choices"][0]["text"].get<std::string>();
    } catch (const std::exception& e) {
        std::cerr << "Error generating summary: " << e.what() << std::endl;
        return "Error fetching summary";
    }
}
