#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "BufferedTrack.h"

class AudioMixer {
public:
    void begin();
    void addTrack(const char* filename, float gain = 1.0f);
    bool isActive() const;
    size_t mix(int16_t* outBuffer, size_t numSamples);

private:
    struct Track {
        WavReader reader;
        float gain = 1.0f;
        bool active = false;
    };

    static const int MAX_TRACKS = 4;
    BufferedTrack tracks[MAX_TRACKS];
    int trackCount = 0;
};

#endif

