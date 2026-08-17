import { useState } from "react";
import "./App.css";

function App() {
  const [file, setFile] = useState(null);
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);

  const analyzeImage = async () => {
    if (!file) return;

    setLoading(true);

    try {
      const response = await fetch("http://localhost:8080/analyze", {
        method: "POST",
        body: `/app/test_images/${file.name}`,
      });

      const data = await response.json();
      console.log("API result:", data);
      setResult(data);
    } catch (error) {
      console.error("API error:", error);
    }

    setLoading(false);
  };

  return (
    <div className="app">
      <h1>Image File Intelligence</h1>

      <p>
        Analyze an image for classification and quality.
      </p>

      <input
        type="file"
        accept="image/*"
        onChange={(e) => setFile(e.target.files[0])}
      />

      <button
        onClick={analyzeImage}
        disabled={!file || loading}
      >
        {loading ? "Analyzing..." : "Analyze Image"}
      </button>

      {result && (
        <div className="results">
          <h2>Results</h2>

          <p>
            <strong>Image:</strong> {result.image}
          </p>

          <p>
            <strong>Predictions:</strong>
          </p>

          <ul>
            {result.predictions?.map((prediction, index) => (
              <li key={index}>
                {prediction.label} — {(prediction.confidence * 100).toFixed(1)}%
              </li>
            ))}
          </ul>

          <p>
            <strong>Quality:</strong> {result.quality}
          </p>
        </div>
      )}
    </div>
  );
}

export default App;