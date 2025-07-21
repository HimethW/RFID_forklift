import { useState } from "react";
import "../client.css";

export default function ManualAddModal({ onSubmit, onClose }) {
  const [tagId, setTagId] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault();
    onSubmit(tagId);
    setTagId("");
  };

  return (
    <div className="modal-overlay">
      <div className="modal-content">
        <h2>Manually Add RFID Tag</h2>
        <form onSubmit={handleSubmit}>
          <input
            type="text"
            value={tagId}
            onChange={(e) => setTagId(e.target.value)}
            placeholder="Enter Tag ID"
            required
          />
          <button type="submit">Submit</button>
        </form>
        <button className="close-btn" onClick={onClose}>
          Cancel
        </button>
      </div>
    </div>
  );
}
