//
// Created by Ata Can Yaymacı on 4.02.2024.
//

#ifndef LOKI_V1_0_ORACLEAPEXCONNECTOR_H
#define LOKI_V1_0_ORACLEAPEXCONNECTOR_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include "ApexOracleCertificate.h"

#define APEX_HOST "apex.oracle.com"
#define APEX_PORT 443

class OracleApexConnector {
public:
    WiFiClientSecure *client;
    String schema;
    String module;

    OracleApexConnector(WiFiClientSecure &client, String schema, String module);
    String createURL(String command);
    void addParameter(String &url, String param, String value);
    void addPath(String &url, String path);
    DynamicJsonDocument sendRequest(const char* url, DynamicJsonDocument payload, const char* method);
private:
    int maxResponseLength = 100000000;
    int maxResponseTime = 5000;

    String sendGet(const char* url);
    String sendPost(const char* url, DynamicJsonDocument payload);
    bool readHTTP(String &body, String &headers);
    bool connectApexClient();
    void closeApexClient();
    bool checkWifi();
};


#endif //LOKI_V1_0_ORACLEAPEXCONNECTOR_H
