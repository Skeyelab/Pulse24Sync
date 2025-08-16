#include <WiFi.h>
#include <SSD1306.h>
#include <Wire.h>

// OLED Display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
SSD1306 display(0x3c, SDA, SCL);

// WiFi configuration
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
bool wifiConnected = false;
String networkLocation = "Unknown";

// Display settings
const int DISPLAY_REFRESH_INTERVAL = 1000; // milliseconds
unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED display
  Wire.begin();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  // Initialize WiFi
  initWiFi();
  
  // Initial display update
  updateDisplay();
}

void loop() {
  // Check WiFi connection status
  checkWiFiStatus();
  
  // Update display periodically
  if (millis() - lastDisplayUpdate > DISPLAY_REFRESH_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  delay(100);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    networkLocation = getNetworkLocation();
    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiConnected = false;
    Serial.println("");
    Serial.println("WiFi connection failed!");
  }
}

void checkWiFiStatus() {
  bool currentStatus = (WiFi.status() == WL_CONNECTED);
  
  if (currentStatus != wifiConnected) {
    wifiConnected = currentStatus;
    if (wifiConnected) {
      networkLocation = getNetworkLocation();
      Serial.println("WiFi reconnected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi disconnected!");
    }
  }
}

String getNetworkLocation() {
  // Simple network detection based on SSID or IP range
  String ssidName = WiFi.SSID();
  if (ssidName.indexOf("Home") >= 0 || ssidName.indexOf("home") >= 0) {
    return "Home";
  } else if (ssidName.indexOf("Work") >= 0 || ssidName.indexOf("work") >= 0 || ssidName.indexOf("Office") >= 0) {
    return "Work";
  } else {
    return WiFi.SSID(); // Return actual SSID if no pattern match
  }
}

void updateDisplay() {
  display.clear();
  
  // Display main content area (example)
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Device Status");
  display.drawString(0, 12, "System: Online");
  display.drawString(0, 24, "Temp: 24.5C");
  
  // Draw status bar at bottom
  drawStatusBar();
  
  display.display();
}

void drawStatusBar() {
  // Status bar starts at bottom of screen
  int statusBarY = SCREEN_HEIGHT - 24; // Leave space for 2 lines
  
  display.setFont(ArialMT_Plain_10);
  
  if (wifiConnected) {
    // Display IP address above network location
    String ipAddress = WiFi.localIP().toString();
    display.drawString(0, statusBarY, "IP: " + ipAddress);
    display.drawString(0, statusBarY + 12, networkLocation);
  } else {
    // Display disconnected status
    display.drawString(0, statusBarY + 6, "WiFi: Disconnected");
  }
  
  // Add other status indicators (battery, time, etc.)
  display.drawString(SCREEN_WIDTH - 30, statusBarY + 6, "100%"); // Battery example
}

void oledMsg(String message, int x, int y) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(x, y, message);
}

void oledMsg(String message, int x, int y, int fontSize) {
  switch(fontSize) {
    case 16:
      display.setFont(ArialMT_Plain_16);
      break;
    case 24:
      display.setFont(ArialMT_Plain_24);
      break;
    default:
      display.setFont(ArialMT_Plain_10);
      break;
  }
  display.drawString(x, y, message);
}