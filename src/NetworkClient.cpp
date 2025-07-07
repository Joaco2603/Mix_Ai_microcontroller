#include "NetworkClient.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "Config.h"

void NetworkClient::connectWifi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
}

bool NetworkClient::sendAudio(const uint8_t* data, size_t len) {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin(SERVER_UPLOAD_URL);
    http.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = http.POST(data, len);
    http.end();

    return httpResponseCode == 200;
}

size_t NetworkClient::fetchAudio(uint8_t* buffer, size_t maxLen) {
    if (WiFi.status() != WL_CONNECTED) return 0;

    HTTPClient http;
    http.begin(SERVER_DOWNLOAD_URL);
    int code = http.GET();
    if (code != HTTP_CODE_OK) return 0;

    WiFiClient stream = http.getStream();
    size_t i = 0;
    while (stream.available() && i < maxLen) {
        buffer[i++] = stream.read();
    }

    http.end();
    return i;
}
