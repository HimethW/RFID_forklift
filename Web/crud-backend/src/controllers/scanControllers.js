import { query } from "../db.js";
import { broadcastNewScan } from "../index.js";

export async function addNewScan(req, res) {
  const { tag_id } = req.body;

  if (!tag_id) {
    return res.status(400).json({ message: "Missing tag_id" });
  }

  try {
    await query("INSERT INTO scans (tag_id, timestamp) VALUES ($1, NOW())", [
      tag_id,
    ]);

    const newScan = { tag_id, timestamp: new Date() };

    broadcastNewScan(newScan);

    res.json({ message: "Scan saved and broadcasted" });
  } catch (err) {
    console.error("❌ Error saving scan:", err);
    res.status(500).json({ message: "Server error" });
  }
}


export async function getAllScans(req, res) {
  try {
    // Select all scans ordered by newest first
    const result = await query(
      "SELECT tag_id, timestamp FROM scans ORDER BY timestamp DESC"
    );

    res.json(result.rows);
  } catch (err) {
    console.error("❌ Error fetching scans:", err);
    res.status(500).json({ message: "Server error" });
  }
}
