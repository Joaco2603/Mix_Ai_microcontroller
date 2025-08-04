#include "BufferedTrack.h"

bool BufferedTrack::open(const char* path, float g) {
    gain = g;
    active = reader.open(path);
    bufferIndex = 0;
    bufferFill = 0;
    refill();
    return active;
}

void BufferedTrack::refill() {
    if (!active) return;
    
    // Solo hacer refill si realmente necesitamos más datos
    // (cuando hemos consumido todo el buffer)
    if (bufferIndex < bufferFill) return;
    
    bufferFill = reader.readSamples(internalBuffer, BUFFER_SIZE);
    bufferIndex = 0;
    
    if (bufferFill == 0) {
        reader.close();
        active = false;
    }
}

// size_t BufferedTrack::getSamples(int16_t* buffer, size_t numSamples) {
//     if (!active) return 0;

//     size_t samplesRead = 0;
//     while (samplesRead < numSamples && active) {
//         if (bufferIndex >= bufferFill) refill();
//         if (!active || bufferFill == 0) break;

//         buffer[samplesRead++] = internalBuffer[bufferIndex++];
//     }
//     return samplesRead;
// }

size_t BufferedTrack::getSamples(int16_t* buffer, size_t numSamples) {
    if (!active || !buffer || !internalBuffer) return 0;
    
    size_t samplesRead = 0;
    while (samplesRead < numSamples && active) {
        if (bufferIndex >= bufferFill) {
            refill();
            // CRÍTICO: Verificar que refill() resetee bufferIndex
            if (bufferIndex >= bufferFill) {
                // Si refill() falló o no reseteo bufferIndex
                Serial.printf("ERROR: After refill bufferIndex=%zu >= bufferFill=%zu\n", 
                             bufferIndex, bufferFill);
                break;
            }
        }
        
        if (!active || bufferFill == 0) break;
        
        // Verificación adicional de seguridad
        if (bufferIndex >= 2048) {  // asumiendo que tienes bufferCapacity
            Serial.printf("ERROR: bufferIndex=%zu >= bufferCapacity\n", bufferIndex);
            break;
        }
        
        buffer[samplesRead++] = internalBuffer[bufferIndex++];
    }
    return samplesRead;
}


bool BufferedTrack::isActive() const {
    return active;
}
