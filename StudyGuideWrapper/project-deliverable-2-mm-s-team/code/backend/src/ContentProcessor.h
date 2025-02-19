#ifndef CONTENT_PROCESSOR_H
#define CONTENT_PROCESSOR_H

#include <string>
#include <vector>

class ContentProcessor {
public:
    // Extract keywords from content
    std::vector<std::string> extractKeywords(const std::string& content);

    // Generate questions from content
    std::vector<std::string> generateQuestions(const std::string& content);

    // Generate a summary from content
    std::string generateSummary(const std::string& content);

private:
    // Utility function to split content into sentences
    std::vector<std::string> splitIntoSentences(const std::string& content);
};

#endif // CONTENT_PROCESSOR_H
