cmake_minimum_required(VERSION 3.10)
project(StudyGuideWrapper)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(StudyGuideWrapper
    src/main.cpp
    src/ContentProcessor.cpp
    src/StudyPlanGenerator.cpp
    src/KeywordExtractor.cpp
    src/QuestionGenerator.cpp
    src/Server.cpp
)
