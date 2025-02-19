import React from "react";
import { useLocation, useNavigate } from "react-router-dom";

const StudyPlan = () => {
  // useLocation to retrieve state passed from previous page
  const location = useLocation();
  // useNavigate for navigation between pages
  const navigate = useNavigate();

  // Extract data passed via navigation or use default values if none are available
  const data = location.state?.data || {
    plan: [], // Default to empty plan if no data provided
    questions: [], // Default to empty questions if no data provided
    summary: "No data available.", // Default message for summary if none provided
  };

  // Navigate to the Questions page, passing the questions as state
  const handleNavigateToQuestions = () => {
    navigate("/questions", { state: { questions: data.questions } });
  };

  // Navigate to the Summary page, passing the summary as state
  const handleNavigateToSummary = () => {
    navigate("/summary", { state: { summary: data.summary } });
  };

  // Navigate back to the File Upload page
  const handleBackToUpload = () => {
    navigate("/");
  };

  return (
    <div style={{ textAlign: "center", padding: "20px" }}>
      {/* Header for the page */}
      <h1 style={{ color: "#61dafb", marginBottom: "20px" }}>
        Your Study Plan
      </h1>

      {/* Display the study plan or fallback message */}
      <div style={{ marginBottom: "20px" }}>
        <h2>Plan:</h2>
        {data.plan.length > 0 ? (
          <ul>
            {data.plan.map((item, index) => (
              <li key={index}>{item}</li>
            ))}
          </ul>
        ) : (
          <p>No study plan available. Please upload a file.</p>
        )}
      </div>

      {/* Buttons for navigation */}
      <div>
        {/* Navigate to the Questions page */}
        <button onClick={handleNavigateToQuestions} style={buttonStyle}>
          View Questions
        </button>

        {/* Navigate to the Summary page */}
        <button onClick={handleNavigateToSummary} style={buttonStyle}>
          View Summary
        </button>

        {/* Navigate back to the File Upload page */}
        <button onClick={handleBackToUpload} style={buttonStyle}>
          Back to Upload
        </button>
      </div>
    </div>
  );
};

// Inline style for buttons
const buttonStyle = {
  padding: "10px 20px",
  fontSize: "1rem",
  color: "#fff",
  backgroundColor: "#61dafb",
  border: "none",
  borderRadius: "5px",
  cursor: "pointer",
  margin: "10px",
};

export default StudyPlan;
