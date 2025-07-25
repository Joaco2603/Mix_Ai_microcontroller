#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "PromptSender.h"
#include "AudioPlayer.h"

// Display pins for M5Core2
#define TFT_CS 5
#define TFT_DC 15
#define TFT_RST -1  // Not used for M5Core2
#define TFT_MOSI 23 // Not needed for hardware SPI
#define TFT_MISO 38 // Not needed for hardware SPI
#define TFT_SCLK 18 // Not needed for hardware SPI

// Display setup
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
#define TFT_ROTATION 3 // Adjust based on your display orientation

// Touch setup (if needed)
#define TOUCH_CS -1 // Not used in your original code
XPT2046_Touchscreen touch(TOUCH_CS);

PromptSender *sender = nullptr;
AudioPlayer audioPlayer;

void clearScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);  // Resetear cursor a la posición inicial
}

void setup()
{
    Serial.begin(115200);

    // Initialize display
    tft.begin();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Iniciando sistema...");

    // Initialize audio
    audioPlayer.playFile("./music.mp3");

    // Create objects
    sender = new PromptSender();

    // Connect WiFi
    delay(1000);
    tft.fillScreen(ILI9341_BLACK);
    clearScreen();
    if (sender->initWiFi())
    {
        delay(1000);

        // Send initial prompt
        tft.fillScreen(ILI9341_BLACK);
        sender->sendPrompt("Sistema iniciado");
    }

    // Show controls
    tft.fillScreen(ILI9341_BLACK);
    tft.println("Controles:");
    tft.println("A: Subir volumen");
    tft.println("B: Bajar volumen");
    tft.println("C: Play/Pause");
}

void loop()
{
    audioPlayer.update();
}
