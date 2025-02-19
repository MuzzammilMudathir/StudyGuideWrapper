# Study Guide Wrapper

A comprehensive web-based application designed to assist users in uploading study materials, generating personalized study plans, creating practice questions, and producing concise summaries. Built with a backend powered by C++ and a React-based frontend, the Study Guide Wrapper is perfect for students and educators alike.

---

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Setup Instructions](#setup-instructions)
- [Dependencies](#dependencies)
- [Usage](#usage)
- [Testing](#usage)
---

## Description

The Study Guide Wrapper simplifies the learning process by enabling users to upload study material and receive a tailored plan, curated questions, and summaries. It leverages cutting-edge API integration to ensure accurate content analysis and an optimized study experience.

---

## Features

- **File Upload**: Supports various file formats like `.pdf`, `.docx`, `.txt`, etc.
- **Personalized Study Plan**: Generate daily study goals based on uploaded content.
- **Practice Questions**: Automated question generation to reinforce learning.
- **Concise Summaries**: Extract and display key takeaways from uploaded content.
- **Modern UI**: Intuitive and clean interface using React.
- **Secure API Integration**: Implements GPT APIs for advanced content analysis.

---

## Setup Instructions

### Prerequisites

- **Node.js**: Install [Node.js](https://nodejs.org/) for running the frontend.
- **C++ Compiler**: Ensure you have a C++ compiler such as GCC or MSVC.
- **Internet Connection**: Required for API integration.

### Installation Steps

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd StudyGuideWrapper
   ```

2. **Backend Setup**:
   - Navigate to the backend directory:
     ```bash
     cd project-deliverable-2-mm-s-team/code/backend/src
     ```
   - Compile the backend C++ code:
     ```bash
     g++ -o server Server.cpp ContentProcessor.cpp -lws2_32
     ```
   - Run the backend server:
     ```bash
     ./server
     ```

3. **Frontend Setup**:
   - Navigate to the frontend directory:
     ```bash
     cd project-deliverable-2-mm-s-team/code/frontend/study-guide-wrapper
     ```
   - Install dependencies:
     ```bash
     npm install
     ```
   - Start the frontend:
     ```bash
     npm start
     ```

4. Access the application in your browser at `http://localhost:3000`.

---

## Dependencies

### Backend
- **C++**: Base programming language.
- **nlohmann/json**: For JSON parsing.
- **WinSock2**: Windows socket programming for HTTP server functionality.

### Frontend
- **React.js**: For creating the user interface.
- **react-router-dom**: For navigation between components.

### External
- **OpenAI GPT API**: Used for advanced content analysis and study plan generation.

---

## Usage

1. Upload your study materials via the "Upload" button.
2. View your personalized study plan, practice questions, and summaries on the results page.
3. Download or print your study plan as needed.

---

## Testing

- Test the backend server by uploading a file via `curl`:
  ```bash
  curl -X POST http://localhost:5000/upload -F "files=@example.pdf"
  ```
- Test the frontend by navigating to `http://localhost:3000` and uploading study material.

---


## Contributors

- **Muzzammil Mudathir**: Developer, Designer, and Contributor.
