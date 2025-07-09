**IoT-Based Predictive Fault Detection System for Power Lines**
An end-to-end IoT system built with an ESP32 to monitor, detect, and predict faults in simulated three-phase overhead power lines. The project features a real-time local web dashboard for on-site monitoring and cloud integration with ThingSpeak for remote data logging and visualization.

Table of Contents
Features
System Architecture
Hardware Components
Software and Libraries
Implementation Details
How It Works
Results
Setup and Installation
Future Work

Features
Real-Time Monitoring: Continuously monitors three-phase voltage levels.
Local Web Dashboard: Hosts a web server on the ESP32 to display live data and system status on any device on the same network.
Cloud Integration: Pushes sensor data to the ThingSpeak cloud platform for historical logging and live graphical visualization.
Fault Detection: Implements logic to detect critical faults like phase-to-phase shorts and open phase conditions.
Predictive Analysis (ML Model): A simulated on-device model analyzes voltage patterns to issue pre-fault warnings for conditions like phase instability or systemic voltage decay.
Local Alerts: An onboard buzzer provides immediate audible alerts upon fault detection.
System Architecture
The system is designed with a centralized sensing unit that performs data acquisition, processing, and communication.

Sensing Unit: An ESP32 microcontroller interfaces with a resistive voltage divider network to read analog signals representing the R, Y, and B phases.
Local Interface: The ESP32 hosts an asynchronous web server, providing a dashboard accessible via its IP address.
Cloud Platform: The ESP32 sends formatted HTTP requests to the ThingSpeak API to log data, which can then be visualized and analyzed remotely.
Hardware Components
Microcontroller: ESP32 DevKitC V4
Voltage Sensing: Custom Resistive Voltage Divider Network (3x)
Local Alarm: 5V Piezo Buzzer
Power Supply: 5V via USB / External Power Adapter

Software and Libraries
Development Environment: Arduino IDE
Language: C++
Core Libraries:
WiFi.h: For network connectivity.
ESPAsyncWebServer.h: For the local web dashboard.
HTTPClient.h: For sending data to the ThingSpeak API.
Implementation Details
Fault Detection
The system detects faults by comparing live voltage readings against pre-calibrated thresholds:
Short Circuit: Triggered when the voltage of two or more phases drops below a critical level (e.g., < 0.2V).
Open Phase: Triggered when a phase's voltage reading is unexpectedly high (e.g., > 3.0V), indicating a potential floating input.
Predictive "ML" Model
To demonstrate predictive capabilities without the need for extensive training data, a simulated model was implemented. It analyzes a moving window of the last 5 voltage readings to detect:
Phase Instability: Flags a warning if the variance (max - min value) of any phase exceeds a threshold within the history window.
Systemic Voltage Drop: Flags a warning if all three phases show a consistent downward trend over several consecutive readings.
Results
The system was successfully tested, demonstrating full end-to-end functionality.
Local Dashboard
The web interface correctly displays live data and updates system status immediately upon a fault event.

Cloud Platform (ThingSpeak)
The device successfully transmitted data to the cloud, creating a live, historical log of all monitored parameters. Fault events are clearly visible on the graphs.

Setup and Installation
Hardware: Assemble the circuit as shown in the hardware image. Ensure sensor inputs are connected to ADC1 pins (GPIO 32, 33, 34 are used in this code).
Software:
Open the .ino sketch file in the Arduino IDE.
Install the required libraries: ESPAsyncWebServer and its AsyncTCP dependency.
Update the following credentials in the code:
Generated cpp
**// --- WiFi Credentials ---**
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

**// --- ThingSpeak Credentials ---**
const char* thingspeak_api_key = "YOUR_THINGSPEAK_WRITE_API_KEY";
Use code with caution.
C++
ThingSpeak: Create a new channel on ThingSpeak and configure it with at least 5 fields (e.g., R_Voltage, Y_Voltage, B_Voltage, Fault_Code, Prediction_Code).
Upload: Select your ESP32 board, the correct COM port, and upload the sketch.
Monitor: Open the Serial Monitor to see the ESP32's IP address, then navigate to that IP in your web browser.
Future Work
True ML Model: Collect data from ThingSpeak to train a proper neural network or Random Forest model for more accurate predictions.
Current Sensing: Integrate current sensors (e.g., SCT-013-000) to distinguish between overloads and short circuits.
Hardware Finalization: Design a custom PCB and a weatherproof enclosure for real-world deployment.
Power Optimization: Implement deep-sleep cycles to enable long-term battery or solar-powered operation.
![WhatsApp Image 2025-06-15 at 21 12 53_c1c33266](https://github.com/user-attachments/assets/ba464a24-fe39-499d-bc56-3b5c15a1f8e1)
