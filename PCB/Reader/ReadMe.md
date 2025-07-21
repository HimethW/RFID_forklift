# PN532 RFID PCB Design

This repository contains the complete PCB design files for an NFC RFID system based on the PN532 chip. The design includes schematic capture, PCB layout, and component libraries.

## Repository Contents

### Design Files
- **Schematic Files**:
  - Antenna circuit
  - Reader chip (PN532) interface
  - Power supply circuitry
  - Hierarchy view (top-level schematic)
  
- **PCB Layout Files**:
  - Complete board layout
  - Layer stackup
  - Manufacturing outputs

### Library Files
- `*.SchLib`: Schematic symbol libraries
- `*.PcbLib`: PCB footprint libraries

### Project Management
- `PN532_final.PrjPcb`: Altium Designer project file (or equivalent for your EDA tool)
- `pn532_LCSC_components.xlsx`: Bill of Materials (BOM) with:
  - All components used
  - Specifications
  - Reference designators
  - Supplier information

## Design Overview

The PCB implements a complete RFID reader solution using NXP's PN532 chip. Key features include:

- Operating frequency: 13.56 MHz
- Communication interfaces: [SPI, UART]
- Power supply: [5V] → [3.3V]
- Antenna design optimized for [Forklift Applications]

## Getting Started

### Prerequisites
- [Altium]

### Opening the Project
1. Clone this repository
2. Open `PN532_final.PrjPcb` in your EDA tool
3. All schematic and layout files should load automatically

## Version History
- v1.0 - Initial release
  - Basic functionality verified
  - Initial PCB prototype tested


