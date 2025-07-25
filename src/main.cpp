#include <M5Unified.h>
#include "PromptSender.h"
#include "AudioPlayer.h"

PromptSender* sender = nullptr;
AudioPlayer audioPlayer;

void setup() {
    // Configurar M5Unified
    auto cfg = M5.config();
    M5.begin(cfg);

    audioPlayer.playFile("/music.mp3");
    
    // Limpiar pantalla y configurar
    M5.Display.clear();
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setTextSize(2);
    M5.Display.println("Iniciando M5Core2...");
    
    // Crear objetos después de inicializar M5
    sender = new PromptSender();
    
    // Conectar WiFi
    delay(1000);
    M5.Display.clear();
    if (sender->initWiFi()) {
        delay(1000);
        
        // Enviar prompt inicial
        M5.Display.clear();
        sender->sendPrompt("Sistema iniciado");

    }
    
    // Mostrar controles
    M5.Display.clear();
    M5.Display.println("Controles:");
    M5.Display.println("A: Subir volumen");
    M5.Display.println("B: Bajar volumen");
    M5.Display.println("C: Play/Pause");
}

void loop() {
  audioPlayer.update(); 
}