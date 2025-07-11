# 🚜 Forklift-Mounted RFID Reader System

A robust, industrial-grade RFID-based asset tracking system designed for real-time inventory management in warehouse environments. This project integrates embedded hardware and full-stack web technologies to automate and optimize warehouse logistics using a forklift-mounted RFID reader.

---

## 📌 Project Overview

This system enables forklifts to detect RFID-tagged goods during pallet movement and automatically update inventory data via Wi-Fi to a centralized web dashboard. It improves warehouse efficiency by minimizing manual data entry and ensuring accurate, real-time inventory tracking.

---

## 🔧 Key Features

- 📡 **RFID Tag Detection**: High-frequency (13.56 MHz) RFID tag scanning using PN532.
- 🧠 **Embedded Controller**: ATmega328P-based control for RFID and communication tasks.
- 🌐 **Wi-Fi Integration**: ESP8266 transmits data wirelessly to backend server.
- 🖥️ **Web Dashboard**: Real-time inventory tracking and management via React frontend and Express + PostgreSQL backend.
- 🔩 **Custom Enclosure & PCB**: Vibration-resistant, forklift-compatible hardware with modular design and test-point access.

---
## 🧠 System Architecture

```text
[RFID Tag]
    ↓
[PN532 Reader] ←→ [ATmega328P MCU] ←→ [ESP8266 WiFi Module] → [Web Server]
                                                       ↓
                                               [PostgreSQL Database]
                                                       ↓
                                               [React Web Dashboard]



---
