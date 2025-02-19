import React, { useState } from "react";
import { useNavigate } from "react-router-dom";

// FileUpload Component
const FileUpload = () => {
  // State to store the selected files
  const [files, setFiles] = useState([]);
  // State to display messages related to the upload process
  const [uploadMessage, setUploadMessage] = useState("");
  // useNavigate hook for navigation to another page
  const navigate = useNavigate();

  // Handle file selection
  const handleFileChange = (e) => {
    // Convert the FileList object to an array and update state
    setFiles(Array.from(e.target.files));
    // Reset the upload message when new files are selected
    setUploadMessage("");
  };

  // Handle file upload
  const handleUpload = async () => {
    // Check if files are selected
    if (files.length === 0) {
      setUploadMessage("Please select at least one file before uploading.");
      return;
    }

    // Create FormData object to send files via HTTP request
    const formData = new FormData();
    files.forEach((file) => {
      formData.append("files", file);
    });

    try {
      // Set upload status message
      setUploadMessage("Uploading files...");

      // Send files to the backend
      const response = await fetch("http://localhost:5000/upload", {
        method: "POST",
        body: formData,
      });

      // Handle successful response
      if (response.ok) {
        const result = await response.json();
        // Navigate to the Study Plan page and pass the uploaded data
        navigate("/study-plan", { state: { data: result } });
      } else {
        // Handle errors returned from the server
        const errorResult = await response.json();
        setUploadMessage(errorResult.message || "Failed to upload files.");
      }
    } catch (error) {
      // Log and display error in case of a network or processing issue
      console.error("Error uploading files:", error);
      setUploadMessage("An error occurred while uploading the files.");
    }
  };

  return (
    <div style={{ textAlign: "center" }}>
      {/* Input for file selection */}
      <input
        type="file"
        onChange={handleFileChange}
        multiple // Allow multiple file selection
        accept=".pdf, .jpg, .jpeg, .png, .txt, .docx, .xlsx" // Allowed file types
        style={{
          margin: "10px 0",
          padding: "5px",
        }}
      />
      <br />
      {/* Button to trigger file upload */}
      <button
        onClick={handleUpload}
        style={{
          padding: "10px 20px",
          fontSize: "1rem",
          color: "#fff",
          backgroundColor: "#61dafb",
          border: "none",
          borderRadius: "5px",
          cursor: "pointer",
        }}
      >
        Upload
      </button>
      <div style={{ marginTop: "20px" }}>
        {/* Display upload message */}
        {uploadMessage && (
          <p
            style={{
              marginTop: "10px",
              color: uploadMessage.includes("success") ? "green" : "red",
            }}
          >
            {uploadMessage}
          </p>
        )}
      </div>
    </div>
  );
};

export default FileUpload;
