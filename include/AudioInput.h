#pragma once

#include <Arduino.h>

class AudioInput {
public:
    void begin();
    size_t read(uint8_t* buffer, size_t len);
};
