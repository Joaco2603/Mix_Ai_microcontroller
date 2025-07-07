#pragma once

#include <stdint.h>

// Convierte una muestra de 16 bits PCM a µ-law comprimido de 8 bits
uint8_t linear_to_ulaw(int16_t sample);
