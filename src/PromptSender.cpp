#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "PromptSender.h"

// Configuración de red - cambiá estos valores
const char* PromptSender::ssid = "GARAJE";
const char* PromptSender::password = "JEjb1307*2603*";
const char* PromptSender::serverUrl = "http://192.168.1.23:5000/query";

PromptSender::PromptSender() {
    wifiConnected = false;
}

bool PromptSender::initWiFi() {
    M5.Display.setTextSize(2);
    M5.Display.println("Conectando WiFi...");
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        M5.Display.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        M5.Display.println("\nWiFi OK!");
        M5.Display.print("IP: ");
        M5.Display.println(WiFi.localIP());
        wifiConnected = true;
        return true;
    } else {
        M5.Display.println("\nError WiFi!");
        wifiConnected = false;
        return false;
    }
}

bool PromptSender::sendPrompt(const String& prompt) {
    if (!wifiConnected) {
        M5.Display.println("WiFi no conectado");
        return false;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        M5.Display.println("WiFi desconectado");
        wifiConnected = false;
        return false;
    }
    
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(10000); // 10 segundos timeout
    
    // Construir JSON payload
    String jsonPayload = "{\"prompt\": \"" + prompt + "\"}";
    
    M5.Display.println("Enviando...");
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        M5.Display.println("Respuesta:");
        M5.Display.println("Código: " + String(httpResponseCode));
        M5.Display.println(response);
        http.end();
        return true;
    } else {
        M5.Display.println("Error HTTP: " + String(httpResponseCode));
        http.end();
        return false;
    }
}

bool PromptSender::isWiFiConnected() {
    return wifiConnected && (WiFi.status() == WL_CONNECTED);
}