import React from "react";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom"; // Importing routing components from React Router
import "./App.css"; // Importing the CSS file for styling
import FileUpload from "./components/FileUpload"; // Component for file upload functionality
import StudyPlan from "./components/StudyPlan"; // Component for displaying the study plan
import QuestionsDisplay from "./components/QuestionsDisplay"; // Component for displaying generated questions
import SummaryDisplay from "./components/SummaryDisplay"; // Component for displaying the summary

function App() {
  return (
    // Router provides the routing context for the application
    <Router>
      <div className="App">
        {/* Header contains the application title and description */}
        <header className="App-header">
          {/* Container for styling purposes */}
          <div className="box-container">
            {/* Application title */}
            <h1 className="title">Study Guide Wrapper</h1>
            {/* Brief description of the application's functionality */}
            <p className="description">
              Welcome to the Study Guide Wrapper! Upload your study materials,
              generate a personalized study plan, and ace your exams with ease.
            </p>
            {/* Routes define the navigation paths and associated components */}
            <Routes>
              {/* Default route for the File Upload page */}
              <Route path="/" element={<FileUpload />} />
              {/* Route for the Study Plan page */}
              <Route path="/study-plan" element={<StudyPlan />} />
              {/* Route for the Questions Display page */}
              <Route path="/questions" element={<QuestionsDisplay />} />
              {/* Route for the Summary Display page */}
              <Route path="/summary" element={<SummaryDisplay />} />
            </Routes>
          </div>
        </header>
      </div>
    </Router>
  );
}

export default App;
