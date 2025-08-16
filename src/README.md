# Embedded Device with WiFi IP Display

This project implements an embedded device with WiFi connectivity and OLED display that shows the device's IP address when connected to a network.

## Features

- WiFi connectivity with automatic reconnection
- OLED display with status information
- **IP address display**: Shows the device's IP address above the network location when WiFi is connected
- Network location detection (Home/Work based on SSID)
- Real-time status updates

## Hardware Requirements

- ESP32 or ESP8266 microcontroller
- SSD1306 OLED display (128x64 pixels)
- I2C connection for display (SDA/SCL pins)

## WiFi IP Display Feature (ERI-13)

When connected to WiFi, the device displays:

```
[Main content area]
...
IP: 192.168.1.100    <- IP address (new feature)
Home                 <- Network location
[Battery: 100%]
```

### Implementation Details

- IP address is obtained using `WiFi.localIP().toString()`
- Only displayed when `wifiConnected` is true
- Updates automatically when WiFi connection status changes
- Uses ArialMT_Plain_10 font for consistent display
- Positioned above the network location in the status bar

### Code Structure

- `drawStatusBar()`: Main function that renders the status bar including IP address
- `oledMsg()`: Utility function for displaying messages on OLED
- `checkWiFiStatus()`: Monitors WiFi connection changes
- `getNetworkLocation()`: Determines network location based on SSID

## Configuration

Edit the following variables in `main.cpp`:

```cpp
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
```

## Display Layout

The OLED display (128x64 pixels) is organized as:
- Top area (0-40px): Main content
- Status bar (40-64px): WiFi IP, network location, battery status

## Acceptance Criteria ✅

- [x] IP address is displayed when WiFi is connected
- [x] IP address appears above the network location  
- [x] Display updates when WiFi connection status changes
- [x] No visual conflicts with existing UI elements
- [x] IP address is clearly readable using standard font size