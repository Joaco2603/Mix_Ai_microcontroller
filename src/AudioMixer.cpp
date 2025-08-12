#include <M5GFX.h>
#include "AudioMixer.h"
#include "BufferedTrack.h"

extern M5GFX display;

void AudioMixer::begin()
{
    channelCount = 0;
    for (int i = 0; i < MAX_CHANNELS; ++i)
    {
        channels[i].name = nullptr;
        channels[i].gain = 1.0f;
        channels[i].track.gain = 1.0f;
    }
}

void AudioMixer::addTrack(const char *filename, float gain)
{
    if (!filename)
        return;
    if (channelCount >= MAX_CHANNELS)
        return;

    printf("Opening track at index %d\n", channelCount);
    bool success = channels[channelCount].track.open(filename, gain);

    BufferedTrack *bt = new BufferedTrack();

    if (success)
    {
        channels[channelCount].name = filename;
        channels[channelCount].gain = gain;
        channels[channelCount].track = *bt;
        channels[channelCount].track.open(filename, gain);

        printf("Track %s opened successfully with gain %.2f\n", filename, gain);
        channelCount++;
    }
}

bool AudioMixer::allTracksFinished() const
{
    for (int i = 0; i < channelCount; i++)
    {
        if (channels[i].track.isActive())
        {
            return false;
        }
    }
    return true;
}

void AudioMixer::setChannelGain(int channel, float gain)
{
    if (channel < 0 || channel >= AudioMixer::MAX_CHANNELS)
    {
        return;
    }

    if (!channels[channel].track.isActive())
    {
        Serial.printf("Channel %d is not active, cannot set gain.\n", channel);
        return;
    }

    if(!channels[channel].gain){
        Serial.printf("Channel %d gain is zero, cannot set gain.\n", channel);
        return;
    }

    if (channelCount < 0)
    {
        Serial.println("ERROR: Mixer not initialized");
        return;
    }

    channels[channel].gain = gain;
    channels[channel].track.gain = gain;
}

float AudioMixer::getChannelGain(int channel) const
{
    if (channel < 0 || channel >= AudioMixer::MAX_CHANNELS)
        return 0.0f;
    return channels[channel].gain;
}

size_t AudioMixer::mix(int16_t *outBuffer, size_t numSamples)
{
    if (!outBuffer || numSamples == 0)
    {
        return 0;
    }

    memset(outBuffer, 0, sizeof(int16_t) * numSamples);

    bool hasActiveAudio = false;
    int16_t temp[2048];

    for (int i = 0; i < channelCount; ++i)
    {
        size_t read = channels[i].track.getSamples(temp, numSamples);

        if (read > 0)
        {
            hasActiveAudio = true;
            for (size_t j = 0; j < read; ++j)
            {
                int32_t sample = (int32_t)(temp[j] * channels[i].gain);
                int32_t mixed = (int32_t)outBuffer[j] + sample;
                if (mixed > 32767)
                    mixed = 32767;
                if (mixed < -32768)
                    mixed = -32768;
                outBuffer[j] = (int16_t)mixed;
            }
        }
    }
    return hasActiveAudio ? numSamples : 0;
}

bool AudioMixer::isActive() const
{
    for (int i = 0; i < channelCount; ++i)
    {
        if (channels[i].track.isActive())
            return true;
    }
    return false;
}
