#include "AudioOutput.h"
#include "driver/i2s.h"

void AudioOutput::begin() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .dma_buf_count = 4,
        .dma_buf_len = 512,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 12,
        .ws_io_num = 0,
        .data_out_num = 2, // DAC
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_1, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &pin_config);
}

void AudioOutput::play(uint8_t* buffer, size_t len) {
    size_t bytes_written;
    i2s_write(I2S_NUM_1, buffer, len, &bytes_written, portMAX_DELAY);
}
