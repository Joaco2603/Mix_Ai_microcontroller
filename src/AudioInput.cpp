#include "AudioInput.h"
#include "driver/i2s.h"

void AudioInput::begin() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .dma_buf_count = 4,
        .dma_buf_len = 512,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 32,
        .ws_io_num = 0,
        .data_in_num = 34,
        .data_out_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

size_t AudioInput::read(uint8_t* buffer, size_t len) {
    size_t bytes_read;
    i2s_read(I2S_NUM_0, buffer, len, &bytes_read, portMAX_DELAY);
    return bytes_read;
}
