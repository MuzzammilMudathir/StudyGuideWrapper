#include "QuestionGenerator.h" // Include the header file for the QuestionGenerator class

// Method to generate questions based on the given content
std::vector<std::string> QuestionGenerator::generateQuestions(const std::string& content) {
    // Generate two example questions using substrings from the provided content
    return {
        // The first question is based on the first 10 characters of the content
        "What is the meaning of " + content.substr(0, 10) + "?",

        // The second question is based on the next 10 characters of the content
        "Explain the significance of " + content.substr(10, 20) + "."
    };
}
