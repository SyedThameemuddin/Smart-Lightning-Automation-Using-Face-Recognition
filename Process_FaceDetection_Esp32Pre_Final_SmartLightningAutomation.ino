#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "IDEA_WIFI_2";
const char* password = "ideawifi2";

WebServer server(80);

bool faceDetectionEnabled = false;
bool irSensorEnabled = true;
bool ledState = false;
bool manualControl = false;
bool bothEnabled = false;

const int ledPin = 4;
const int irSensorPin = 14;
const int buzzerPin = 5;  // Pin for the buzzer

bool lastLedState = false;

void handleRoot();
void handleFaceDetectionControl();
void handleIrSensorControl();
void handleLedControl();
void handleBothControl();
void handleStatusCheck();

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);  // Set the buzzer pin as output

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/face", HTTP_GET, handleFaceDetectionControl);
  server.on("/ir", HTTP_GET, handleIrSensorControl);
  server.on("/led", HTTP_GET, handleLedControl);
  server.on("/both", HTTP_GET, handleBothControl);
  server.on("/status", HTTP_GET, handleStatusCheck);

  server.begin();
}

void loop() {
  server.handleClient();

  bool irSensorState = digitalRead(irSensorPin);
  bool faceDetected = faceDetectionEnabled ? getFaceDetectionStatus() : false;

  if (manualControl) {
    updateLedState(ledState);
  } else if (bothEnabled) {
    if (irSensorState == LOW || faceDetected) {
      updateLedState(true);
    } else {
      updateLedState(false);
    }
  } else if (faceDetectionEnabled) {
    updateLedState(faceDetected);
  } else if (irSensorEnabled) {
    updateLedState(irSensorState == LOW);
  }
}

bool getFaceDetectionStatus() {
  // Simulate face detection status for demo purposes
  // You should replace this with actual status retrieval logic
  return false; // Replace with actual status retrieval from the Python script
}

void updateLedState(bool state) {
  if (state && !lastLedState) {
    // LED just turned on, make the buzzer beep for one second
    digitalWrite(buzzerPin, HIGH);
    delay(200);  // Beep for one second
    digitalWrite(buzzerPin, LOW);
  }
  digitalWrite(ledPin, state ? HIGH : LOW);
  lastLedState = state;  // Update the last LED state
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Smart Lighting Automation</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background: linear-gradient(45deg, #e3f2fd, #90caf9); height: 100vh; display: flex; justify-content: center; align-items: center; margin: 0; }";
  html += ".container { text-align: center; }";
  html += "h1 { color: #ffffff; font-size: 48px; }";
  html += ".button { display: inline-block; padding: 15px 25px; font-size: 20px; color: white; background-color: #4CAF50; text-decoration: none; border-radius: 10px; margin: 10px; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>Smart Lighting Automation</h1>";
  html += "<a href=\"/face?enable=1\" class='button'>Enable Face Detection</a>";
  html += "<a href=\"/face?enable=0\" class='button'>Disable Face Detection</a>";
  html += "<a href=\"/ir?enable=1\" class='button'>Enable Motion Sensor</a>";
  html += "<a href=\"/ir?enable=0\" class='button'>Disable Motion Sensor</a>";
  html += "<a href=\"/led?state=1\" class='button'>Turn LED On</a>";
  html += "<a href=\"/led?state=0\" class='button'>Turn LED Off</a>";
  html += "<a href=\"/both?enable=1\" class='button'>Enable Both Sensors</a>";
  html += "<a href=\"/both?enable=0\" class='button'>Disable Both Sensors</a>";
  html += "<a href=\"/status\" class='button'>Check Status</a>";
  html += "</div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleFaceDetectionControl() {
  if (server.hasArg("enable")) {
    faceDetectionEnabled = server.arg("enable") == "1";
  }
  bothEnabled = false;
  manualControl = false;
  server.send(200, "text/plain", faceDetectionEnabled ? "Face Detection Enabled" : "Face Detection Disabled");
}

void handleIrSensorControl() {
  if (server.hasArg("enable")) {
    irSensorEnabled = server.arg("enable") == "1";
  }
  bothEnabled = false;
  manualControl = false;
  server.send(200, "text/plain", irSensorEnabled ? "Motion Sensor Enabled" : "Motion Sensor Disabled");
}

void handleLedControl() {
  if (server.hasArg("state")) {
    ledState = server.arg("state") == "1";
    manualControl = true;
    updateLedState(ledState);
  }
  server.send(200, "text/plain", ledState ? "LED On" : "LED Off");
}

void handleBothControl() {
  if (server.hasArg("enable")) {
    bothEnabled = server.arg("enable") == "1";
    faceDetectionEnabled = bothEnabled;
    irSensorEnabled = bothEnabled;
  }
  manualControl = false;
  server.send(200, "text/plain", bothEnabled ? "Both Sensors Enabled" : "Both Sensors Disabled");
}

void handleStatusCheck() {
  String status = "Face Detection is ";
  status += faceDetectionEnabled ? "Enabled" : "Disabled";
  status += "\nMotion Sensor is ";
  status += irSensorEnabled ? "Enabled" : "Disabled";
  server.send(200, "text/plain", status);
}




