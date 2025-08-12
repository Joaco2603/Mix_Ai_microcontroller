#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "BufferedTrack.h"

class AudioMixer
{
public:
    void begin();
    void addTrack(const char *filename, float gain = 1.0f);
    bool isActive() const;
    size_t mix(int16_t *outBuffer, size_t numSamples);

    void setChannelGain(int channel, float gain);
    float getChannelGain(int channel) const;
    bool allTracksFinished() const;

    struct alignas(4) Channel
    {
        const char *name = nullptr;
        float gain = 1.0f;
        BufferedTrack track;
        bool isActive = true;
    };

    static const int MAX_CHANNELS = 4;
    Channel channels[MAX_CHANNELS];
    int channelCount = 0;
};

#endif
