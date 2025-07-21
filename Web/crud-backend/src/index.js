import path from "path";
import { fileURLToPath } from "url";
import express from "express";
import cors from "cors";
import clientRoutes from "./routes/clientRoutes.js";
import tagclientRoutes from "./routes/tagsclientRoutes.js";
import warehouseRoutes from "./routes/warehouseRoutes.js";
import readerRoutes from "./routes/readerRoutes.js";
import userRoutes from "./routes/userRoutes.js";
import authRoutes from "./routes/authRoutes.js";
import scanRoutes from "./routes/scanRoutes.js";
import dotenv from "dotenv";
import { WebSocketServer } from "ws";
import http from "http";
import espRoutes from "./routes/espRoutes.js";

dotenv.config();

const app = express();
const port = 80;

app.use(cors());
app.use(express.json());

app.use("/api", clientRoutes);
app.use("/api", tagclientRoutes);
app.use("/api", warehouseRoutes);
app.use("/api", readerRoutes);
app.use("/api", userRoutes);
app.use("/api", authRoutes);
app.use("/api", scanRoutes);
app.use("/api", espRoutes);

app.get("/", (req, res) => {
  return res.send("Welcome to the CRUD API");
});

app.get("/api/esp/send-id", async (req, res) => {
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

// Create HTTP server from Express app
const server = http.createServer(app);

// Create WebSocket server on top of the HTTP server
const wss = new WebSocketServer({ server });

const clients = new Set();

wss.on("connection", (ws) => {
  console.log("✅ Client connected via WebSocket");
  clients.add(ws);

  ws.on("close", () => {
    console.log("❌ Client disconnected");
    clients.delete(ws);
  });
});

// Broadcast helper
export function broadcastNewScan(scanData) {
  console.log("📢 Broadcasting:", scanData);
  for (const client of clients) {
    if (client.readyState === 1) {
      client.send(JSON.stringify(scanData));
    }
  }
}

// Start the HTTP server (which also handles WS)
server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
