#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HardwareSerial.h>

// --- WiFi Credentials ---
const char* ssid = "Airtel_dhan_6847";         // <<<<<---- REPLACE
const char* password = "Air@12557"; // <<<<<---- REPLACE

// --- GSM Setup (Kept for now, but likely not functional yet) ---
HardwareSerial mySerial(1); // UART1
const char* phoneNumber = "+919591515045";

// --- Sensor Pin Definitions (MUST BE ADC1 PINS) ---
#define R_PIN 32 // Example: SVP - ADC1_CH0
#define Y_PIN 33 // Example: SVN - ADC1_CH3
#define B_PIN 34 // Example: ADC1_CH6
// #define R_PIN 12 // OLD ADC2 - WILL NOT WORK RELIABLY WITH WIFI
// #define Y_PIN 13 // OLD ADC2
// #define B_PIN 14 // OLD ADC2

#define BUZZER 4

// --- Global Variables for Web Interface & Logic ---
float rVolt = 0.0;
float yVolt = 0.0;
float bVolt = 0.0;
String faultStatus = "Normal";
bool buzzerState = false;

unsigned long lastSensorReadTime = 0;
const long sensorReadInterval = 1000; // Read sensors every 1 second

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// --- Function Prototypes ---
void sendSMS(const char* number, const String& text);
void sendATCommand(const char* cmd);
void readAndProcessSensors();
String generateHtmlPage();

