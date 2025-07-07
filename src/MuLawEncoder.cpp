#include "MuLawEncoder.h"

#define BIAS 0x84
#define CLIP 32635

uint8_t linear_to_ulaw(int16_t pcm_val) {
    int mask;
    int seg;
    uint8_t uval;

    // Get the sign and the magnitude of the value.
    pcm_val = pcm_val >> 2; // reduce resolution (opcional)
    if (pcm_val < 0) {
        pcm_val = -pcm_val;
        mask = 0x7F;
    } else {
        mask = 0xFF;
    }
    if (pcm_val > CLIP) pcm_val = CLIP;
    pcm_val += BIAS;

    // Convert the scaled magnitude to segment number.
    static const int seg_end[8] = {
        0x1F, 0x3F, 0x7F, 0xFF,
        0x1FF, 0x3FF, 0x7FF, 0xFFF
    };

    for (seg = 0; seg < 8; seg++) {
        if (pcm_val <= seg_end[seg]) break;
    }

    // Combine the sign, segment, and quantization bits.
    uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
    return uval ^ mask;
}
