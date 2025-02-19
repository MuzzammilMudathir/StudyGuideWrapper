import React from "react";
import { useLocation, useNavigate } from "react-router-dom";

const QuestionsDisplay = () => {
  // useLocation to retrieve state passed from previous page
  const location = useLocation();
  // useNavigate for navigation between pages
  const navigate = useNavigate();
  // Extract questions from the state passed via navigation, or default to an empty array
  const questions = location.state?.questions || [];

  // Navigate back to the file upload page
  const handleBackToUpload = () => {
    navigate("/");
  };

  return (
    <div style={{ textAlign: "center", padding: "20px" }}>
      {/* Header for the page */}
      <h1 style={{ color: "#61dafb" }}>Generated Questions</h1>
      {/* Display questions if available, otherwise show a fallback message */}
      {questions.length > 0 ? (
        <ul style={{ listStyle: "none", padding: "0" }}>
          {questions.map((question, index) => (
            <li key={index} style={listItemStyle}>
              {question}
            </li>
          ))}
        </ul>
      ) : (
        // Fallback message if no questions are available
        <p>No questions available. Please upload a file.</p>
      )}
      {/* Button to navigate back to the upload page */}
      <button onClick={handleBackToUpload} style={buttonStyle}>
        Back to Upload
      </button>
    </div>
  );
};

// Inline style for each list item (question)
const listItemStyle = {
  marginBottom: "10px",
  padding: "10px",
  backgroundColor: "#f5f5f5",
  color: "#333",
  borderRadius: "5px",
  textAlign: "left",
  maxWidth: "300px",
  margin: "auto",
};

// Inline style for the button
const buttonStyle = {
  padding: "10px 20px",
  fontSize: "1rem",
  color: "#fff",
  backgroundColor: "#61dafb",
  border: "none",
  borderRadius: "5px",
  cursor: "pointer",
  margin: "20px",
};

export default QuestionsDisplay;
