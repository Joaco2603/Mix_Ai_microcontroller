#include <WiFi.h>
#include <HTTPClient.h>
#include "PromptSender.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft; // Reference to the display object from main file

// Network configuration - change these values
const char* PromptSender::ssid = "GARAJE";
const char* PromptSender::password = "JEjb1307*2603*";
const char* PromptSender::serverUrl = "http://192.168.1.23:5000/query";

PromptSender::PromptSender() {
    wifiConnected = false;
}

bool PromptSender::initWiFi() {
    tft.setTextSize(2);
    tft.println("Conectando WiFi...");
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        tft.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        tft.println("\nWiFi OK!");
        tft.print("IP: ");
        tft.println(WiFi.localIP());
        wifiConnected = true;
        return true;
    } else {
        tft.println("\nError WiFi!");
        wifiConnected = false;
        return false;
    }
}

bool PromptSender::sendPrompt(const String& prompt) {
    if (!wifiConnected) {
        tft.println("WiFi no conectado");
        return false;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        tft.println("WiFi desconectado");
        wifiConnected = false;
        return false;
    }
    
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(10000); // 10 segundos timeout
    
    // Build JSON payload
    String jsonPayload = "{\"prompt\": \"" + prompt + "\"}";
    
    tft.println("Enviando...");
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        tft.println("Respuesta:");
        tft.println("Código: " + String(httpResponseCode));
        tft.println(response);
        http.end();
        return true;
    } else {
        tft.println("Error HTTP: " + String(httpResponseCode));
        http.end();
        return false;
    }
}

bool PromptSender::isWiFiConnected() {
    return wifiConnected && (WiFi.status() == WL_CONNECTED);
}