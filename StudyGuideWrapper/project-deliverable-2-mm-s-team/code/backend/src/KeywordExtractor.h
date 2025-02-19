#ifndef KEYWORDEXTRACTOR_H
#define KEYWORDEXTRACTOR_H

#include <vector>
#include <string>

class KeywordExtractor {
public:
    std::vector<std::string> extractKeywords(const std::string& content);
};

#endif
