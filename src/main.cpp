// #include <M5Core2.h>
// #include <WiFi.h>
// #include <HTTPClient.h>

// Cambia estos por los de tu red y servidor

// void setup() {
//   // ... (código anterior)
//   WiFi.begin(ssid, password);

//   int intentos = 0;
//   while (WiFi.status() != WL_CONNECTED && intentos < 2) {
//     delay(2000);
//     M5.Lcd.print(".");
//     intentos++;
//   }

//   // Diagnóstico detallado
//   if (WiFi.status() == WL_CONNECTED) {
//     M5.Lcd.println("\nConectado!");
//     M5.Lcd.print("IP: ");
//     M5.Lcd.println(WiFi.localIP());
//   } else {
//     M5.Lcd.println("\nError de conexión");
//     switch (WiFi.status()) {
//       case WL_NO_SSID_AVAIL:
//         M5.Lcd.println("Red no encontrada");
//         break;
//       case WL_CONNECT_FAILED:
//         M5.Lcd.println("Contraseña incorrecta");
//         break;
//       case WL_DISCONNECTED:
//         M5.Lcd.println("Desconectado (router rechazó la conexión)");
//         break;
//       default:
//         M5.Lcd.print("Código de error: ");
//         M5.Lcd.println(WiFi.status());
//     }
//   }
// }

// void setup() {
//   M5.begin();
//   M5.Lcd.setTextSize(2);
//   M5.Lcd.print("Conectando WiFi...");

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(2000);
//     M5.Lcd.print(".");
//   }

//   M5.Lcd.println("\nConectado!");

//   HTTPClient http;
//   http.begin(serverUrl);
//   http.addHeader("Content-Type", "application/json");

//   // JSON con el prompt para Langchain
//   String jsonPayload = "{\"prompt\": \"Subí el volumen del canal izquierdo\"}";

//   int httpResponseCode = http.POST(jsonPayload);

//   if (httpResponseCode > 0) {
//     String response = http.getString();
//     M5.Lcd.println("Respuesta:");
//     M5.Lcd.println(response);
//   } else {
//     M5.Lcd.print("Error HTTP: ");
//     M5.Lcd.println(httpResponseCode);
//   }

//   http.end();
// }

// void loop() {
//   // nada, o podrías dejar botón para enviar nuevos prompts
// }

// #include <WiFi.h>
// #include <WiFiUdp.h>
// #include <driver/i2s.h>

// WiFiUDP udp;
// const int port = 12345;
// uint8_t buffer[512];

// void setupWiFi() {
//   WiFi.begin("SSID", "PASSWORD");
//   while (WiFi.status() != WL_CONNECTED) delay(500);
//   udp.begin(port);
// }

// void setupI2S() {
//   i2s_config_t config = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//     .sample_rate = 16000,
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = I2S_COMM_FORMAT_I2S_MSB,
//     .intr_alloc_flags = 0,
//     .dma_buf_count = 8,
//     .dma_buf_len = 64,
//     .use_apll = false
//   };

//   i2s_pin_config_t pin_config = {
//     .bck_io_num = 26,
//     .ws_io_num = 25,
//     .data_out_num = 22,
//     .data_in_num = I2S_PIN_NO_CHANGE
//   };

//   i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
//   i2s_set_pin(I2S_NUM_0, &pin_config);
// }

// void setup() {
//   setupWiFi();
//   setupI2S();
// }

// void loop() {
//   int len = udp.parsePacket();
//   if (len) {
//     udp.read(buffer, sizeof(buffer));
//     size_t bytes_written;
//     i2s_write(I2S_NUM_0, buffer, len, &bytes_written, portMAX_DELAY);
//   }
// }


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
