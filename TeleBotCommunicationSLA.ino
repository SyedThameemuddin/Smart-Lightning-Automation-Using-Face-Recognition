#include <WiFi.h>
#include <WebServer.h>
#include <UniversalTelegramBot.h>

#define BOT_TOKEN "7500118477:AAG8R4YHWyMsQrKS0rjxlP0TOTDAzbkguS8"  // Your bot token
#define CHAT_ID "1644763236"                                       // Your chat ID

WebServer server(80);
WiFiClient wifiClient;  // Create a WiFiClient instance
UniversalTelegramBot bot(BOT_TOKEN, wifiClient);  // Pass the WiFiClient instance

bool faceDetectionEnabled = false;
bool irSensorEnabled = true;
bool ledState = false;
bool manualControl = false;

const int ledPin = 4;
const int irSensorPin = 14;
const int buzzerPin = 5;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  WiFi.begin("IDEA_WIFI_2", "ideawifi2");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/face", HTTP_GET, handleFaceDetectionControl);
  server.on("/ir", HTTP_GET, handleIrSensorControl);
  server.on("/led", HTTP_GET, handleLedControl);
  server.on("/status", HTTP_GET, handleStatusCheck);
  server.begin();
}

void loop() {
  server.handleClient();
  checkTelegramMessages();  // Check for incoming Telegram messages
  updateLedState();
}

void checkTelegramMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    Serial.println("New message received");
    handleTelegramCommand(bot.messages[0].text);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}

void handleTelegramCommand(String command) {
  if (command == "/start") {
    bot.sendMessage(CHAT_ID, "Welcome to Smart Lighting Automation! Use /help to see commands.", "");
  } else if (command == "/help") {
    bot.sendMessage(CHAT_ID, "Available commands:\n"
                              "/face_on - Enable Face Detection\n"
                              "/face_off - Disable Face Detection\n"
                              "/ir_on - Enable Motion Sensor\n"
                              "/ir_off - Disable Motion Sensor\n"
                              "/led_on - Turn LED On\n"
                              "/led_off - Turn LED Off\n"
                              "/status - Check Status", "");
  } else if (command == "/face_on") {
    faceDetectionEnabled = true;
    bot.sendMessage(CHAT_ID, "Face Detection Enabled", "");
  } else if (command == "/face_off") {
    faceDetectionEnabled = false;
    bot.sendMessage(CHAT_ID, "Face Detection Disabled", "");
  } else if (command == "/ir_on") {
    irSensorEnabled = true;
    bot.sendMessage(CHAT_ID, "Motion Sensor Enabled", "");
  } else if (command == "/ir_off") {
    irSensorEnabled = false;
    bot.sendMessage(CHAT_ID, "Motion Sensor Disabled", "");
  } else if (command == "/led_on") {
    ledState = true;
    manualControl = true;
    updateLedState();
    bot.sendMessage(CHAT_ID, "LED Turned On", "");
  } else if (command == "/led_off") {
    ledState = false;
    manualControl = true;
    updateLedState();
    bot.sendMessage(CHAT_ID, "LED Turned Off", "");
  } else if (command == "/status") {
    String status = "Face Detection: " + String(faceDetectionEnabled ? "Enabled" : "Disabled") +
                    "\nMotion Sensor: " + String(irSensorEnabled ? "Enabled" : "Disabled") +
                    "\nLED: " + String(ledState ? "On" : "Off");
    bot.sendMessage(CHAT_ID, status, "");
  }
}

void updateLedState() {
  bool irSensorState = digitalRead(irSensorPin);
  
  if (manualControl) {
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  } else if (faceDetectionEnabled) {
    // Here you would integrate your face detection logic
    digitalWrite(ledPin, HIGH);  // Example: turn LED on if face detection is enabled
  } else if (irSensorEnabled) {
    digitalWrite(ledPin, irSensorState == LOW ? HIGH : LOW);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Smart Lighting Automation</title>";
  html += "<style>body { font-family: Arial; background: #f0f0f0; }</style></head><body>";
  html += "<h1>Smart Lighting Automation</h1>";
  html += "<a href=\"/face?enable=1\">Enable Face Detection</a><br>";
  html += "<a href=\"/face?enable=0\">Disable Face Detection</a><br>";
  html += "<a href=\"/ir?enable=1\">Enable Motion Sensor</a><br>";
  html += "<a href=\"/ir?enable=0\">Disable Motion Sensor</a><br>";
  html += "<a href=\"/led?state=1\">Turn LED On</a><br>";
  html += "<a href=\"/led?state=0\">Turn LED Off</a><br>";
  html += "<a href=\"/status\">Check Status</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleFaceDetectionControl() {
  if (server.hasArg("enable")) {
    faceDetectionEnabled = server.arg("enable") == "1";
  }
  server.send(200, "text/plain", faceDetectionEnabled ? "Face Detection Enabled" : "Face Detection Disabled");
}

void handleIrSensorControl() {
  if (server.hasArg("enable")) {
    irSensorEnabled = server.arg("enable") == "1";
  }
  server.send(200, "text/plain", irSensorEnabled ? "Motion Sensor Enabled" : "Motion Sensor Disabled");
}

void handleLedControl() {
  if (server.hasArg("state")) {
    ledState = server.arg("state") == "1";
    manualControl = true;
    updateLedState();
  }
  server.send(200, "text/plain", ledState ? "LED On" : "LED Off");
}

void handleStatusCheck() {
  String status = "Face Detection is " + String(faceDetectionEnabled ? "Enabled" : "Disabled") + "\n";
  status += "Motion Sensor is " + String(irSensorEnabled ? "Enabled" : "Disabled");
  server.send(200, "text/plain", status);
}


