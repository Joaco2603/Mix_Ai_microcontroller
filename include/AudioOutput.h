#pragma once

#include <Arduino.h>

class AudioOutput {
public:
    void begin();
    void play(uint8_t* buffer, size_t len);
};
