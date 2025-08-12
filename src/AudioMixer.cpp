#include <M5GFX.h>

#include "AudioMixer.h"

extern M5GFX display;

void AudioMixer::begin()
{
    trackCount = 0;
}

void AudioMixer::addTrack(const char* filename, float gain) {
    printf("Adding track: %s, trackCount: %d, MAX_TRACKS: %d\n", 
           filename ? filename : "NULL", trackCount, MAX_TRACKS);
    
    if (!filename) {
        printf("ERROR: filename is NULL\n");
        return;
    }
    
    if (trackCount >= MAX_TRACKS) {
        printf("ERROR: too many tracks\n");
        return;
    }
    
    printf("Opening track at index %d\n", trackCount);
    bool success = tracks[trackCount].open(filename, gain);
    printf("Track open result: %s\n", success ? "success" : "failed");
    
    if (success) {
        trackCount++;
    }
}


size_t AudioMixer::mix(int16_t *outBuffer, size_t numSamples)
{
    if (!outBuffer || numSamples == 0) return 0;
    
    memset(outBuffer, 0, sizeof(int16_t) * numSamples);
    
    bool hasActiveAudio = false;
    
    for (int i = 0; i < trackCount; ++i)
    {
        
        if (!tracks[i].isActive()) continue;
        
        int16_t* temp = new int16_t[numSamples];
        size_t read = tracks[i].getSamples(temp, numSamples);
        
        
        if (read > 0) {
            hasActiveAudio = true;
            
            // DEBUG: Verificar si hay audio real (no solo ceros)
            bool hasNonZero = false;
            for (size_t j = 0; j < read; ++j)
            {
                if (temp[j] != 0) {
                    hasNonZero = true;
                    break;
                }
            }
            
            for (size_t j = 0; j < read; ++j)
            {
                int32_t sample = (int32_t)(temp[j] * tracks[i].gain);
                int32_t mixed = (int32_t)outBuffer[j] + sample;
                
                if (mixed > 32767) mixed = 32767;
                if (mixed < -32768) mixed = -32768;
                
                outBuffer[j] = (int16_t)mixed;
            }
        }
        
        delete[] temp;
    }
    
    return hasActiveAudio ? numSamples : 0;
}

bool AudioMixer::isActive() const
{
    for (int i = 0; i < trackCount; ++i)
    {
        if (tracks[i].isActive())
            return true;
    }
    return false;
}
