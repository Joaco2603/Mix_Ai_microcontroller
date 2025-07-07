#include <Arduino.h>
#include "AudioInput.h"
#include "AudioOutput.h"
#include "NetworkClient.h"
#include "MuLawEncoder.h"
#include "Config.h"

AudioInput mic;
AudioOutput speaker;
NetworkClient net;

// Buffers para audio
int16_t pcm_buffer[256];  // Buffer para datos PCM sin comprimir (16 bits por muestra)
uint8_t ulaw_encoded[256]; // Buffer para datos comprimidos MuLaw (8 bits por muestra)

void setup() {
    Serial.begin(115200);
    mic.begin();
    speaker.begin();
    net.connectWifi(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
    // Captura audio PCM (asumiendo que mic.read devuelve datos PCM 16-bit)
    size_t bytesRead = mic.read((uint8_t*)pcm_buffer, sizeof(pcm_buffer));
    int samplesRead = bytesRead / sizeof(int16_t); // Calcula cuántas muestras PCM se leyeron
    
    // Comprime a MuLaw
    for (int i = 0; i < samplesRead; i++) {
        ulaw_encoded[i] = linear_to_ulaw(pcm_buffer[i]);
    }
    
    // Envía audio comprimido
    net.sendAudio(ulaw_encoded, samplesRead);

    // Espera, luego intenta recibir audio
    delay(3000);
    size_t recv = net.fetchAudio((uint8_t*)pcm_buffer, sizeof(pcm_buffer));
    if (recv > 0) {
        // Asumiendo que el audio recibido ya está en formato MuLaw
        // y que el speaker.play puede manejarlo directamente
        speaker.play((uint8_t*)pcm_buffer, recv);
    }
}