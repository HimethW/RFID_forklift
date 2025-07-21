import express from "express";
import { addNewScan, getAllScans } from "../controllers/scanControllers.js";

const router = express.Router();

router.post("/scans", addNewScan);
router.get("/scans", getAllScans);

export default router;
