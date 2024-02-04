# Oracle APEX ESP32 Web ClientSecure Connector

This project is a library for securely connecting to Oracle APEX (Application Express) servers using an ESP32 microcontroller. This library provides an interface for communicating with APEX servers using the ESP32's WiFiClientSecure library.

## Features

- Supports GET and POST requests.
- Utilizes SSL/TLS certificates to secure connections.
- Sends and receives requests in JSON format.

## Usage

1. Add the `OracleApexConnector` class to your project.
2. Initialize the WiFi connection and configure any custom settings if necessary.
3. Create an instance of `OracleApexConnector` with a `WiFiClientSecure` instance.
4. Create and send requests to the server using the `sendRequest` function.

**Example:**

```cpp
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "OracleApexConnector.h"

const char* ssid = "your-ssid";
const char* password = "your-password";

const char* schema = "your-apex-schema";
const char* module = "your-apex-module";

WiFiClientSecure client;
OracleApexConnector apexClient(client, schema, module);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("WiFi connected");
}

void loop() {
    if (apexClient.checkWifi()) {
        // WiFi connection check
        DynamicJsonDocument responseDoc = apexClient.sendRequest("/your-apex-endpoint", NULL, "GET");
        // Response handling can be done here.
    } else {
        Serial.println("WiFi connection lost.");
    }

    delay(5000);
}
