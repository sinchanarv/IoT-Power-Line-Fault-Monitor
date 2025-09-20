# IoT Power Line Fault Monitor

**An IoT-based system using ESP32 to detect and predict faults in overhead power lines, featuring a local web dashboard and ThingSpeak cloud integration.**

---

## Table of contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware](#hardware)
- [Setup & Installation](#setup--installation)
  - [ThingSpeak setup](#thingspeak-setup)
  - [Flashing the ESP32](#flashing-the-esp32)
  - [Running the local dashboard](#running-the-local-dashboard)
- [Usage](#usage)
- [Data & Cloud Integration](#data--cloud-integration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

---

## Project overview
This project implements a prototype IoT system that reads electrical parameters from overhead power line sensors using an ESP32, displays the current status on a local web dashboard and pushes telemetry to ThingSpeak for remote monitoring and simple analytics.

## Features
- Real-time data acquisition from sensors (voltage/current/phase) using ESP32.
- Local web dashboard hosted on the ESP32 for immediate monitoring.
- Telemetry upload to ThingSpeak for cloud storage/visualization.
- Simple fault detection logic (threshold based) with alerts on the dashboard.

## Hardware
Minimum hardware list (use the exact parts you used in the project):
- ESP32 development board (e.g. ESP32-WROOM-32)
- Voltage sensing module(s) or potential transformer(s)
- Current sensor (e.g. SCT-013 series or ACS712)
- Level shifting / signal conditioning (as needed)
- Breadboard / jumper wires / power supply

Demo screenshots of the dashboard/UI.

[![](https://github.com/sinchanarv/IoT-Power-Line-Fault-Monitor/blob/main/Screenshot%202025-06-29%20132824.png)]()
[![](https://github.com/sinchanarv/IoT-Power-Line-Fault-Monitor/blob/main/Screenshot%202025-06-29%20132846.png)]()


> The repository is primarily implemented in C++ (Arduino).

**Example (abstract):**
- Voltage sensor output -> ESP32 analog input (with proper divider and isolation)
- Current sensor output -> ESP32 analog input
- ESP32 GND -> Common ground
- Power ESP32 with 5V/USB or regulated 3.3V as applicable

**Safety:** Do NOT connect the ESP32 directly to high-voltage lines. Use transformers, potential/voltage dividers, isolation, and follow electrical safety standards.

## Setup & Installation
1. Install Arduino IDE (or PlatformIO) and add ESP32 board support.
2. Open `iottesting.ino` in Arduino IDE.
3. Replace Wi‑Fi credentials and ThingSpeak API keys in the sketch (search for `ssid`, `password`, `thingspeak` or `API_KEY`).
4. Configure sensor calibration constants (e.g., ADC scaling, offsets).

### ThingSpeak setup
1. Create an account on ThingSpeak and make a new channel.
2. Note down the Write API Key and Channel ID.
3. Enter these values into `iottesting.ino` where the sketch uploads telemetry.

### Flashing the ESP32
1. Select the correct ESP32 board in Tools → Board.
2. Select the correct COM port.
3. Click Upload. Monitor the Serial Monitor (115200 baud) for boot messages.

### Running the local dashboard
- The sketch runs a small web server on the ESP32. After connecting the board to your Wi‑Fi network, find its IP address from the Serial Monitor and open that IP in a browser to view the dashboard.

## Usage
- Observe live sensor values on the local dashboard.
- The sketch uploads periodic telemetry to ThingSpeak (interval configurable).
- Fault detection thresholds trigger visual alerts on the dashboard. For advanced use, extend fault logic to record timestamps and send notifications.

## Data & Cloud Integration
- ThingSpeak stores the time series data and can plot fields. Use ThingSpeak's TimeSeries charts or export CSV for deeper analysis.

## Troubleshooting
- If the board doesn't connect to Wi‑Fi: check SSID/password, signal strength, and Serial Monitor for errors.
- If telemetry does not appear on ThingSpeak: verify API key, Channel ID, and network connectivity.
- If ADC readings are noisy: add RC filtering, average samples in software, and calibrate offsets.

## Contributing
Contributions are welcome!
- Open an issue describing the feature or bug.
- Fork the repo, create a branch (`feature/awesome`), make changes, and open a pull request.


---

