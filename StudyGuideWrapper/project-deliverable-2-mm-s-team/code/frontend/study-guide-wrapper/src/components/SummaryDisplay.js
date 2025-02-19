import React from "react";
import { useLocation, useNavigate } from "react-router-dom";

const SummaryDisplay = () => {
  // useLocation is used to retrieve the state passed from the previous page
  const location = useLocation();
  // useNavigate is used for navigation between pages
  const navigate = useNavigate();

  // Extract the summary from the state or default to "No summary available"
  const summary = location.state?.summary || "No summary available.";

  // Function to navigate back to the File Upload page
  const handleBackToUpload = () => {
    navigate("/"); // Redirects to the File Upload page
  };

  return (
    <div style={{ textAlign: "center", padding: "20px" }}>
      {/* Display the page title */}
      <h1 style={{ color: "#61dafb" }}>Summary</h1>

      {/* Display the summary content */}
      <p style={summaryStyle}>{summary}</p>

      {/* Button to navigate back to the File Upload page */}
      <button onClick={handleBackToUpload} style={buttonStyle}>
        Back to Upload
      </button>
    </div>
  );
};

// Inline style for the summary display
const summaryStyle = {
  padding: "10px",
  backgroundColor: "#f5f5f5",
  color: "#333",
  borderRadius: "5px",
  textAlign: "left",
  maxWidth: "300px",
  margin: "auto", // Centers the element horizontally
};

// Inline style for the button
const buttonStyle = {
  padding: "10px 20px",
  fontSize: "1rem",
  color: "#fff",
  backgroundColor: "#61dafb",
  border: "none",
  borderRadius: "5px",
  cursor: "pointer", // Pointer cursor to indicate clickability
  margin: "20px", // Adds spacing around the button
};

export default SummaryDisplay;
