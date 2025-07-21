import "../client.css";

export default function RFIDPopup({ tagId, onClose }) {
  return (
    <div className="popup-overlay">
      <div className="popup-content">
        <img
          className="tick-image"
          src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/50/Yes_Check_Circle.svg/2048px-Yes_Check_Circle.svg.png"
          alt="Tick"
        />
        <h2>Tag Scanned!</h2>
        <p>
          RFID Tag ID: <strong>{tagId}</strong>
        </p>
        <button onClick={onClose}>Close</button>
      </div>
    </div>
  );
}
