//
// Created by Ata Can Yaymacı on 4.02.2024.
//

#include "OracleApexConnector.h"

OracleApexConnector::OracleApexConnector(WiFiClientSecure &client, String schema, String module) {
    this->client = &client;
    this->schema = schema;
    this->module = module;
    Serial.println("[Oracle APEX] - Connector created");
}

String OracleApexConnector::createURL(String command) {
    String url = "/pls/apex/" + this->schema + "/" + this->module + "/" + command;
    return url;
}

void OracleApexConnector::addParameter(String &url, String param, String value) {
    if(url.indexOf("?") == -1) {
        url += "?";
    } else {
        url += "&";
    }
    url += param + "=" + value;
}

void OracleApexConnector::addPath(String &url, String path) {
    url += "/" + path;
}

DynamicJsonDocument OracleApexConnector::sendRequest(const char* url, DynamicJsonDocument payload, const char* method) {
    DynamicJsonDocument doc(1024);
    if (strcmp(method, "GET") == 0) {
        Serial.print(F("[Oracle APEX] - Sending GET request to "));
        Serial.println(url);
        String response = sendGet(url);
        deserializeJson(doc, response);
        return doc;
    } else if (strcmp(method, "POST") == 0) {
        Serial.print(F("[Oracle APEX] - Sending POST request to "));
        Serial.println(url);
        String response = sendPost(url, payload);
        deserializeJson(doc, response);
        return doc;
    }
    return doc;
}

String OracleApexConnector::sendGet(const char* url) {
    String body, headers;

    if (!connectApexClient()) {
        return "";
    }

    if (client->connected()) {
        client->print(F("GET /"));
        client->print(url);
        client->println(F(" HTTP/1.1"));
        client->println(F("Host:" APEX_HOST));
        client->println(F("Accept: application/json"));
        client->println(F("Cache-Control: no-cache"));
        client->println();

        readHTTP(body, headers);
    }

    return body;
}

String OracleApexConnector::sendPost(const char* url, DynamicJsonDocument payload) {
    String body, headers, out;
    serializeJson(payload, out);

    if (!connectApexClient()) {
        return "";
    }

    if (client->connected()) {
        client->print(F("POST /"));
        client->print(url);
        client->println(F(" HTTP/1.1"));
        client->println(F("Host:" APEX_HOST));
        client->println(F("Accept: application/json"));

        int length = out.length();
        client->print(F("Content-Length:"));
        client->println(length);

        if(payload.isNull()){
            client->println();
            client->println(out);
        }

        readHTTP(body, headers);
    }

    return body;
}

bool OracleApexConnector::readHTTP(String &body, String &headers) {
    int ch_count = 0;
    unsigned long now = millis();
    bool finishedHeaders = false;
    bool currentLineIsBlank = true;
    bool responseReceived = false;

    while (millis() - now < maxResponseTime) {
        while (client->available()) {
            char c = client->read();
            responseReceived = true;

            if (!finishedHeaders) {
                if (currentLineIsBlank && c == '\n') {
                    finishedHeaders = true;
                } else {
                    headers += c;
                }
            } else {
                if (ch_count < maxResponseLength) {
                    body += c;
                    ch_count++;
                }
            }

            if (c == '\n') currentLineIsBlank = true;
            else if (c != '\r') currentLineIsBlank = false;
        }

        if (responseReceived) {
            Serial.println(F("[Oracle APEX] - Response received"));
            Serial.println(body);
            break;
        }
    }
    return responseReceived;
}

bool OracleApexConnector::connectApexClient() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("[Oracle APEX] - No WiFi connection"));
        return false;
    }

    client->setCACert(APEX_ORACLE_CERTIFICATE);

    if (!client->connect(APEX_HOST, APEX_PORT)) {
        Serial.println(F("[Oracle APEX] - Connection error"));
        return false;
    }

    return true;
}

void OracleApexConnector::closeApexClient() {
    if(client->connected()) {
        client->stop();
    }
}

bool OracleApexConnector::checkWifi() {
    return WiFi.status() == WL_CONNECTED;
}

// Created by Ata Can Yaymacı on 4.02.2024.