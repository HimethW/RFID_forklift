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


```

## ⚙️ Technologies Used

### 🖥️ Hardware
- ATmega328P (QFP)
- PN532 RFID Reader
- ESP8266-12F Wi-Fi Module
- LM2596 Buck Converter
- Custom PCBs (EasyEDA)
- SolidWorks Enclosure Design

### 💻 Software
- Embedded C (SPI & UART protocols)
- React + Vite (Frontend)
- Express.js (Backend)
- PostgreSQL (Database)
- Node.js

---

## 🚀 Functional Workflow

1. RFID reader (PN532) detects and reads nearby tags.
2. ATmega328P processes the UID and sends it to ESP8266 via UART.
3. ESP8266 transmits tag data to a remote PostgreSQL database.
4. Web app updates inventory and shipment statuses in real time.

---




## 🧪 Field Testing


- Verified tag detection accuracy, Wi-Fi stability, and user workflows  
- Refined enclosure and PCB based on field challenges (vibrations, range, alignment)  

---

## 👨‍💻 Contributors

  
- Udugamasooriya P.H.J.  
- Perera A.L.C.K.  
- Aazir M.A.M.  
- Ranaweera R.P.D.D.H.
- De Alwis W.M.R. 
- Walgampaya H.K.B.  
- Vidmal S.S.A.A.  
- Rathnayake R.M.T.N.B.  

---

## 📃 License

This project is licensed under the [MIT License](LICENSE).  

---