void setup() {
  Serial.begin(115200);
  // mySerial.begin(115200, SERIAL_8N1, 16, 17); // Initialize GSM Serial
                                               // Consider moving GSM init to only when SMS needs to be sent
                                               // and add proper boot time for it.

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  // analogReadResolution(12); // Default for ESP32

  // --- Initialize Sensor Pins (ADC1) ---
  pinMode(R_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(B_PIN, INPUT);

  // --- Connect to Wi-Fi ---
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // --- Define Web Server Routes ---
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", generateHtmlPage());
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    // This endpoint provides data in JSON format for dynamic updates (more advanced UI)
    String json = "{";
    json += "\"rVolt\":" + String(rVolt, 2) + ",";
    json += "\"yVolt\":" + String(yVolt, 2) + ",";
    json += "\"bVolt\":" + String(bVolt, 2) + ",";
    json += "\"faultStatus\":\"" + faultStatus + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");

  // Initial sensor read
  readAndProcessSensors();
}

void loop() {
  // Periodically read sensors and update status
  if (millis() - lastSensorReadTime > sensorReadInterval) {
    readAndProcessSensors();
    lastSensorReadTime = millis();
  }

  // Handle Buzzer (non-blocking)
  if (buzzerState) {
    // You might want a more sophisticated buzzer pattern here
    // For now, it will be solid ON if faultStatus is not "Normal"
    // and turned OFF by readAndProcessSensors if condition clears or after SMS
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  // Note: ESPAsyncWebServer handles client requests asynchronously.
  // Avoid long delays in the loop() function.
}

void readAndProcessSensors() {
  int rVal = analogRead(R_PIN);
  int yVal = analogRead(Y_PIN);
  int bVal = analogRead(B_PIN);

  rVolt = rVal * 3.3 / 4095.0;
  yVolt = yVal * 3.3 / 4095.0;
  bVolt = bVal * 3.3 / 4095.0;

  Serial.print("R: "); Serial.print(rVolt, 2); Serial.print(" V, ");
  Serial.print("Y: "); Serial.print(yVolt, 2); Serial.print(" V, ");
  Serial.print("B: "); Serial.print(bVolt, 2); Serial.println(" V");

  // --- Fault Detection Logic (modified slightly for web status) ---
  String currentFault = "Normal"; // Assume normal initially for this cycle
  bool faultDetectedThisCycle = false;

  if (rVolt < 1.9 && yVolt < 1.9) {
    currentFault = "RY Short Detected";
    faultDetectedThisCycle = true;
  } else if (yVolt < 1.9 && bVolt < 1.9) { // Use else if to avoid multiple fault reports at once
    currentFault = "YB Short Detected";
    faultDetectedThisCycle = true;
  } else if (rVolt < 1.9 && bVolt < 1.9) {
    currentFault = "RB Short Detected";
    faultDetectedThisCycle = true;
  } else if (rVolt > 2.8) { // Consider if this "open" logic is correct for your sensors
    currentFault = "R Phase Open";
    faultDetectedThisCycle = true;
  } else if (yVolt > 2.8) {
    currentFault = "Y Phase Open";
    faultDetectedThisCycle = true;
  } else if (bVolt > 2.8) {
    currentFault = "B Phase Open";
    faultDetectedThisCycle = true;
  }

  // Update global fault status and handle alerts
  if (faultDetectedThisCycle) {
    if (faultStatus == "Normal" || faultStatus != currentFault) { // New fault or different fault
      faultStatus = currentFault;
      Serial.println("⚠ " + faultStatus);
      buzzerState = true; // Turn buzzer flag on
      // sendSMS(phoneNumber, faultStatus); // SMS sending attempt
      // For now, buzzer will stay on until fault clears or for a short duration
      // You might want to turn buzzerState off after a short period or after SMS sent
    }
  } else {
    if (faultStatus != "Normal") {
      Serial.println("Fault condition cleared. Status: Normal");
    }
    faultStatus = "Normal";
    buzzerState = false; // Turn buzzer flag off
  }
}

String generateHtmlPage() {
  String html = "<!DOCTYPE HTML><html><head>";
  html += "<title>ESP32 Phase Monitor</title>";
  html += "<meta http-equiv='refresh' content='3'>"; // Auto-refresh page every 3 seconds
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f0f0; text-align: center;}";
  html += "h1 { color: #333; }";
  html += ".sensor-data { background-color: #fff; border: 1px solid #ddd; border-radius: 8px; padding: 15px; margin: 20px auto; width: 80%; max-width: 500px; box-shadow: 0 2px 4px rgba(0,0,0,0.1);}";
  html += "p { font-size: 1.2em; color: #555; margin: 8px 0;}";
  html += ".fault { color: red; font-weight: bold; font-size: 1.5em; }";
  html += ".normal { color: green; font-weight: bold; font-size: 1.5em; }";
  html += "</style></head><body>";
  html += "<h1>ESP32 Three-Phase Monitor</h1>";
  html += "<div class='sensor-data'>";
  html += "<h2>Live Readings</h2>";
  html += "<p>R Phase: " + String(rVolt, 2) + " V</p>";
  html += "<p>Y Phase: " + String(yVolt, 2) + " V</p>";
  html += "<p>B Phase: " + String(bVolt, 2) + " V</p>";
  html += "<h2>Status</h2>";
  if (faultStatus == "Normal") {
    html += "<p class='normal'>" + faultStatus + "</p>";
  } else {
    html += "<p class='fault'>" + faultStatus + "</p>";
  }
  html += "</div></body></html>";
  return html;
}


// --- GSM Functions (Kept for now) ---
void sendSMS(const char* number, const String& text) {
  // It's better to initialize and de-initialize mySerial here if only used for SMS
  // and ensure proper boot time for GSM module if it's powered down between uses.
  Serial.println("Attempting to send SMS: " + text);
  // mySerial.begin(115200, SERIAL_8N1, 16, 17); // If not already begun or re-initializing
  // delay(6000); // Give module time if just powered/initialized

  sendATCommand("AT+CMGF=1"); // Set SMS to text mode
  delay(500); // Shorter delay if module is already responsive

  String command = "AT+CMGS=\"";
  command += number;
  command += "\"";

  sendATCommand(command.c_str());
  delay(500);

  mySerial.print(text); // Send the SMS text
  delay(100);

  mySerial.write(26); // Send CTRL+Z
  delay(1000); // Wait for SMS to send

  // Read response
  unsigned long startTime = millis();
  while(millis() - startTime < 3000 && !mySerial.available()) { delay(10); } // Wait up to 3s for response
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  Serial.println(); // Newline after SMS attempt
  // mySerial.end(); // If you want to free up UART pins / power down module
}

void sendATCommand(const char* cmd) {
  mySerial.println(cmd);
  // Wait for specific response (e.g., "OK" or ">") instead of fixed delay is more robust
  unsigned long startTime = millis();
  while(millis() - startTime < 2000 && !mySerial.available()) { delay(10); } // Wait up to 2s
  
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}