#pragma once

#include <Arduino.h>

class NetworkClient {
public:
    void connectWifi(const char* ssid, const char* password);
    bool sendAudio(const uint8_t* data, size_t len);
    size_t fetchAudio(uint8_t* buffer, size_t maxLen);
};
