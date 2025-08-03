#include "MyWebServer.h"
#include "Routes.h"
#include "./adapter/WebSocketAdapter.h"

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Garaje";
const char* password = "Jejb2603*1307*";

AsyncWebServer webServer(80);
WebSocketAdapter webSocketAdapter(&webServer);

void MyWebServer::start() {
  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  // Inicializar rutas HTTP
  Routes::init(webServer);

  // Inicializar WebSockets
  initializeWebSockets();

  // Iniciar servidor
  webServer.begin();
}

void MyWebServer::initializeWebSockets() {
  webSocketAdapter.initialize();
}
