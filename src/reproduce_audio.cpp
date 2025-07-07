#include <WiFiClient.h>
#include <HTTPClient.h>
#include "AudioOutputI2S.h"
#include "AudioGeneratorWAV.h"

AudioOutputI2S *out = nullptr;
AudioGeneratorWAV *wav = nullptr;
WiFiClient client;

void loop() {
  // Reproducir audio de servidor
  HTTPClient http;
  http.begin("http://tu_server_llm.com/audio_output.wav");
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    client = http.getStream();

    out = new AudioOutputI2S();
    out->begin();

    wav = new AudioGeneratorWAV();
    wav->begin(client, out);

    while (wav->isRunning()) {
      wav->loop();
    }

    wav->stop();
    http.end();
  }

  delay(5000); // espera antes de siguiente reproducción
}
