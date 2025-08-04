#ifndef BUFFERED_TRACK_H
#define BUFFERED_TRACK_H

#include "WavReader.h"

class BufferedTrack {
public:
    bool open(const char* path, float gain = 1.0f);
    size_t getSamples(int16_t* buffer, size_t numSamples);
    bool isActive() const;
    void refill();  // recargar buffer si hace falta
    float gain = 1.0f;

private:
    WavReader reader;
    static const size_t BUFFER_SIZE = 2048;
    int16_t internalBuffer[BUFFER_SIZE];
    size_t bufferIndex = 0;
    size_t bufferFill = 0;
    bool active = false;
};

#endif
