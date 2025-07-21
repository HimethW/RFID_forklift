import { query } from "../db.js";
import { broadcastNewScan } from "../index.js";
import express from "express";

const router = express.Router();

router.post("/esp/send-id", async (req, res) => {
  const { id } = req.body;

  if (!id) {
    return res.status(400).json({ error: "ID is required" });
  }

  const scanData = {
    tag_id: id,
    timestamp: new Date().toISOString(),
  };

  try {
    await query("INSERT INTO scans (tag_id, timestamp) VALUES ($1, NOW())", [
      id,
    ]);
    console.log("✅ Scan saved to DB from ESP:", scanData);

    broadcastNewScan(scanData);

    res.status(200).json({ message: "Scan saved and broadcasted", scanData });
  } catch (err) {
    console.error("❌ Error inserting scan from ESP:", err);
    res.status(500).json({ error: "Failed to save scan" });
  }
});

export default router;
