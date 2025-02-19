#include "StudyPlanGenerator.h"
#include "KeywordExtractor.h"
#include "QuestionGenerator.h"

std::vector<std::string> StudyPlanGenerator::generatePlan(const std::string& content) {
    KeywordExtractor extractor;
    QuestionGenerator questionGen;

    // Extract keywords and questions
    auto keywords = extractor.extractKeywords(content);
    auto questions = questionGen.generateQuestions(content);

    // Build a structured study plan
    std::vector<std::string> studyPlan;
    studyPlan.push_back("Day 1: Focus on the following keywords:");
    for (size_t i = 0; i < keywords.size(); ++i) {
        studyPlan.push_back("- " + keywords[i]);
        if ((i + 1) % 5 == 0 && i < keywords.size() - 1) {
            studyPlan.push_back("Day " + std::to_string((i / 5) + 2) + ": Continue with the next set of keywords:");
        }
    }

    studyPlan.push_back("Review and practice with these questions:");
    for (const auto& question : questions) {
        studyPlan.push_back("- " + question);
    }

    return studyPlan;
}
