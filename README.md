# ESP32 Wi-Fi Network Scanner & LED Web Server

A self-hosted local web server built using native C++ on the **ESP32** architecture. The application initializes a Wi-Fi connection in Station (`WIFI_STA`) mode, boots up an HTTP server on port 80, and renders an interactive control page.

## Features
*   **Wi-Fi Connection Status:** Hooks directly into local access points using hardcoded network configurations and checks status systematically.
*   **Dynamic Wi-Fi Scanner:** Discovers visible networks locally via `WiFi.scanNetworks()` and prints an HTML ordered list.
*   **Signal Strength Sorting:** Implements an optimized internal bubble sort algorithm to rank networks chronologically from the strongest signal (`RSSI`) to the weakest.
*   **Hardware Control Endpoint:** Exposes a text-input configuration terminal (`/control`) that converts raw browser text arguments into hardware status flags (`HIGH`/`LOW`) to control the onboard diagnostic LED.

---

## Code Infrastructure Overview

*   **`setup()`**: Configures serial baud communication (115200), pins diagnostic channels (`ledPin` as output), authenticates network handshake flags, and registers URI handlers.
*   **`handleRoot()`**: Renders the baseline web layout with standalone CSS styling grids and functional interface panels.
*   **`handleScan()`**: Iterates through locally found SSIDs, extracts numeric signal values, re-orders them by strength, and dynamically builds an interactive list response.
*   **`handleControl()`**: Normalizes string entries to lowercase data format, sets physical output parameters, and outputs an HTTP `303 Redirect` back to the monitoring view.
*   **`loop()`**: Continuously updates `server.handleClient()` to handle asynchronous incoming client connection strings.

---

## Getting Started

### Hardware Prerequisites
*   **ESP32 Development Board** (or ESP8266 equivalent running appropriate layout pins)
*   **Onboard LED / External LED** tied directly to GPIO 2

### Software Requirements
*   **Arduino IDE** (or VS Code with PlatformIO extension installed)
*   **ESP32 Board Library Core** installed via Board Manager

### Pin Configuration
By default, the script flashes to the built-in diagnostic pin:
*   `ledPin = 2` (Standard onboard blue LED on most ESP32 dev blocks)

### Installation & Deployment
1. Copy the code into your `.ino` sketch file.
2. Replace your network parameter constants at the top of the file if deploying to a custom router network:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and flash the payload code to your target development board.
4. Keep the microchip plugged into your machine and boot up your **Serial Monitor** at a `115200` baud rate to check the designated local IP routing address.
5. Paste the generated IP address into your local machine's web browser to toggle your endpoints!
