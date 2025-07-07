#include <WiFi.h>
#include <HTTPClient.h>
#include "driver/i2s.h"
#include "AudioTools.h" // de arduino-audio-tools, muy útil

// Configuración I2S para micrófono
#define I2S_WS  0      // LRCLK
#define I2S_SD  34     // DIN
#define I2S_SCK 32     // BCLK

I2SSampler *sampler;

void setup() {
  Serial.begin(115200);

  // Conectar WiFi
  WiFi.begin("tuSSID", "tuPASS");
  while (WiFi.status() != WL_CONNECTED) delay(1000);

  // Configurar entrada de audio
  auto config = i2s_config_t{
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000, // baja tasa para tamaño pequeño
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .dma_buf_count = 4,
    .dma_buf_len = 512,
    .use_apll = false
  };

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, new i2s_pin_config_t{I2S_SCK, I2S_WS, I2S_SD, I2S_PIN_NO_CHANGE});
}

// Envío de audio comprimido al backend
void loop() {
  uint8_t buffer[512];
  size_t bytesRead;
  i2s_read(I2S_NUM_0, buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);

  // Aquí podrías aplicar compresión µ-law (G.711) o ADPCM
  // Luego enviar por HTTP POST
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://tu_server_llm.com/audio_input");
    http.addHeader("Content-Type", "application/octet-stream");
    http.POST(buffer, bytesRead);
    http.end();
  }
}
