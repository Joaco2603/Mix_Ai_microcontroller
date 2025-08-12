#include "BufferedTrack.h"

bool BufferedTrack::open(const char* path, float g) {
    Serial.printf("Opening track: %s\n", path);
    
    gain = g;
    active = reader.open(path);
    
    if (!active) {
        Serial.printf("ERROR: Failed to open file: %s\n", path);
        return false;
    }
    
    bufferIndex = 0;
    bufferFill = 0;
    refill();
    
    Serial.printf("Track opened successfully: %s, bufferFill: %zu\n", 
                  path, bufferFill);
    return active && bufferFill > 0;
}

void BufferedTrack::refill() {
    if (!active) return;
    
    // Solo hacer refill si realmente necesitamos más datos
    if (bufferIndex < bufferFill) return;
    
    bufferFill = reader.readSamples(internalBuffer, BUFFER_SIZE);
    bufferIndex = 0;
    
    if (bufferFill == 0) {
        Serial.println("End of file reached");
        reader.close();
        active = false;
    }
}

size_t BufferedTrack::getSamples(int16_t* buffer, size_t numSamples) {
    if (!active || !buffer) {
        Serial.println("getSamples: not active or null buffer");
        return 0;
    }
    
    size_t samplesRead = 0;
    
    while (samplesRead < numSamples && active) {
        if (bufferIndex >= bufferFill) {
            refill();
            if (bufferIndex >= bufferFill) {
                Serial.println("No more samples available after refill");
                break;
            }
        }
        
        if (!active || bufferFill == 0) {
            Serial.println("Track became inactive");
            break;
        }
        
        // Verificación de bounds para array fijo
        if (bufferIndex >= BUFFER_SIZE) { 
            Serial.printf("ERROR: bufferIndex=%zu >= BUFFER_SIZE=%zu\n", 
                         bufferIndex, BUFFER_SIZE);
            break;
        }
        
        buffer[samplesRead++] = internalBuffer[bufferIndex++];
    }
    
    return samplesRead;
}

bool BufferedTrack::isActive() const {
    return active;
}
