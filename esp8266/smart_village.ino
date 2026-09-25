#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =========================
// Wi-Fi
// =========================
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// =========================
// Server URLs
// =========================
const char* SERVER_SAVE_URL =
  "http://192.168.1.100/smart_village/api/save_data.php";

const char* SERVER_CONTROL_URL =
  "http://192.168.1.100/smart_village/api/get_control.php";

// =========================
// Pin Configuration
// =========================
#define SOIL_PIN A0
#define RELAY_PIN D1

#define SDA_PIN D2
#define SCL_PIN D5

// =========================
// Soil Calibration
// =========================
#define RAW_DRY 780
#define RAW_WET 320

// =========================
// Irrigation Threshold
// =========================
#define MOISTURE_THRESHOLD 50

// =========================
// LCD
// =========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =========================
// Timing
// =========================
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;

unsigned long lastControlTime = 0;
const unsigned long controlInterval = 2000;

// =========================
// Control variables
// =========================
String currentMode = "AUTO";
bool manualPumpCommand = false;

// =====================================================
// SETUP
// =====================================================
void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF initially
  digitalWrite(RELAY_PIN, HIGH);

  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Village");

  lcd.setCursor(0, 1);
  lcd.print("WiFi Connecting");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  delay(2000);
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  // Read soil moisture sensor
  int rawValue = analogRead(SOIL_PIN);

  // Convert raw value to moisture percentage
  int moisture = map(
    rawValue,
    RAW_DRY,
    RAW_WET,
    0,
    100
  );

  moisture = constrain(moisture, 0, 100);

  // Get control command from server
  if (millis() - lastControlTime >= controlInterval) {

    lastControlTime = millis();

    getControlFromServer();
  }

  // =========================
  // AUTO MODE
  // =========================
  if (currentMode == "AUTO") {

    if (moisture < MOISTURE_THRESHOLD) {

      // Pump ON
      digitalWrite(RELAY_PIN, LOW);

    } else {

      // Pump OFF
      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  // =========================
  // MANUAL MODE
  // =========================
  else {

    if (manualPumpCommand) {

      // Pump ON
      digitalWrite(RELAY_PIN, LOW);

    } else {

      // Pump OFF
      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  // =========================
  // Determine Pump Status
  // =========================
  String pumpStatus;

  if (digitalRead(RELAY_PIN) == LOW) {
    pumpStatus = "ON";
  } else {
    pumpStatus = "OFF";
  }

  // =========================
  // Serial Monitor
  // =========================
  Serial.println("-------------------------");

  Serial.print("Mode: ");
  Serial.println(currentMode);

  Serial.print("Raw Value: ");
  Serial.println(rawValue);

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");

  Serial.print("Threshold: ");
  Serial.print(MOISTURE_THRESHOLD);
  Serial.println("%");

  Serial.print("Pump: ");
  Serial.println(pumpStatus);

  // =========================
  // LCD
  // =========================
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Moisture:");
  lcd.print(moisture);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Pump:");
  lcd.print(pumpStatus);

  // =========================
  // Send Sensor Data
  // =========================
  if (millis() - lastSendTime >= sendInterval) {

    lastSendTime = millis();

    sendDataToServer(moisture, pumpStatus);
  }

  delay(500);
}

// =====================================================
// GET CONTROL FROM PHP BACKEND
// =====================================================
void getControlFromServer() {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi Disconnected!");

    return;
  }

  WiFiClient client;
  HTTPClient http;

  Serial.println("Checking control...");

  http.begin(client, SERVER_CONTROL_URL);

  int httpResponseCode = http.GET();

  Serial.print("Control HTTP Code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {

    String response = http.getString();

    Serial.print("Control Response: ");
    Serial.println(response);

    // Check mode
    if (response.indexOf("\"mode\":\"MANUAL\"") >= 0) {

      currentMode = "MANUAL";

    } else if (response.indexOf("\"mode\":\"AUTO\"") >= 0) {

      currentMode = "AUTO";
    }

    // Check pump command
    if (response.indexOf("\"pump\":\"ON\"") >= 0) {

      manualPumpCommand = true;

    } else if (response.indexOf("\"pump\":\"OFF\"") >= 0) {

      manualPumpCommand = false;
    }
  }

  http.end();
}

// =====================================================
// SEND DATA TO PHP BACKEND
// =====================================================
void sendDataToServer(int moisture, String pumpStatus) {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi Disconnected!");

    return;
  }

  WiFiClient client;
  HTTPClient http;

  Serial.println("Sending data to server...");

  http.begin(client, SERVER_SAVE_URL);

  http.addHeader(
    "Content-Type",
    "application/x-www-form-urlencoded"
  );

  String postData =
    "moisture=" + String(moisture) +
    "&pump=" + pumpStatus;

  int httpResponseCode = http.POST(postData);

  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {

    String response = http.getString();

    Serial.print("Server Response: ");
    Serial.println(response);

  } else {

    Serial.print("Error sending data: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}