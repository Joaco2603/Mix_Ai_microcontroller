#include <M5GFX.h>
#include "MyWebServer.h"
#include "AudioPlayer.h"

MyWebServer myWebServer;

AudioPlayer audioPlayer;
M5GFX display;

void setup()
{
  Serial.begin(115200); // ¡FALTABA!
  display.begin();
  myWebServer.start();

  // Limpiar pantalla y configurar
  display.clear();
  display.setTextSize(2);
  display.println("Iniciando M5Core2...");

  audioPlayer.begin();

  // Ahora sí reproducir
  audioPlayer.playFile("/music.MP3");
  delay(10000);

  // Conectar WiFi
  // Mostrar controles
  display.clear();
  display.println("Canales:");
  display.println("A: Guitarra");
  display.println("B: Piano");
  display.println("C: Bajo");
}

void loop()
{
  audioPlayer.update();
}