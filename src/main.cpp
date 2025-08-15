#include <M5GFX.h>
#include <Arduino.h>
#include <SD.h>

#include "AudioPlayer.h"
#include "MyWebServer.h"
#include "AudioMixer.h"
#include "AudioPlayer.h"

MyWebServer myWebServer;

WavReader reader;
AudioMixer mixer;
AudioPlayer audioPlayer;

int16_t mixedBuffer[512];


M5GFX display;

void setup()
{
  Serial.begin(115200);
  display.begin();
  SD.begin(4);
  audioPlayer.begin();

  myWebServer.start(&mixer, &audioPlayer);
  audioPlayer.setMixer(&mixer);

  // Limpiar pantalla y configurar
  display.clear();
  display.setTextSize(2);
  display.println("Iniciando M5Core2...");

  const char *files[] = {"/guitar_output.wav", "/vocal_output.wav", "/bass_output.wav", "/drum_output.wav"};
  audioPlayer.playMixedFiles(files);

  // Conectar WiFi
  // Mostrar controles
  display.clear();
  display.println("Canales:");
  display.println("A: Guitarra");
  display.println("B: Voz");
  display.println("C: Bajo");
  display.println("D: Bateria");
}

void loop()
{
  audioPlayer.update();
}