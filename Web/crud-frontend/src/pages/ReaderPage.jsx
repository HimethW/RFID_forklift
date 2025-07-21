import { useEffect, useState } from "react";
import RFIDPopup from "../components/RFIDPopup";
import ManualAddModal from "../components/ManualAddModal";
import axios from "axios";
import "../client.css";

export default function ReaderPage() {
  const [scans, setScans] = useState([]);
  const [lastTag, setLastTag] = useState(null);
  const [isPopupOpen, setIsPopupOpen] = useState(false);
  const [isManualOpen, setIsManualOpen] = useState(false);

  // Helper to add scan uniquely by tag_id at front
  function addScanUnique(scans, newScan) {
    const filtered = scans.filter((s) => s.tag_id !== newScan.tag_id);
    return [newScan, ...filtered];
  }

  // Initial load and polling every 5 seconds
  useEffect(() => {
    const fetchScans = async () => {
      try {
        const res = await axios.get("http://localhost:3000/api/scans");
        setScans(res.data);
      } catch (error) {
        console.error("Error fetching scans:", error);
      }
    };

    fetchScans(); // initial fetch

    const interval = setInterval(fetchScans, 5000); // poll every 5 seconds

    return () => clearInterval(interval); // cleanup on unmount
  }, []);

  // Handle manual add
  const handleManualSubmit = async (tagId) => {
    try {
      await axios.post("http://localhost:3000/api/esp/send-id", { id: tagId });
      setIsManualOpen(false);
      setLastTag(tagId);
      setIsPopupOpen(true);

      const newScan = {
        tag_id: tagId,
        timestamp: new Date().toISOString(),
      };

      setScans((prev) => addScanUnique(prev, newScan));
    } catch (error) {
      console.error("Error submitting manually:", error);
    }
  };

  return (
    <div className="reader-page">
      <h1>RFID Reader Screen</h1>
      <p>Waiting for scan...</p>
      <button onClick={() => setIsManualOpen(true)}>
        Manually Add RFID Tag
      </button>

      {isManualOpen && (
        <ManualAddModal
          onSubmit={handleManualSubmit}
          onClose={() => setIsManualOpen(false)}
        />
      )}

      {isPopupOpen && (
        <RFIDPopup tagId={lastTag} onClose={() => setIsPopupOpen(false)} />
      )}

      {/* Scans table */}
      <table
        border="1"
        cellPadding="8"
        style={{ marginTop: "20px", width: "100%", borderCollapse: "collapse" }}
      >
        <thead>
          <tr>
            <th>Tag ID</th>
            <th>Timestamp</th>
          </tr>
        </thead>
        <tbody>
          {scans.map((scan, index) => (
            <tr key={index}>
              <td>{scan.tag_id}</td>
              <td>
                {scan.timestamp
                  ? new Date(scan.timestamp).toLocaleString()
                  : "N/A"}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
